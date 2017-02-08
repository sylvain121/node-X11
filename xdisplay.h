void display_init(const char * displayname, int * desktopWidth, int * desktopHeight, int *desktopDepth);
void display_image( Image* image );
void display_keypress( int keycode, bool isDown );
void display_mouse_move( int x, int y );
void display_mouse_button(int button, bool isDown );
