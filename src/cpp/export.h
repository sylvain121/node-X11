//TODO ugly
static int MOUSE_LEFT = 1;
static int MOUSE_MIDDLE = 2;
static int MOUSE_RIGHT = 3;


typedef struct image {
	int width;
	int height;
	char * data;
	int depth;
	int bytes_per_line;
	int bits_per_pixel;
} Image ;
