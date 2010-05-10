#include <e.h>
#include "e_mod_main.h"
#include "e_mod_gauche.h"

EAPI E_Module_Api e_modapi =
{
   E_MODULE_API_VERSION, "EGauche"
};

EAPI void *
e_modapi_init(E_Module *m)
{
   if (e_mod_gauche_init(e_module_dir_get(m), "egauche.scm") != 0) return NULL;
   return m;
}

EAPI int
e_modapi_shutdown(E_Module *m)
{
   // TODO: Gauche is ALIVE!
   //Scm_Cleanup();
   return 1;
}

EAPI int
e_modapi_save(E_Module *m)
{
   return 1;
}
