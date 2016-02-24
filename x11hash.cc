#include <nan.h>

extern "C" {
    #include "xcoin.h"
}

NAN_METHOD(digest) {
    if (info.Length() != 1){
        Nan::ThrowError("You must provide exactly one argument.");
        return;
    }

    v8::Local<v8::Object> target = info[0]->ToObject();

    if(!node::Buffer::HasInstance(target)) {
        Nan::ThrowError("Argument should be a buffer object.");
        return;
    }

    char * input = node::Buffer::Data(target);
    char * output = new char[32];

    xcoin_hash(input, output);

    Nan::MaybeLocal<v8::Object> buffer = Nan::CopyBuffer(output, 32);

    delete []output;
    info.GetReturnValue().Set(buffer.ToLocalChecked());
}

NAN_MODULE_INIT(init) {
    NAN_EXPORT(target, digest);
}

NODE_MODULE(x11hash, init)
