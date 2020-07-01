#include <nan.h>
#include <iostream>
#include <cstdlib>
#include "xdisplay.h"

using namespace v8;

using v8::FunctionTemplate;

char *pDisplay = std::getenv("DISPLAY");
int width = 0;
int height = 0;
int depth = 0;
int xoffset = 0;
int yoffset = 0;
int offsetWidth = 0;
int offsetHeight = 0;

class GetImageWorker : public Nan::AsyncWorker
{
public:
	int xoffset;
	int yoffset;
	int offsetWidth;
	int offsetHeight;
	Image *image;

	GetImageWorker(int xoffset, int yoffset, int offsetWidth, int offsetHeight, Nan::Callback *callback)
		: Nan::AsyncWorker(callback)
	{
		this->xoffset = xoffset;
		this->yoffset = yoffset;
		this->offsetWidth = offsetWidth;
		this->offsetHeight = offsetHeight;
	}

	void Execute()
	{
		image = (Image *)malloc(sizeof(Image));
		image->data = (char *)malloc(sizeof(char) * width * height * 4);
		display_image(image, xoffset, yoffset, offsetWidth, offsetHeight);
	}

	void HandleOKCallback()
	{
		Nan::HandleScope scope;
		uint32_t bufferSize = image->width * image->height * (image->bits_per_pixel / 8);
		Local<Object> buffer = Nan::CopyBuffer((char *)image->data, bufferSize).ToLocalChecked();
		Local<Object> obj = Nan::New<Object>();
		Nan::Set(obj, Nan::New("width").ToLocalChecked(), Nan::New<Number>(image->width));
		Nan::Set(obj, Nan::New("height").ToLocalChecked(), Nan::New<Number>(image->height));
		Nan::Set(obj, Nan::New("depth").ToLocalChecked(), Nan::New<Number>(image->depth));
		Nan::Set(obj, Nan::New("bits_per_pixel").ToLocalChecked(), Nan::New<Number>(image->bits_per_pixel));
		Nan::Set(obj, Nan::New("bytes_per_line").ToLocalChecked(), Nan::New<Number>(image->bytes_per_line));
		Nan::Set(obj, Nan::New("data").ToLocalChecked(), buffer);
		free(image->data);
		free(image);
		v8::Local<v8::Value> argv[] = {
			Nan::Null(), // no error occured
			obj

		};
		callback->Call(2, argv);
	}

	void HandleErrorCallback()
	{
		Nan::HandleScope scope;
		v8::Local<v8::Value> argv[] = {
			Nan::New(this->ErrorMessage()).ToLocalChecked(), // return error message
			Nan::Null()

		};
		callback->Call(2, argv);
	}
};

NAN_METHOD(init)
{

	if (info.Length() == 1 && info[0]->IsString())
	{
		Nan::Utf8String dstr(info[0]);
		pDisplay = *dstr;
	}

	std::cout << "init on display " << pDisplay << std::endl;
	display_init(pDisplay, &width, &height, &depth);

	Local<Object> obj = Nan::New<Object>();
	Nan::Set(obj, Nan::New("width").ToLocalChecked(), Nan::New<Number>(width));
	Nan::Set(obj, Nan::New("height").ToLocalChecked(), Nan::New<Number>(height));
	Nan::Set(obj, Nan::New("depth").ToLocalChecked(), Nan::New<Number>(depth));
	info.GetReturnValue().Set(obj);
}

NAN_METHOD(getImage)
{
	if (info[0]->IsNumber())
	{
		xoffset = info[0]->NumberValue();
	}
	if (info[1]->IsNumber())
	{
		yoffset = info[1]->NumberValue();
	}
	if (info[2]->IsNumber())
	{
		offsetWidth = info[2]->NumberValue();
	}
	else
	{
		offsetWidth = 0;
	}
	if (info[3]->IsNumber())
	{
		offsetHeight = info[3]->NumberValue();
	}
	else
	{
		offsetHeight = 0;
	}

	Nan::AsyncQueueWorker(new GetImageWorker(
		xoffset, yoffset, offsetWidth, offsetHeight,
		new Nan::Callback(info[4].As<v8::Function>())));
}

