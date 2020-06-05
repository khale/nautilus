#include <nautilus/nautilus.h>
#include <nautilus/syscall.h>
#include <nautilus/shutdown.h>

void sys_exit(int arg)
{
    qemu_shutdown_with_code(arg);
}
