/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>

extern info_t *info;

void tp()
{
	gdt_reg_t * gdt;
	get_gdtr(gdt);
	debug("gdt limit_1 : %lx\n", gdt->desc->limit_1);
	debug("gdt limit_2 : %lx\n", gdt->desc->limit_2);
	debug("gdt base_1 : %1x\n", gdt->desc->base_1);
	debug("gdt base_2 : %01x\n", gdt->desc->base_2);
	debug("gdt base_3 : %01x\n", gdt->desc->base_3);
	debug("gdt segment type : %01x\n", gdt->desc->type);
	debug("gdt descriptor type : %01x\n", gdt->desc->s);		
	debug("gdt descriptor privilege level : %01x\n", gdt->desc->dpl);
	debug("gdt segment present flag : %01x\n", gdt->desc->p);
	debug("gdt longmode : %01x\n", gdt->desc->l);
	debug("gdt default length : %01x\n", gdt->desc->d);
	debug("gdt granularity : %01x\n", gdt->desc->g);
}
