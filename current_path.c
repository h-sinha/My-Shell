#include "headers.h"

char* get_current_path() 
{
   static char current[100];
   if (getcwd(current, sizeof(current)) != NULL) 
   {
   		return current;
   } 
   else 
   {
       perror("getcwd() error");
   }
}