#include <nan.h>
#include <iostream>
#include <cstdlib>
#include "export.h"
#include "xdisplay.h"

using namespace v8;

using v8::FunctionTemplate;

char * pDisplay = std::getenv("DISPLAY");
int width = 0;
int height = 0;
int depth = 0;

NAN_METHOD(init)
{


	if(info.Length() == 1 && info[0]->IsString()) {
		Nan::Utf8String dstr(info[0]);
		pDisplay  = *dstr;
	}

	std::cout << "init on display " << pDisplay <<std::endl;
	display_init(pDisplay, &width, &height, &depth);

}

NAN_METHOD(getImage)
{

	Image *image = ( Image* )malloc(sizeof(Image));
	image->data = (char*) malloc(sizeof(char)*width*height*4);
	display_image(image);
	uint32_t bufferSize = image->width * image->height * (image->bits_per_pixel / 8);
	Local<Object> buffer = Nan::NewBuffer((char*) image->data, bufferSize).ToLocalChecked();

	Local<Object> obj = Nan::New<Object>();
	Nan::Set(obj, Nan::New("width").ToLocalChecked(), Nan::New<Number>(image->width));
	Nan::Set(obj, Nan::New("height").ToLocalChecked(), Nan::New<Number>(image->height));
	Nan::Set(obj, Nan::New("depth").ToLocalChecked(), Nan::New<Number>(image->depth));
	Nan::Set(obj, Nan::New("bits_per_pixel").ToLocalChecked(), Nan::New<Number>(image->bits_per_pixel));
	Nan::Set(obj, Nan::New("bytes_per_line").ToLocalChecked(), Nan::New<Number>(image->bytes_per_line));
	Nan::Set(obj, Nan::New("data").ToLocalChecked(), buffer);

	info.GetReturnValue().Set(obj);

}
NAN_METHOD(keyPressWithKeycode)
{


	if(info.Length() < 2)
	{
		return Nan::ThrowError(Nan::TypeError("too few arguments"));
	}


	display_keypress_with_keycode(info[0]->NumberValue(),info[1]->BooleanValue());

}

NAN_METHOD(keyPressWithKeysym)
{


	if(info.Length() < 2)
	{
		return Nan::ThrowError(Nan::TypeError("too few arguments"));
	}


	display_keypress_with_keysym(info[0]->NumberValue(),info[1]->BooleanValue());

}

NAN_METHOD(mouseMove)
{

	if(info.Length() <2 )
	{
		return Nan::ThrowError(Nan::TypeError("too few arguments"));
	}

	display_mouse_move(info[0]->NumberValue(), info[1]->NumberValue());
}

NAN_METHOD(mouseButton)
{

	if(info.Length() <2 )
	{
		return Nan::ThrowError(Nan::TypeError("too few arguments"));
	}
	int button = info[0]->NumberValue();
	if( button <1 || button > 3 )
	{

		return Nan::ThrowError(Nan::TypeError("Invalid button value"));
	}
	display_mouse_button( info[0]->NumberValue(), info[1]->BooleanValue());

}

NAN_MODULE_INIT(Init) 
{
	Nan::Set(target, Nan::New("init").ToLocalChecked(),
			Nan::GetFunction(Nan::New<FunctionTemplate>(init)).ToLocalChecked());
	Nan::Set(target, Nan::New("getImage").ToLocalChecked(),
			Nan::GetFunction(Nan::New<FunctionTemplate>(getImage)).ToLocalChecked());	
	Nan::Set(target, Nan::New("keyPressWithKeycode").ToLocalChecked(),
			Nan::GetFunction(Nan::New<FunctionTemplate>(keyPressWithKeycode)).ToLocalChecked());
	Nan::Set(target, Nan::New("keyPressWithKeysym").ToLocalChecked(),
			Nan::GetFunction(Nan::New<FunctionTemplate>(keyPressWithKeysym)).ToLocalChecked());
	Nan::Set(target, Nan::New("mouseMove").ToLocalChecked(),
			Nan::GetFunction(Nan::New<FunctionTemplate>(mouseMove)).ToLocalChecked());
	Nan::Set(target, Nan::New("mouseButton").ToLocalChecked(),
			Nan::GetFunction(Nan::New<FunctionTemplate>(mouseButton)).ToLocalChecked());

}

NODE_MODULE(node_x11, Init)