NAN_METHOD(getImageSync)
{

	if (info[0]->IsNumber())
	{
		xoffset = info[0]->NumberValue();
	}
	if (info[1]->IsNumber())
	{
		yoffset = info[1]->NumberValue();
	}
	if (info[2]->IsNumber())
	{
		offsetWidth = info[2]->NumberValue();
	}
	else
	{
		offsetWidth = 0;
	}
	if (info[3]->IsNumber())
	{
		offsetHeight = info[3]->NumberValue();
	}
	else
	{
		offsetHeight = 0;
	}

	Image *image = (Image *)malloc(sizeof(Image));
	image->data = (char *)malloc(sizeof(char) * width * height * 4);
	display_image(image, xoffset, yoffset, offsetWidth, offsetHeight);
	uint32_t bufferSize = image->width * image->height * (image->bits_per_pixel / 8);
	Local<Object> buffer = Nan::CopyBuffer((char *)image->data, bufferSize).ToLocalChecked();

	Local<Object> obj = Nan::New<Object>();
	Nan::Set(obj, Nan::New("width").ToLocalChecked(), Nan::New<Number>(image->width));
	Nan::Set(obj, Nan::New("height").ToLocalChecked(), Nan::New<Number>(image->height));
	Nan::Set(obj, Nan::New("depth").ToLocalChecked(), Nan::New<Number>(image->depth));
	Nan::Set(obj, Nan::New("bits_per_pixel").ToLocalChecked(), Nan::New<Number>(image->bits_per_pixel));
	Nan::Set(obj, Nan::New("bytes_per_line").ToLocalChecked(), Nan::New<Number>(image->bytes_per_line));
	Nan::Set(obj, Nan::New("data").ToLocalChecked(), buffer);

	free(image->data);
	free(image);

	info.GetReturnValue().Set(obj);
}
NAN_METHOD(keyPressWithKeycode)
{

	if (info.Length() < 2)
	{
		return Nan::ThrowError(Nan::TypeError("too few arguments"));
	}

	display_keypress_with_keycode(info[0]->NumberValue(), info[1]->BooleanValue());
}

NAN_METHOD(keyPressWithKeysym)
{

	if (info.Length() < 2)
	{
		return Nan::ThrowError(Nan::TypeError("too few arguments"));
	}

	display_keypress_with_keysym(info[0]->NumberValue(), info[1]->BooleanValue());
}

NAN_METHOD(mouseMove)
{

	if (info.Length() < 2)
	{
		return Nan::ThrowError(Nan::TypeError("too few arguments"));
	}

	display_mouse_move(info[0]->NumberValue(), info[1]->NumberValue());
}

NAN_METHOD(mouseButton)
{

	if (info.Length() < 2)
	{
		return Nan::ThrowError(Nan::TypeError("too few arguments"));
	}
	int button = info[0]->NumberValue();
	if (button < 1 || button > 3)
	{

		return Nan::ThrowError(Nan::TypeError("Invalid button value"));
	}
	display_mouse_button(info[0]->NumberValue(), info[1]->BooleanValue());
}
NAN_METHOD(close)
{
	close();
}

NAN_METHOD(node_get_current_resolution)
{
	int width, height;
	get_current_screen_resolution(&width, &height);
	Local<Object> obj = Nan::New<Object>();
	Nan::Set(obj, Nan::New("width").ToLocalChecked(), Nan::New<Number>(width));
	Nan::Set(obj, Nan::New("height").ToLocalChecked(), Nan::New<Number>(height));
	info.GetReturnValue().Set(obj);
}

NAN_METHOD(node_get_screen_resolutions)
{
	XRRScreenSize *sizes;
	int length;
	get_screen_resolutions(&sizes, &length);
	Local<Array> array = Nan::New<Array>();

	for (int i = 0; i < length; i++)
	{
		Local<Object> obj = Nan::New<Object>();
		Nan::Set(obj, Nan::New("id").ToLocalChecked(), Nan::New<Number>(i));
		Nan::Set(obj, Nan::New("width").ToLocalChecked(), Nan::New<Number>(sizes[i].width));
		Nan::Set(obj, Nan::New("height").ToLocalChecked(), Nan::New<Number>(sizes[i].height));
		array->Set(i, obj);
	}

	info.GetReturnValue().Set(array);
}
NAN_METHOD(set_screen_resolution)
{

	if (info.Length() < 1)
	{
		return Nan::ThrowError(Nan::TypeError("too few arguments"));
	}
	int resolution_id = info[0]->NumberValue();
	set_current_screen_resolution(resolution_id);
}

