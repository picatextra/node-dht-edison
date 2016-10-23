#include <node.h>
#include <unistd.h>
#include <nan.h>

using namespace v8;

extern "C" {
    bool readDHT22(int pin, double *h, double *t);
}

void Read(const Nan::FunctionCallbackInfo<Value>& args) {

    if (args.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
    	return;
    }
    
    int pin = args[0]->Uint32Value();
    double t = 0, h = 0;    
    bool result = readDHT22(pin, &h, &t);
       
    Local<Object> readout = Nan::New<Object>();
    readout->Set(Nan::New("h").ToLocalChecked(), Nan::New<Number>(h));
    readout->Set(Nan::New("t").ToLocalChecked(), Nan::New<Number>(t));
    readout->Set(Nan::New("valid").ToLocalChecked(), Nan::New<Boolean>(result));
    args.GetReturnValue().Set(readout);
}


void Init(Handle<Object> exports) {
	Nan::SetMethod(exports, "read", Read);
}

NODE_MODULE(node_dht_edison, Init);
