#include "cube.h"
const char bwoperations[] = 
 {
  '>','<','~','&','|','^'
 };
void bitwise(unsigned int a,const char*op,unsigned int b)
{
 if(!op||op[0]=='\0')return;
 for(int i=0;i<6;i++)
  {
   if(bwoperations[i]==op[0])
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
