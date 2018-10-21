#include <todo.h>
#include <stdio.h>
#include <string.h>

void to_printf(int x,int y){
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  // 打印测试
  print_string(binfo->vram, binfo->scrnx, x, y, COL8_FFFFFF, "welcome come!");
}
