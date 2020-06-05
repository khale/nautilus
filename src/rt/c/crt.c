#include <nautilus/nautilus.h>
void _init(void) __attribute__((section (".init")));
void _fini(void) __attribute__((section (".fini")));

void 
_init (void)
{
    nk_vc_printf("_init\n");
}

void 
_fini (void)
{
    nk_vc_printf("_fini\n");
}
