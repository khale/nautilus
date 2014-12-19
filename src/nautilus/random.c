#include <nautilus/nautilus.h>
#include <nautilus/random.h>
#include <nautilus/cpu.h>
#include <nautilus/percpu.h>
#include <nautilus/smp.h>
#include <nautilus/irq.h>
#include <nautilus/spinlock.h>
#include <nautilus/random.h>
#include <dev/apic.h>
#include <lib/liballoc.h>


void
nk_rand_seed (uint64_t seed) {
    struct nk_rand_info * rand = per_cpu_get(rand);
    uint8_t flags = spin_lock_irq_save(&rand->lock);
    rand->xi   = seed;
    rand->seed = seed;
    rand->n    = 0;
    spin_unlock_irq_restore(&rand->lock, flags);
}


/* 
 * side effect: increments counter
 * NOTE: this function assumes that the random lock
 * is held when coming in
 */
void
nk_rand_set_xi (uint64_t xi) 
{
    struct nk_rand_info * rand = per_cpu_get(rand);
    rand->xi = xi;
    rand->n++;
}


#define _AB(x,n) ((char)(((x) >> (n)) & 0xff))
/* 
 * To get something appearing random, 
 * we fold the APIC IRRs together and 
 * then combine them with the cycle count
 *
 * this is likely pretty bogus, but 
 * we don't need any kind of security for now,
 * just something that appears random
 */
static char
get_rand_byte (void) 
{
    struct apic_dev * apic = per_cpu_get(apic);
    struct nk_rand_info * rand = per_cpu_get(rand);
    uint64_t cycles;
    uint32_t val;
    char b =  0xff;
    char b2 = 0;
    uint8_t i;


    for (i = 0; i < 8; i++) {
        val = apic_read(apic, APIC_GET_IRR(i)) ^ (rand->seed & 0xffffffff);
        b ^= ~(_AB(val, 0) ^
               _AB(val, 1) ^
               _AB(val, 2) ^
               _AB(val, 3));
    }

    cycles = rdtsc();

    for (i = 0; i < 8; i++) {
        b2 ^= _AB(cycles, i);
    }

    return b + b2;
}


void
nk_get_rand_bytes (uint8_t * buf, unsigned len)
{
    if (!buf) {
        return;
    }

    while (len--) {
        *buf++ = get_rand_byte();
    }
}


int
nk_rand_init (struct cpu * cpu) 
{
    cpu->rand = malloc(sizeof(struct nk_rand_info));
    if (!cpu->rand) {
        ERROR_PRINT("Could not allocate CPU random info\n");
        return -1;
    }
    memset(cpu->rand, 0, sizeof(struct nk_rand_info));

    spinlock_init(&cpu->rand->lock);

    return 0;
}
