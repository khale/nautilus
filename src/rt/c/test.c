//#include <stdlib.h>
//#include <stdio.h>
//#include <unistd.h>
#include <nautilus/nautilus.h>


int main (int argc, char ** argv, char ** envp)
{
    int i, j;

    /*
    for (i = 0; i < argc; i++) {
        printf("argv[%d]=%s\n", i, argv[i]);
    }
    */
    nk_vc_printf("Hello World, argc=%d, argv[0]=%s\n", argc, argv[0]);

    return 0;
}
