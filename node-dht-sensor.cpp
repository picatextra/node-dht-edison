#include <node.h>
#include <unistd.h>
#include <nan.h>

using namespace v8;

bool readDHT22(int pin, double *h, double *t);

void Read(const Nan::FunctionCallbackInfo<Value>& args) {

    if (args.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
    	return;
    }
    
    int pin = args[0]->Uint32Value();
    double temperature = 0, humidity = 0;    
    bool result = readDHT22(pin, &temperature, &humidity);
       
    Local<Object> readout = Nan::New<Object>();
    readout->Set(Nan::New("humidity").ToLocalChecked(), Nan::New<Number>(humidity));
    readout->Set(Nan::New("temperature").ToLocalChecked(), Nan::New<Number>(temperature));
    readout->Set(Nan::New("isValid").ToLocalChecked(), Nan::New<Boolean>(result));
    args.GetReturnValue().Set(readout);
}


void Init(Handle<Object> exports) {
	Nan::SetMethod(exports, "read", Read);
}

NODE_MODULE(node_dht_edison, Init);
