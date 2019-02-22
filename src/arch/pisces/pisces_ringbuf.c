/* Console Driver when Kitten is running as a multi-instance guest on pisces
 * Jiannan Ouyang (ouyang@cs.pitt.edu)
 */

#include <nautilus/nautilus.h>
#include <nautilus/naut_types.h>
#include "pisces_boot_params.h"
#include "pisces_lock.h"

extern struct pisces_boot_params * pisces_boot_params;

// Embedded ringbuffer that maps into a 64KB chunk of memory
struct pisces_cons_ringbuf {
	struct pisces_spinlock lock;
    	uint64_t read_idx;
    	uint64_t write_idx;
    	uint64_t cur_len;
    	uint8_t buf[(64 * 1024) - 32];
} __attribute__((packed));


static struct pisces_cons_ringbuf * console_buffer = NULL;


/** Set when the console has been initialized. */
static int initialized = 0;


/**
 * Prints a single character to the pisces console buffer.
 */
static void pisces_cons_putc(unsigned char c)
{
	pisces_spin_lock(&(console_buffer->lock));

	// If the buffer is full, then we are just going to start overwriting the log
	console_buffer->buf[console_buffer->write_idx] = c;

	console_buffer->cur_len++;
	console_buffer->write_idx++;
	console_buffer->write_idx %= sizeof(console_buffer->buf);
	
	if (console_buffer->cur_len > sizeof(console_buffer->buf)) {
		// We are overwriting, update the read state to be sane
		console_buffer->read_idx++;
		console_buffer->read_idx %= sizeof(console_buffer->buf);
		console_buffer->cur_len--;
	}

	pisces_spin_unlock(&(console_buffer->lock));
}


/**
 * Reads a single character from the pisces console port.
 */
/*
static char pisces_cons_getc(struct console *con)
{
    u64 *cons, *prod;
    char c;

    cons = &console_buffer->in_cons;
    prod = &console_buffer->in_prod;

    pisces_spin_lock(&console_buffer->lock_out);
    c = console_buffer->in[*cons];
    *cons = (*cons + 1) % PISCES_CONSOLE_SIZE_IN; 
    pisces_spin_unlock(&console_buffer->lock_out);


    return c;
}
*/

/**
 * Writes a string to the pisces console buffer.
 */
void pisces_cons_write (const char *str);
void pisces_cons_write (const char *str)
{	
	unsigned char c;
    str = (const char*)0x434b6f;
    int cnt = 0;

    *(uint64_t*)((uint64_t) pisces_boot_params->init_dbg_buf + 8) = (uint64_t)str;

	while ((c = *str++) != '\0') {
		pisces_cons_putc(c);
        pisces_boot_params->init_dbg_buf[cnt] = c;
        cnt = (cnt + 1) % 8;
	}

    pisces_boot_params->init_dbg_buf[0] = *str;

	
}



/**
 * Initializes and registers the pisces console driver.
 */
int pisces_console_init(void);
int 
pisces_console_init(void) 
{
    if (initialized) {
        printk("Pisces console already initialized.\n");
        return -1;
    }

    console_buffer = (struct pisces_cons_ringbuf*)pisces_boot_params->console_ring_addr; 

    initialized = 1;

    return 0;
}

