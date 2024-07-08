#if !defined __EOAVOV_ARRAY_ASSOC_
#define __EOAVOV_ARRAY_ASSOC_
#include "cube.h"
enum
{
    DYNPROP_IVAR = 0,
    DYNPROP_FVAR = 1,
    DYNPROP_SVAR = 2,
    DYNPROP_VVAR = 3,
    DYNPROP_PTR = 4
};
struct arrayAssoc
{
  class dynamicProperty
  {
    private:
    union{
        int *iv;
        float *fv;
        char *sv;
        vec *vv;
        void *ptr;
    }value;
    uchar type;
    void clearData();
    public:
    string name;
    dynamicProperty(int t, void *val);
    dynamicProperty():dynamicProperty(DYNPROP_PTR,NULL){}
    ~dynamicProperty();
    int intValue();
    float floatValue();
    void *ptrValue();
    vec vecValue();
    const char * strValue();
    void setValue(int t,void *val);
    void setValue(int val);
    void setValue(const char *str);
    void setValue(void *ptr);
    void setValue(float val);
  };
  hashnameset<dynamicProperty>fields;
  arrayAssoc(){}
  ~arrayAssoc(){fields.clear();}
  dynamicProperty &operator [](const char *name)
  {
   dynamicProperty *field = fields.access(name);
   if(!field)
   {
    field = &fields[name];
    copystring(field->name,name);
   }
   return *field;  
  }
};

#endif