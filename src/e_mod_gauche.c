/* START of the adaption. (main.c - interpreter main program) */

/*
 * main.c - interpreter main program
 *
 *   Copyright (c) 2000-2010  Shiro Kawai  <shiro@acm.org>
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Adapted by Takeshi Banse (takebi@laafc.net)
 *  - Borrowed both `sig_setup' and `load_gauche_init'.
 *  - The `report_error' is based on the `error_exit' (without `Scm_Exit').
 *  - The `e_mod_gauche_init' is based on the `main'.
 */

#include <gauche.h>
#include <signal.h>

static int initialized_p = 0;

/* signal handler setup.  let's catch as many signals as possible. */
static void sig_setup(void)
{
    sigset_t set;
    sigfillset(&set);
    sigdelset(&set, SIGABRT);
    sigdelset(&set, SIGILL);
#ifdef SIGKILL
    sigdelset(&set, SIGKILL);
#endif
#ifdef SIGCONT
    sigdelset(&set, SIGCONT);
#endif
#ifdef SIGSTOP
    sigdelset(&set, SIGSTOP);
#endif
    sigdelset(&set, SIGSEGV);
#ifdef SIGBUS
    sigdelset(&set, SIGBUS);
#endif /*SIGBUS*/
#if defined(GC_LINUX_THREADS)
    /* some signals are used in the system */
    sigdelset(&set, SIGPWR);  /* used in gc */
    sigdelset(&set, SIGXCPU); /* used in gc */
    sigdelset(&set, SIGUSR1); /* used in linux threads */
    sigdelset(&set, SIGUSR2); /* used in linux threads */
#endif /*GC_LINUX_THREADS*/
#if defined(GC_FREEBSD_THREADS)
    sigdelset(&set, SIGUSR1); /* used by GC to stop the world */
    sigdelset(&set, SIGUSR2); /* used by GC to restart the world */
#endif /*GC_FREEBSD_THREADS*/
    Scm_SetMasterSigmask(&set);
}

/* Load gauche-init.scm */
void load_gauche_init(void)
{
    ScmLoadPacket lpak;
    if (Scm_Load("gauche-init.scm", 0, &lpak) < 0) {
        Scm_Printf(SCM_CURERR, "gosh: WARNING: Error while loading initialization file: %A(%A).\n",
                   Scm_ConditionMessage(lpak.exception),
                   Scm_ConditionTypeName(lpak.exception));
    }
}

static void
report_error(ScmObj c)
{
   ScmObj m = Scm_ConditionMessage(c);
   if (SCM_FALSEP(m))
     Scm_Printf(SCM_CURERR, "egauche: Thrown unknown condition: %S\n", c);
   else
     Scm_Printf(SCM_CURERR, "egauche: %S: %A\n", Scm_ConditionTypeName(c), m);
}

int
e_mod_gauche_init(const char *scriptfile)
{
   int exit_code = 0;
   ScmObj av = SCM_NIL;

   ScmLoadPacket lpak;
   ScmObj mainproc;
   ScmEvalPacket epak;

   if (!initialized_p) {
	char dir[4096];

	GC_INIT();
	Scm_Init(GAUCHE_SIGNATURE);
	sig_setup();
	load_gauche_init();
	av = SCM_LIST1(SCM_MAKE_STR_IMMUTABLE("egauche"));

	SCM_DEFINE(Scm_UserModule(), "*program-name*", SCM_CDR(av));

	initialized_p = 1;
   }

   if (Scm_Load(scriptfile, 0, &lpak) < 0) {
	report_error(lpak.exception);
	return -1;
   }

   /* if symbol 'main is bound to a procedure in the user module,
      call it.  (SRFI-22) */
   mainproc = Scm_SymbolValue(Scm_UserModule(),
			      SCM_SYMBOL(SCM_INTERN("main")));
   if (SCM_PROCEDUREP(mainproc)) {
#if 0 /* Temporarily turned off due to the bug that loses stack traces. */
	int r = Scm_Apply(mainproc, SCM_LIST1(av), &epak);
	if (r > 0) {
	     ScmObj res = epak.results[0];
	     if (SCM_INTP(res)) exit_code = SCM_INT_VALUE(res);
	     else exit_code = 70;  /* EX_SOFTWARE, see SRFI-22. */
	} else {
	     Scm_ReportError(epak.exception);
	     exit_code = 70;  /* EX_SOFTWARE, see SRFI-22. */
	}
#else
	ScmObj r = Scm_ApplyRec(mainproc, SCM_LIST1(av));
	if (SCM_INTP(r)) {
	     exit_code = SCM_INT_VALUE(r);
	} else {
	     exit_code = 70;
	}
#endif
   }

   return exit_code;
}

/* END of the adaption. (main.c - interpreter main program) */

int
e_mod_gauche_shutdown()
{
   int exit_code = 0;
   ScmObj shutdownproc;

   shutdownproc = Scm_SymbolValue(Scm_UserModule(),
				  SCM_SYMBOL(SCM_INTERN("shutdown")));
   if (SCM_PROCEDUREP(shutdownproc)) {
	ScmObj r = Scm_ApplyRec(shutdownproc, SCM_NIL);
	if (SCM_INTP(r)) {
	     exit_code = SCM_INT_VALUE(r);
	} else {
	     exit_code = 70;
	}
   }

   Scm_Cleanup();

   return exit_code;
}

int
e_mod_gauche_eval(const char *form)
{
   ScmEvalPacket epak;

   if (Scm_EvalCString(form,
		       SCM_OBJ(Scm_UserModule()),
		       &epak) < 0) {
	report_error(epak.exception);
	return -1;
   }

   return 0;
}
