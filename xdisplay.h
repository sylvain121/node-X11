
#ifdef __cplusplus
extern "C" {
#endif


void display_init(const char * displayname, int * desktopWidth, int * desktopHeight, int *desktopDepth);
void display_image( Image* image );
void display_keypress_with_keycode( int keycode, bool isDown );
void display_keypress_with_keysym( int keysym, bool isDown );
void display_mouse_move( int x, int y );
void display_mouse_button(int button, bool isDown );
void close();
#ifdef __cplusplus
}
#endif
