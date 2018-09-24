/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp()
{
   debug("kernel mem [0x%x - 0x%x]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);

   mbi_t* mbi;
   multiboot_memory_map_t *start;
   multiboot_memory_map_t *end;

   multiboot_uint64_t * val;
   val = (multiboot_uint64_t*) 0xffffffff;
   *val = 516513;
 

   debug("0x%llx\n", &val);
   
   
   mbi = (mbi_t*) info->mbi;
   start = (multiboot_memory_map_t*) mbi->mmap_addr;
   end = (multiboot_memory_map_t*) (mbi-> mmap_addr + mbi->mmap_length);

   while (start < end)
     {
       debug("mmap_addr 0x%llx, mmap_length 0x%llx\n", start->addr, start->len);
       start++;
     }

   
}
