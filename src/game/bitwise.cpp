#include "cube.h"
const char* bwoperations[] = 
 {
  ">>","<<","~","&","|","^"
 };
void bitwise(unsigned int a,const char*op,unsigned int b)
{
 if(!op)return;
 for(int i=0;i<6;i++)
  {
   if(!strcmp(bwoperations[i],op))
    {
     switch(i)
      {
      case(0):intret(a>>b);break;
      case(1):intret(a<<b);break;
      case(2):intret(~a);break;
      case(3):intret(a&b);break;
      case(4):intret(a|b);break;
      case(5):intret(a^b);break;
      }
     return;
    }
  }
}
ICOMMAND(bitwise,"isi",(unsigned int *a,const char*op,unsigned int *b),bitwise(*a,op,*b));
