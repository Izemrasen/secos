/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>

extern info_t *info;

void tp()
{
	gdt_reg_t * gdt = NULL;
	get_gdtr(*gdt);
	
	unsigned long * p = (unsigned long*) gdt -> addr;
	gdt_reg_t * ptr = (gdt_reg_t *)p; 
	/*debug("gdt limit_1 : %llx\n", p->desc->limit_1);
	debug("gdt limit_2 : %llx\n", p->desc->limit_2);
	debug("gdt base_1 : %1x\n", p->desc->base_1);
	debug("gdt base_2 : %01x\n", p->desc->base_2);
	debug("gdt base_3 : %01x\n", p->desc->base_3);
	debug("gdt segment type : %01x\n", p->desc->type);
	debug("gdt descriptor type : %01x\n", p->desc->s);		
	debug("gdt descriptor privilege level : %01x\n", p->desc->dpl);
	debug("gdt segment present flag : %01x\n", p->desc->p);
	debug("gdt longmode : %01x\n", p->desc->l);
	debug("gdt default length : %01x\n", p->desc->d);
	debug("gdt granularity : %01x\n", p->desc->g);*/
	
	unsigned int i;
	for(i=0; i > (size_t)gdt->limit+1/8; i++){
		debug("Descriptor %d", i);
		debug("gdt limit : %llx%llx\n", ptr->desc->limit_1, ptr->desc->limit_2);
	}	
	
}
