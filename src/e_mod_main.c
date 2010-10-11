#include <e.h>
#include "e_mod_main.h"
#include "e_mod_gauche.h"

static void _e_mod_action_cb(E_Object *obj, const char *params);

static E_Action *act = NULL;

EAPI E_Module_Api e_modapi =
{
   E_MODULE_API_VERSION, "EGauche"
};

EAPI void *
e_modapi_init(E_Module *m)
{
   act = e_action_add("egauche");
   if (act)
     {
	act->func.go = _e_mod_action_cb;
	e_action_predef_name_set("EGauche", "Evaluate A Single Scheme Form",
				 "egauche", NULL, NULL, 1);
     }

   e_module_delayed_set(m, 0);
   e_module_priority_set(m, -1000);

   if (e_mod_gauche_init("egauche.scm") < 0)
     {
	printf("***e_mod_gauche_init failure.");
	return NULL;
     }

   return m;
}

EAPI int
e_modapi_shutdown(E_Module *m)
{
   if (e_mod_gauche_shutdown() < 0)
     {
	printf("***e_mod_gauche_shutdown failure.");
     }

   if (act)
     {
	e_action_predef_name_del("EGauche", "Evaluate A Single Scheme Form");
	e_action_del("egauche");
     }

   return 1;
}

EAPI int
e_modapi_save(E_Module *m)
{
   return 1;
}

static void
_e_mod_action_cb(E_Object *obj, const char *params)
{
   if (e_mod_gauche_eval(params) < 0)
     {
	printf("***evaluation failure %s", params);
     }
}
