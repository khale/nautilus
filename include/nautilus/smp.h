#ifndef __SMP_H__
#define __SMP_H__

#ifndef __ASSEMBLER__ 

#ifdef __cplusplus
extern "C" {
#endif


/******* EXTERNAL INTERFACE *********/

uint32_t nk_get_num_cpus (void);

/******* !EXTERNAL INTERFACE! *********/



#include <dev/apic.h>
#include <nautilus/spinlock.h>
#include <nautilus/queue.h>

struct naut_info;
struct nk_topo_params;
struct nk_cpu_coords;

typedef struct nk_queue nk_thread_queue_t;
typedef struct nk_thread nk_thread_t;
typedef void (*nk_xcall_func_t)(void * arg);
typedef uint32_t cpu_id_t;


struct nk_xcall {
    nk_queue_entry_t xcall_node;
    void * data;
    nk_xcall_func_t fun;
    uint8_t xcall_done;
    uint8_t has_waiter;
};


struct cpu {
    nk_thread_t * cur_thread; /* KCH: this must be first! */

    cpu_id_t id;
    uint8_t lapic_id;
    uint8_t enabled;
    uint8_t is_bsp;
    uint32_t cpu_sig;
    uint32_t feat_flags;

    volatile uint8_t booted;

    struct apic_dev * apic;

    struct sys_info * system;

    spinlock_t lock;

    nk_thread_queue_t * run_q;

    nk_queue_t * xcall_q;
    struct nk_xcall xcall_nowait_info;

    ulong_t cpu_khz; 
    
    /* NUMA info */
    struct nk_topo_params * tp;
    struct nk_cpu_coords * coord;

    struct nk_rand_info * rand;
};


struct ap_init_area {
    uint32_t stack;  // 0
    uint32_t rsvd; // to align the GDT on 8-byte boundary // 4
    uint32_t gdt[6] ; // 8
    uint16_t gdt_limit; // 32
    uint32_t gdt_base; // 34
    uint16_t rsvd1; // 38
    uint64_t gdt64[3]; // 40
    uint16_t gdt64_limit; // 64
    uint64_t gdt64_base; // 66
    uint64_t cr3; // 74
    struct cpu * cpu_ptr; // 82

    void (*entry)(struct cpu * core); // 90

} __packed;


int smp_early_init(struct naut_info * naut);
int smp_bringup_aps(struct naut_info * naut);
int smp_xcall(cpu_id_t cpu_id, nk_xcall_func_t fun, void * arg, uint8_t wait);
void smp_ap_entry (struct cpu * core);
int smp_setup_xcall_bsp (struct cpu * core);



#ifdef __cplusplus
}
#endif

#endif /* !__ASSEMBLER__ */

#define AP_TRAMPOLINE_ADDR 0xf000 
#define AP_BOOT_STACK_ADDR 0x1000
#define AP_INFO_AREA       0x2000

#define BASE_MEM_LAST_KILO 0x9fc00
#define BIOS_ROM_BASE      0xf0000
#define BIOS_ROM_END       0xfffff


#endif
