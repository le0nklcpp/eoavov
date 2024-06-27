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
void arrayAssoc::dynamicProperty::clearData()
{
    switch(type)
    {
        case(DYNPROP_IVAR):
        {
            delete value.iv;
        }
        break;
        case(DYNPROP_FVAR):
        {
            delete value.fv;
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
template<typename T>T arrayAssoc::dynamicProperty::getValue()
{
    switch(type)
    {
        case(DYNPROP_IVAR):
        {
            return *static_cast<T*>(value.iv);
        }
        break;
        case(DYNPROP_FVAR):
        {
            return *static_cast<T*>(value.fv);
        }
        break;
        case(DYNPROP_SVAR):
        {
            return static_cast<T>(value.sv);
        }
        break;
        case(DYNPROP_VVAR):
        {
            return static_cast<T>(value.vv);
        }
        break;
        case(DYNPROP_PTR):
        {
            return static_cast<T>(value.ptr);
        }
        break;
    }
    return 0;
}
arrayAssoc::dynamicProperty::~dynamicProperty()
{
    clearData();
}