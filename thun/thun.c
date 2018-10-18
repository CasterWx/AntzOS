#include <todo.h>
#include <stdio.h>
#include <string.h>

void to_printf(){
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  putfonts8_asc(binfo->vram, binfo->scrnx, 57, 57, COL8_FFFFFF, "new printf");
}
