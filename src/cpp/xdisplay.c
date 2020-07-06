#ifdef __cplusplus
extern "C"
{
#endif

#include "xdisplay.h"

	//X11GRAB vars
	static int screenNumber;
	static Display *display = NULL;
	static Window rootWindow;
	static Screen *screen = NULL;
	XColor color;
	XImage *ximage = NULL;
	XRRScreenResources *rr_screen;
	static XShmSegmentInfo __xshminfo;
	Rotation rotation = 1;
	int reflection = 0;

	int display_init(const char *displayname, int *desktopWidth, int *desktopHeight, int *desktopDepth)
	{
		// init
		int ignore = 0;
		XInitThreads();
		bzero(&__xshminfo, sizeof(__xshminfo));

		// open display
		if ((display = XOpenDisplay(displayname)) == NULL)
		{
			printf("cannot open display \"%s\"\n", displayname ? displayname : "DEFAULT");
			return -1;
		}

		// check MIT extension
		if (XQueryExtension(display, "MIT-SHM", &ignore, &ignore, &ignore))
		{
			int major, minor;
			Bool pixmaps;
			if (XShmQueryVersion(display, &major, &minor, &pixmaps) == True)
			{
				printf("XShm extention version %d.%d %s shared pixmaps\n",
					   major, minor, (pixmaps == True) ? "with" : "without");
			}
			else
			{
				printf("XShm extension not supported.\n");
			}
		}
		// get default screen
		screenNumber = XDefaultScreen(display);
		if ((screen = XScreenOfDisplay(display, screenNumber)) == NULL)
		{
			printf("cannot obtain screen #%d\n", screenNumber);
		}
		// get screen hight, width, depth
		*desktopWidth = XDisplayWidth(display, screenNumber);
		*desktopHeight = XDisplayHeight(display, screenNumber);
		*desktopDepth = XDisplayPlanes(display, screenNumber);
		printf("X-Window-init: dimension: %dx%dx%d @ %d/%d\n",
			   *desktopWidth, *desktopHeight, *desktopDepth,
			   screenNumber, XScreenCount(display));
		//create image context
		if ((ximage = XShmCreateImage(display,
									  XDefaultVisual(display, screenNumber),
									  *desktopDepth, ZPixmap, NULL, &__xshminfo,
									  *desktopWidth, *desktopHeight)) == NULL)
		{
			printf("XShmCreateImage failed.\n");
		}

		//get shm info
		if ((__xshminfo.shmid = shmget(IPC_PRIVATE,
									   ximage->bytes_per_line * ximage->height,
									   IPC_CREAT | 0777)) < 0)
		{
			printf("shmget error");
		}

		//
		__xshminfo.shmaddr = ximage->data = (char *)shmat(__xshminfo.shmid, 0, 0);
		__xshminfo.readOnly = False;
		if (XShmAttach(display, &__xshminfo) == 0)
		{
			printf("XShmAttach failed.\n");
		}
		//
		rootWindow = XRootWindow(display, screenNumber);
		// Xrandr init
		rr_screen = XRRGetScreenResources(display, rootWindow);
		int errorbase;
		int eventbase;
		XRRQueryExtension(display, &eventbase, &errorbase);
	}

	void display_image(Image *image, int xoffset, int yoffset, int width, int height)
	{
		XLockDisplay(display);
		if (xoffset > 0 || yoffset > 0 || width > 0 || height > 0)
		{

			ximage = XGetImage(display, rootWindow, xoffset, yoffset, width, height, XAllPlanes(), ZPixmap);
		}
		else
		{
			if (XShmGetImage(display, rootWindow, ximage, 0, 0, XAllPlanes()) == 0)
			{
				// image->data RGBA
				printf("FATAL: XShmGetImage failed.\n");
				exit(-1);
			}
		}

		XUnlockDisplay(display);

		image->width = ximage->width;
		image->height = ximage->height;
		memcpy(image->data, ximage->data, ximage->width * ximage->height * 4);
		image->depth = ximage->depth;
		image->bits_per_pixel = ximage->bits_per_pixel;
		image->bytes_per_line = ximage->bytes_per_line;
		/*if (withPointer)
		{
			paint_mouse_pointer(image);
		}*/
	}
	void display_keypress_with_keycode(int keycode, bool isDown)
	{
		XLockDisplay(display);
		XTestGrabControl(display, True);
		XTestFakeKeyEvent(display, keycode, isDown ? True : False, CurrentTime);
		XFlush(display);
		XTestGrabControl(display, False);
		XUnlockDisplay(display);
	}

	void display_keypress_with_keysym(int keysym, bool isDown)
	{

		XLockDisplay(display);
		XTestGrabControl(display, True);
		XTestFakeKeyEvent(display, XKeysymToKeycode(display, keysym), isDown, CurrentTime);
		XFlush(display);
		XTestGrabControl(display, False);
		XUnlockDisplay(display);
	}

	void display_mouse_move(int x, int y)
	{
		XLockDisplay(display);
		XTestGrabControl(display, True);
		XTestFakeMotionEvent(display, screenNumber, x, y, CurrentTime);
		XFlush(display);
		XTestGrabControl(display, False);
		XUnlockDisplay(display);
	}

	void display_mouse_button(int button, bool isDown)
	{

		XLockDisplay(display);
		XTestGrabControl(display, True);
		XTestFakeButtonEvent(display, button, isDown ? True : False, CurrentTime);
		XFlush(display);
		XTestGrabControl(display, False);
		XUnlockDisplay(display);
	}

	/**
	 * from x11grab.c in ffmpeg project
	 */

	XFixesCursorImage* get_mouse_pointer()
	{
		
		XLockDisplay(display);
		XFixesCursorImage* cursor = XFixesGetCursorImage(display);
		XUnlockDisplay(display);
		return cursor;
	}

	void get_current_screen_resolution(int *width, int *height)
	{
		//0 to get the first monitor
		XRRCrtcInfo *crtc_info = XRRGetCrtcInfo(display, rr_screen, rr_screen->crtcs[0]);
		*width = crtc_info->width;
		*height = crtc_info->height;
	}

	int set_current_screen_resolution(int id)
	{
		printf("rez id %d", id);
		XRRScreenConfiguration *sc = XRRGetScreenInfo(display, rootWindow);
		return XRRSetScreenConfig(display, sc, rootWindow, (SizeID)id, (Rotation)(rotation | reflection), CurrentTime);
	}

	void get_screen_resolutions(XRRScreenSize **sizes, int *length)
	{
		Rotation current_rotation;
		XRRScreenConfiguration *sc = XRRGetScreenInfo(display, rootWindow);
		SizeID current_size = XRRConfigCurrentConfiguration(sc, &current_rotation);

		*sizes = XRRConfigSizes(sc, length);
	}

	void close()
	{
		if (display)
		{
			XCloseDisplay(display);
			display = NULL;
		}
	}

#ifdef __cplusplus
}
#endif
