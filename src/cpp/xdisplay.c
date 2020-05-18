#ifdef __cplusplus
extern "C" {
#endif

#include "xdisplay.h"

	//X11GRAB vars
	static int screenNumber;
	static Display *display = NULL;
	static Window rootWindow;
	static Screen *screen = NULL;
	XColor color;
	XImage * ximage = NULL;
	static XShmSegmentInfo __xshminfo;


	int display_init(const char * displayname, int * desktopWidth, int * desktopHeight, int *desktopDepth) 
	{
		// init
		int ignore = 0;
		bzero(&__xshminfo, sizeof(__xshminfo));

		// open display
		if((display = XOpenDisplay(displayname)) == NULL) {
			printf("cannot open display \"%s\"\n", displayname ? displayname : "DEFAULT");
			return -1;
		}

		// check MIT extension
		if(XQueryExtension(display, "MIT-SHM", &ignore, &ignore, &ignore) ) {
			int major, minor;
			Bool pixmaps;
			if(XShmQueryVersion(display, &major, &minor, &pixmaps) == True) {
				printf("XShm extention version %d.%d %s shared pixmaps\n",
						major, minor, (pixmaps==True) ? "with" : "without");
			} else {
				printf("XShm extension not supported.\n");
			}
		}
		// get default screen
		screenNumber = XDefaultScreen(display);
		if((screen = XScreenOfDisplay(display, screenNumber)) == NULL) {
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
		if((ximage = XShmCreateImage(display,
						XDefaultVisual(display, screenNumber),
						*desktopDepth, ZPixmap, NULL, &__xshminfo,
						*desktopWidth, *desktopHeight)) == NULL) {
			printf("XShmCreateImage failed.\n");
		}


		//get shm info
		if((__xshminfo.shmid = shmget(IPC_PRIVATE,
						ximage->bytes_per_line*ximage->height,
						IPC_CREAT | 0777)) < 0) {
			printf("shmget error");
		}

		//
		__xshminfo.shmaddr = ximage->data = (char*) shmat(__xshminfo.shmid, 0, 0);
		__xshminfo.readOnly = False;
		if(XShmAttach(display, &__xshminfo) == 0) {
			printf("XShmAttach failed.\n");
		}
		//
		rootWindow = XRootWindow(display, screenNumber);

	}

	void display_image( Image *image, bool withPointer )
	{

		if(XShmGetImage(display, rootWindow, ximage, 0, 0, XAllPlanes()) == 0) {
			// image->data RGBA
			printf("FATAL: XShmGetImage failed.\n");
			exit(-1);
		}
		image->width = ximage->width;
		image->height = ximage->height;
		memcpy(image->data, ximage->data, ximage->width * ximage->height * 4);
		image->depth = ximage->depth;
		image->bits_per_pixel = ximage->bits_per_pixel;
		image->bytes_per_line = ximage->bytes_per_line;
		if(withPointer)
		{
			paint_mouse_pointer(image);
		}

	}
	void display_keypress_with_keycode( int keycode, bool isDown )
	{
		XLockDisplay(display);
		XTestGrabControl(display, True);
		XTestFakeKeyEvent(display,keycode, isDown ? True : False, CurrentTime);
		XFlush(display);
		XTestGrabControl(display, False);
		XUnlockDisplay(display);
	}

	void display_keypress_with_keysym( int keysym, bool isDown )
	{

		XLockDisplay(display);
		XTestGrabControl(display, True);
		XTestFakeKeyEvent(display,XKeysymToKeycode(display, keysym),isDown, CurrentTime);
		XFlush(display);
		XTestGrabControl(display, False);
		XUnlockDisplay(display);
	}

	void display_mouse_move( int x, int y )
	{
		XLockDisplay(display);
		XTestGrabControl(display, True);
		XTestFakeMotionEvent(display, screenNumber, x, y, CurrentTime);
		XFlush(display);
		XTestGrabControl(display, False);
		XUnlockDisplay(display);
	}

	void display_mouse_button(int button, bool isDown ) 
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

	void paint_mouse_pointer(Image *image)
	{
		int x_off = 0;
		int y_off = 0;
		XFixesCursorImage *xcim;
		int width = image->width;
		int height = image->height;
		int x, y;
		int line, column;
		int to_line, to_column;
		int pixstride = image->bits_per_pixel >> 3;
		/* Warning: in its insanity, xlib provides unsigned image data through a
		 * char* pointer, so we have to make it uint8_t to make things not break.
		 * Anyone who performs further investigation of the xlib API likely risks
		 * permanent brain damage. */
		uint8_t *pix = image->data;
		/* Code doesn't currently support 16-bit or PAL8 */
		if (image->bits_per_pixel != 24 && image->bits_per_pixel != 32)
			return;
		xcim = XFixesGetCursorImage(display);
		x = xcim->x - xcim->xhot;
		y = xcim->y - xcim->yhot;
		to_line = FFMIN((y + xcim->height), (height + y_off));
		to_column = FFMIN((x + xcim->width), (width + x_off));
		for (line = FFMAX(y, y_off); line < to_line; line++) {
			for (column = FFMAX(x, x_off); column < to_column; column++) {
				int  xcim_addr = (line - y) * xcim->width + column - x;
				int image_addr = ((line - y_off) * width + column - x_off) * pixstride;
				int r = (uint8_t)(xcim->pixels[xcim_addr] >>  0);
				int g = (uint8_t)(xcim->pixels[xcim_addr] >>  8);
				int b = (uint8_t)(xcim->pixels[xcim_addr] >> 16);
				int a = (uint8_t)(xcim->pixels[xcim_addr] >> 24);
				if (a == 255) {
					pix[image_addr+0] = r;
					pix[image_addr+1] = g;
					pix[image_addr+2] = b;
				} else if (a) {
					/* pixel values from XFixesGetCursorImage come premultiplied by alpha */
					pix[image_addr+0] = r + (pix[image_addr+0]*(255-a) + 255/2) / 255;
					pix[image_addr+1] = g + (pix[image_addr+1]*(255-a) + 255/2) / 255;
					pix[image_addr+2] = b + (pix[image_addr+2]*(255-a) + 255/2) / 255;
				}
			}
		}
		XFree(xcim);
		xcim = NULL;
	}
	void close() 
	{
		if(display) {
			XCloseDisplay(display);
			display = NULL;

		}
	}

#ifdef __cplusplus
}
#endif
