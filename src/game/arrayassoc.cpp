#include "arrayassoc.h"
#include <string.h>
#include "eoavov.h"
template<typename T>T* initField(T *v)
{
    void *result = malloc(sizeof(T));
    *(T*)(result) = *v;
    return (T*)result;
}
arrayAssoc::dynamicProperty::dynamicProperty(int t,void *val)
{
    setValue(t,val);
}
void arrayAssoc::dynamicProperty::setValue(int t,void *val)
{
    type = t;
    switch(t)
    {
        case(DYNPROP_IVAR):
        {
            value.iv = initField<int>(static_cast<int*>(val));
            break;
        }
        case(DYNPROP_FVAR):
        {
            value.fv = initField<float>(static_cast<float*>(val));
            break;
        }
        case(DYNPROP_SVAR):
        {
            value.sv = newstring((const char*)val);
            break;
        }
        case(DYNPROP_VVAR):
        {
            vec &d = *(vec*)(val);
            value.vv = new vec(d);
            break;
        }
        case(DYNPROP_PTR):
        {
            value.ptr = val;
            break;
        }
    }
}
void arrayAssoc::dynamicProperty::setValue(float value)
{
    clearData();
    setValue(DYNPROP_FVAR,(void*)&value);
}
void arrayAssoc::dynamicProperty::setValue(int value)
{
    clearData();
    setValue(DYNPROP_IVAR,(void*)&value);
}
void arrayAssoc::dynamicProperty::setValue(const char *str)
{
    clearData();
    setValue(DYNPROP_SVAR,(void*)&value);
}
int arrayAssoc::dynamicProperty::intValue()
{
    assert(type==DYNPROP_IVAR&&"arrayAssoc::dynamicProperty::intValue called on non-int type");
    return *(value.iv);
}
float arrayAssoc::dynamicProperty::floatValue()
{
    assert(type==DYNPROP_FVAR&&"arrayAssoc::dynamicProperty::floatValue called on non-float type");
    return *(value.fv);
}
vec arrayAssoc::dynamicProperty::vecValue()
{
    assert(type==DYNPROP_VVAR&&"arrayAssoc::dynamicProperty::vecValue called on non-vec type");
    return *(vec*)(value.vv);
}
const char* arrayAssoc::dynamicProperty::strValue()
{
    assert(type==DYNPROP_SVAR&&"arrayAssoc::dynamicProperty::strValue called on non-vec type");
    return (const char*)value.ptr;
}
void * arrayAssoc::dynamicProperty::ptrValue()
{
    return value.ptr;
}
void arrayAssoc::dynamicProperty::clearData()
{
    switch(type)
    {
        case(DYNPROP_IVAR):
        {
            free(value.iv);
        }
        break;
        case(DYNPROP_FVAR):
        {
            free(value.fv);
        }
        break;
        case(DYNPROP_SVAR):
        {
            delete[] value.sv;
        }
        break;
        case(DYNPROP_VVAR):
        {
            delete value.vv;
        }
        break;
        case(DYNPROP_PTR):
        {
            delete value.ptr;
        }
        break;
    }
}
arrayAssoc::dynamicProperty::~dynamicProperty()
{
    clearData();
}