/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <string.h>

#define CSD_TYPE 0x9A
#define DSD_TYPE 0x92

extern info_t *info;

	// uint64_t    limit_1:16;    /* bits 00-15 of the segment limit */
	// uint64_t    base_1:16;     /* bits 00-15 of the base address */
	// uint64_t    base_2:8;      /* bits 16-23 of the base address */
	// uint64_t    type:4;        /* segment type */
	// uint64_t    s:1;           /* descriptor type */
	// uint64_t    dpl:2;         /* descriptor privilege level */
	// uint64_t    p:1;           /* segment present flag */
	// uint64_t    limit_2:4;     /* bits 16-19 of the segment limit */
	// uint64_t    avl:1;         /* available for fun and profit */
	// uint64_t    l:1;           /* longmode */
	// uint64_t    d:1;           /* default length, depend on seg type */
	// uint64_t    g:1;           /* granularity */
	// uint64_t    base_3:8;      /* bits 24-31 of the base address */

void show_gdt(gdt_reg_t gdtr){
	
	unsigned short gdtr_size = gdtr.limit + 1;
	int number_of_segments = gdtr_size/sizeof(seg_desc_t);
	int i;
	for(i=0; i < number_of_segments; i++){
		seg_desc_t *dsc   = &gdtr.desc[i];
      		uint32_t    base  = dsc->base_3<<24 | dsc->base_2<<16 | dsc->base_1;
      		uint32_t    limit = dsc->limit_2<<16| dsc->limit_1;
		
      		debug("GDT[%d] = 0x%llx | base 0x%x | limit 0x%x | type 0x%x | granularity 0x%x\n", i, gdtr.desc[i].raw, base, limit, dsc->type, dsc->g);

	}	
}

//attention : limite absolue, pas relative
void gdt_setup_descriptor(seg_desc_t * seg_p, unsigned long base, unsigned long limit, uint8_t type){
    uint8_t * oct = (uint8_t*)seg_p;
    
    if ((limit > 65536) && (limit & 0xFFF) != 0xFFF) {
        debug("Impossible lim 0x%lx\n", limit);
    }
    if (limit > 65536) {
       // Adjust granularity if required
        debug("Gran. Ancienne limite  0x%lx\n", limit);
        limit = limit >> 12;
        oct[6] = 0xC0;
        debug("Gran. Nouvelle limite  0x%lx\n", limit);

    } else {
       oct[6] = 0x40;
    }
    seg_p->g = 1&0x1;

    oct[2] = base & 0xFF;
    oct[3] =  (base >> 8) & 0xFF;
    oct[4] =  (base >> 16) & 0xFF;
    oct[7] =  (base >> 24) & 0xFF;
 
    oct[0] = limit & 0xFF;
    oct[1] = (limit >> 8)  & 0xFF;
    oct[6] |= (limit >> 16)  & 0xFF;

    oct[5] = type & 0xFF;
    seg_p->l = 1&0x1;

}

void tp()
{
	gdt_reg_t gdt;
	get_gdtr(gdt);

    	debug("///////Show initial GDT///////\n");

	show_gdt(gdt);

    	debug("///////Add new GDT///////\n");
    	
    	gdt_reg_t gdt_addr_new;
    	seg_desc_t gdt_tab[4];
    	if ((int )&gdt_tab[0] % 8 != 0){
      	  debug("Not aligned! T0D0. current addr = 0x%x \n", gdt_tab);
    	}
    	gdt_addr_new.desc = &gdt_tab[0];
    	gdt_addr_new.limit = 4 * sizeof(seg_desc_t) - 1;

    	debug("Limite :  0x%x, Adr de base : 0x%lx\n", gdt_addr_new.limit, gdt_addr_new.addr);

    	gdt_tab[0] = (seg_desc_t) *gdt.desc ;

//gdt_setup_descriptor(seg_desc_t * seg_p, unsigned long base, unsigned long limit, uint8_t type)
    	gdt_setup_descriptor(&gdt_tab[1], 0x00000, 0xfff0, CSD_TYPE);
    	gdt_setup_descriptor(&gdt_tab[2], 0x00000, 0xfff0, DSD_TYPE);
    	gdt_setup_descriptor(&gdt_tab[3], 0x600000, 0x20, DSD_TYPE);
    	gdt_tab[3].g = 0&0x1;
    	show_gdt(gdt_addr_new);

    	set_gdtr(gdt_addr_new); 
    	set_cs(0x08);
    	set_ds(0x10);
    	set_ss(0x10);
    
    	set_fs(0x10);
    	set_gs(0x10);
    

    	char  src[64];
    	char *dst = 0;

    	memset(src, 0xff, 64);
    	set_es(0x18);
    	_memcpy8(dst, src, 32);
    	// ca marche. Il faut 'setter' es juste avant car memset change le contenu de es

    	_memcpy8(dst, src, 64);
	

	
}
