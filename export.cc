#include <nan.h>
#include <iostream>
#include <cstdlib>
#include "xdisplay.h"

using namespace v8;

using v8::FunctionTemplate;

char * pDisplay = std::getenv("DISPLAY");
int width = 0;
int height = 0;
int depth = 0;

class GetImageWorker : public Nan::AsyncWorker {
	public:
		bool withPointer;
		Image *image;

		GetImageWorker(bool withpointer, Nan::Callback* callback) 
		: Nan::AsyncWorker(callback) {
			this->withPointer = withpointer;
		}
		
		void Execute() {
			image = ( Image* )malloc(sizeof(Image));
			image->data = (char*) malloc(sizeof(char)*width*height*4);
			display_image(image, withPointer);

		}
		
		void HandleOKCallback() {
			Nan::HandleScope scope;
			uint32_t bufferSize = image->width * image->height * (image->bits_per_pixel / 8);
			Local<Object> buffer = Nan::NewBuffer((char*) image->data, bufferSize).ToLocalChecked();
			Local<Object> obj = Nan::New<Object>();
			Nan::Set(obj, Nan::New("width").ToLocalChecked(), Nan::New<Number>(image->width));
			Nan::Set(obj, Nan::New("height").ToLocalChecked(), Nan::New<Number>(image->height));
			Nan::Set(obj, Nan::New("depth").ToLocalChecked(), Nan::New<Number>(image->depth));
			Nan::Set(obj, Nan::New("bits_per_pixel").ToLocalChecked(), Nan::New<Number>(image->bits_per_pixel));
			Nan::Set(obj, Nan::New("bytes_per_line").ToLocalChecked(), Nan::New<Number>(image->bytes_per_line));
			Nan::Set(obj, Nan::New("data").ToLocalChecked(), buffer);

			v8::Local<v8::Value> argv[] = {
				Nan::Null(), // no error occured
				obj 

			};
			callback->Call(2, argv);

		}

		void HandleErrorCallback() {
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


	if(info.Length() == 1 && info[0]->IsString()) {
		Nan::Utf8String dstr(info[0]);
		pDisplay  = *dstr;
	}

	std::cout << "init on display " << pDisplay <<std::endl;
	display_init(pDisplay, &width, &height, &depth);

}

NAN_METHOD(getImage) 
{
	bool withPointer = false;
	if(info.Length() == 1 && info[0]->IsBoolean())
	{
		withPointer = info[0]->BooleanValue();
	}

	Nan::AsyncQueueWorker(new GetImageWorker(
				withPointer,
				new Nan::Callback(info[1].As<v8::Function>()
					)));
}


NAN_METHOD(getImageSync)
{

	bool withPointer = false;
	if(info.Length() == 1 && info[0]->IsBoolean())
	{
		withPointer = info[0]->BooleanValue();
	}
	Image *image = ( Image* )malloc(sizeof(Image));
	image->data = (char*) malloc(sizeof(char)*width*height*4);
	display_image(image, withPointer);
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
NAN_METHOD(close)
{
	close();
}

NAN_MODULE_INIT(Init) 
{
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
	Nan::Set(target, Nan::New("close").ToLocalChecked(),
			Nan::GetFunction(Nan::New<FunctionTemplate>(close)).ToLocalChecked());



}

NODE_MODULE(node_x11, Init)

