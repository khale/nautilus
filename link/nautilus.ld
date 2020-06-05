ENTRY(nautilus_entry)

SECTIONS
{
    . = 0x100000;

    .boot :
    {
        *(.mbhdr)
        _loadStart = .;
        *(.boot)
        . = ALIGN(4096);
        pml4 = .;
        . += 0x1000;
        pdpt = .;
        . += 0x1000;
        pd = .;
        . += 0x1000; 

        boot_stack_start = .;
        . += 0x200000;  /* we start out with a 2M stack */
        boot_stack_end = .;
    }


    .text ALIGN(0x1000): 
    {
        *(.text*)
        *(.gnu.linkonce.t*)
    }

    .init ALIGN(0x1000): 
    {
        *(.init)
        *(.gnu.linkonce.init)
    }

    .fini ALIGN(0x1000):
    {
        *(.fini)
        *(.gnu.linkonce.fini)
    }

    .preinit_array ALIGN(0x1000):
    {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN(__preinit_array_end = .);
      }

    .init_array ALIGN(0x1000): 
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o) .ctors))
        PROVIDE_HIDDEN(__init_array_end = .);

    }

    .fini_array ALIGN(0x1000):
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
        PROVIDE_HIDDEN (__fini_array_end = .);
  }

    .gcc_except_table ALIGN(0x1000):
    {
        *(.gcc_except_table*)
        *(.gnu.linkonce.gcc_except*)
    }

    .ctors  ALIGN(0x1000):
    {
        /* gcc uses crtbegin.o to find the start of
        the constructors, so we make sure it is
        first.  Because this is a wildcard, it
        doesn't matter if the user does not
        actually link against crtbegin.o; the
        linker won't look for a file to match a
        wildcard.  The wildcard also means that it
        doesn't matter which directory crtbegin.o
        is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
    the crtend.o file until after the sorted ctors.
    The .ctor section from the crtend file contains the
    end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  }
  .dtors ALIGN(0x1000):
  {
      KEEP (*crtbegin.o(.dtors))
      KEEP (*crtbegin?.o(.dtors))
      KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
      KEEP (*(SORT(.dtors.*)))
      KEEP (*(.dtors))
  }

    _data_start = . ;    

    .data ALIGN(0x1000) : 
    {
        *(.data*)
        *(.gnu.linkonce.d*)
    }

    .tests ALIGN(0x1000) : 
    {
        __start_tests = .;
        *(.tests*);
        __stop_tests = .;
    }

    .cmdline_flags ALIGN(0x1000) : 
    {
        __start_flags = .;
        *(.cmdline_flags*);
        __stop_flags = .;
    }
    
    .rodata ALIGN(0x1000) : 
    {
        *(.rodata*)
        *(.gnu.linkonce.r*)
    }

    .got ALIGN(0x1000) : 
    {
        *(.got*)
        *(.gnu.linkconce.got*)
    }

    .shell_cmds ALIGN(0x1000) : 
    {
        __start_shell_cmds = .;
        *(.shell_cmds*);
        __stop_shell_cmds = .;
    }

    .aspace_impls ALIGN(0x1000) :
    {
        __start_aspace_impls = .;
        *(.aspace_impls*);
        __stop_aspace_impls = .;
    }

    _loadEnd = .; 
    
    .bss ALIGN(0x1000) : 
    {
        *(COMMON)
        *(.bss*)
        *(.gnu.linkonce.b*)
    }
    
    _bssEnd = .; 

    _data_end = .;
    
    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
    }
}

