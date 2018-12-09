#include <todo.h>
#include <stdio.h>
#include <string.h>
#include <bmp.h>

void to_printf_dijkstra(){
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  int x, y;
  int k = 0 ;
  for (y = 0; y < 115; y++) {
    for (x = 0; x < 100; x++){
      int rgb_num = 16 + bmp_r[k]/43 + 6* (bmp_g[k]/43) + 36* (bmp_b[k]/43) ;
      print_area(binfo->vram, binfo->scrnx , rgb_num , x+binfo->scrnx-100, y, x+binfo->scrnx-100, y);
      k++;
    }
  }
}

void to_printf_pdd(){
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  int x, y;
  int k = 0 ;
  for (y = 0; y < 75; y++) { //75
    for (x = 0; x < 100; x++){ //100
      print_area(binfo->vram, binfo->scrnx , bmp[k] , x+binfo->scrnx-500, y+215, x+binfo->scrnx-500, y+215);
      k++;
    }
  }
}
