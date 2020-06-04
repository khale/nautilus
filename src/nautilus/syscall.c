#include <nautilus/nautilus.h>
#include <nautilus/syscall.h>


void sys_exit(int arg)
{
    nk_vc_printf("Got to exit!\n");
}
