#include "ew.h"
#include "e_mod_gauche.h"

#include <stdio.h>

int main(int argc, char **argv)
{
   eina_init();
   ecore_init();

   e_mod_gauche_init("./test-main.scm");

   ecore_x_init(NULL);

   printf("start\n");

   ecore_main_loop_begin();
   return 0;
}
