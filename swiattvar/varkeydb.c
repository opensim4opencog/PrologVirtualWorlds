
#define STRING char *
														 
/*#define O_DEBUG 1*/
#include "/opt/pl/src/config.h"
#include "/opt/pl/src/parms.h"
#include "/opt/pl/src/pl-buffer.h"
#include "/opt/pl/src/pl-ctype.h"
#include "/opt/pl/src/pl-incl.h"
#include "/opt/pl/src/pl-itf.h"
//#include "/opt/pl/src/pl-main.h"
//#include "/opt/pl/src/pl-os.h"
#include "/opt/pl/src/pl-data.h"
#include "/opt/pl/include/SWI-Prolog.h"



#undef ulong
#define ulong unsigned long

							
foreign_t
pl_var_ref(term_t term, term_t hval)
{ 
	
  Word p = valTermRef(term);
  //long hraw;

  deRef(p);

    return PL_unify_integer(hval,  (long) valTermRef(term));
	//PL_succeed;
}

static install_t 
install()
	{
	PL_register_foreign("var_ref", 2, pl_var_ref, 0);
	}


int main(argc, argv) 
{
	install();
	return 0;
}


