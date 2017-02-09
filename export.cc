#include <nan.h>
#include <iostream>
#include <cstdlib>
#include "export.h"
#include "xdisplay.h"

using namespace v8;

using v8::FunctionTemplate;

char * pDisplay = std::getenv("DISPLAY");
Image *image = ( Image* )malloc(sizeof(Image));
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
	image->data = (char*) malloc(sizeof(char)*width*height*depth);

}

NAN_METHOD(getImage)
{

	display_image(image);
	uint32_t bufferSize = image->width * image->height * image->depth;
	Local<Object> buffer = Nan::NewBuffer((char*) image->data, bufferSize, NULL, NULL).ToLocalChecked();

	Local<Object> obj = Nan::New<Object>();
	Nan::Set(obj, Nan::New("width").ToLocalChecked(), Nan::New<Number>(image->width));
	Nan::Set(obj, Nan::New("height").ToLocalChecked(), Nan::New<Number>(image->height));
	Nan::Set(obj, Nan::New("depth").ToLocalChecked(), Nan::New<Number>(image->depth));
	Nan::Set(obj, Nan::New("data").ToLocalChecked(), buffer);

	info.GetReturnValue().Set(obj);

}

NAN_METHOD(keyPress)
{


	if(info.Length() < 2)
	{
		return Nan::ThrowError(Nan::TypeError("too few arguments"));
	}


	display_keypress(info[0]->NumberValue(),info[1]->BooleanValue());

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
	Nan::Set(target, Nan::New("keyPress").ToLocalChecked(),
			Nan::GetFunction(Nan::New<FunctionTemplate>(keyPress)).ToLocalChecked());
	Nan::Set(target, Nan::New("mouseMove").ToLocalChecked(),
			Nan::GetFunction(Nan::New<FunctionTemplate>(mouseMove)).ToLocalChecked());
	Nan::Set(target, Nan::New("mouseButton").ToLocalChecked(),
			Nan::GetFunction(Nan::New<FunctionTemplate>(mouseButton)).ToLocalChecked());

}

NODE_MODULE(node_x11, Init)

