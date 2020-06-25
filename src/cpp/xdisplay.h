
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/ipc.h>
#include <X11/X.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <sys/shm.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/Xrandr.h>
#include "export.h"

	int display_init(const char *displayname, int *desktopWidth, int *desktopHeight, int *desktopDepth);
	void display_image(Image *image, int xoffset, int yoffset, int width, int height);
	void display_keypress_with_keycode(int keycode, bool isDown);
	void display_keypress_with_keysym(int keysym, bool isDown);
	void display_mouse_move(int x, int y);
	void display_mouse_button(int button, bool isDown);
	XFixesCursorImage* get_mouse_pointer();
	void get_screen_resolutions(XRRScreenSize **sizes, int *length);
	void get_current_screen_resolution(int *width, int *height);
	int set_current_screen_resolution(int id);
	void close();
#ifdef __cplusplus
}
#endif