NAN_METHOD(getMouseCursorSync)
{
	XFixesCursorImage *cursor = get_mouse_pointer();

	uint32_t bufferSize = cursor->width * cursor->height * 4 * 4;
	Local<Object> obj = Nan::New<Object>();
	Local<Object> buffer = Nan::NewBuffer((char *)cursor->pixels, bufferSize).ToLocalChecked();
	Nan::Set(obj, Nan::New("x").ToLocalChecked(), Nan::New<Number>(cursor->x));
	Nan::Set(obj, Nan::New("y").ToLocalChecked(), Nan::New<Number>(cursor->y));
	Nan::Set(obj, Nan::New("xhot").ToLocalChecked(), Nan::New<Number>(cursor->xhot));
	Nan::Set(obj, Nan::New("yhot").ToLocalChecked(), Nan::New<Number>(cursor->yhot));
	Nan::Set(obj, Nan::New("cursorSerial").ToLocalChecked(), Nan::New<Number>(cursor->cursor_serial));
	Nan::Set(obj, Nan::New("width").ToLocalChecked(), Nan::New<Number>(cursor->width));
	Nan::Set(obj, Nan::New("height").ToLocalChecked(), Nan::New<Number>(cursor->height));
	Nan::Set(obj, Nan::New("data").ToLocalChecked(), buffer);
	info.GetReturnValue().Set(obj);
}

NAN_MODULE_INIT(Init)
{
	Nan::Set(target, Nan::New("setResolution").ToLocalChecked(),
			 Nan::GetFunction(Nan::New<FunctionTemplate>(set_screen_resolution)).ToLocalChecked());
	Nan::Set(target, Nan::New("getCurrentResolution").ToLocalChecked(),
			 Nan::GetFunction(Nan::New<FunctionTemplate>(node_get_current_resolution)).ToLocalChecked());
	Nan::Set(target, Nan::New("getScreenResolutions").ToLocalChecked(),
			 Nan::GetFunction(Nan::New<FunctionTemplate>(node_get_screen_resolutions)).ToLocalChecked());
	Nan::Set(target, Nan::New("init").ToLocalChecked(),
			 Nan::GetFunction(Nan::New<FunctionTemplate>(init)).ToLocalChecked());
	Nan::Set(target, Nan::New("getImage").ToLocalChecked(),
			 Nan::GetFunction(Nan::New<FunctionTemplate>(getImage)).ToLocalChecked());
	Nan::Set(target, Nan::New("getImageSync").ToLocalChecked(),
			 Nan::GetFunction(Nan::New<FunctionTemplate>(getImageSync)).ToLocalChecked());
	Nan::Set(target, Nan::New("keyPressWithKeycode").ToLocalChecked(),
			 Nan::GetFunction(Nan::New<FunctionTemplate>(keyPressWithKeycode)).ToLocalChecked());
	Nan::Set(target, Nan::New("keyPressWithKeysym").ToLocalChecked(),
			 Nan::GetFunction(Nan::New<FunctionTemplate>(keyPressWithKeysym)).ToLocalChecked());
	Nan::Set(target, Nan::New("mouseMove").ToLocalChecked(),
			 Nan::GetFunction(Nan::New<FunctionTemplate>(mouseMove)).ToLocalChecked());
	Nan::Set(target, Nan::New("mouseButton").ToLocalChecked(),
			 Nan::GetFunction(Nan::New<FunctionTemplate>(mouseButton)).ToLocalChecked());
	Nan::Set(target, Nan::New("getMouseCursorSync").ToLocalChecked(),
			 Nan::GetFunction(Nan::New<FunctionTemplate>(getMouseCursorSync)).ToLocalChecked());
	Nan::Set(target, Nan::New("close").ToLocalChecked(),
			 Nan::GetFunction(Nan::New<FunctionTemplate>(close)).ToLocalChecked());
}

NODE_MODULE(node_x11, Init)
