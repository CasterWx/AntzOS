#include <todo.h>
#include <stdio.h>
#include <string.h>
#include <bmp.h>

;  // 2019/3/18/17点47分 此处莫名报错，得在void前加一个分号才可以 ，报错信息
/*
make.exe[2]: Entering directory `C:/Users/13252/Desktop/AntzS/AntzOS'
cc1.exe -I include/ -Os -Wall -quiet -o thun/thun.gas thun/thun.c
thun/thun.c:6: parse error before "void"
make.exe[2]: *** [thun/thun.gas] Error 1
*/

void to_printf_dijkstra(){
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  int i, x, y ;
  int k ; // 1024x768
    k = 0  ;
    for (y = 0; y < 60; y++) { //10800
      for (x = 0; x < 108; x++){
       // 16 + bmp_r[k]/43 + 6* (bmp_g[k]/43) + 36* (bmp_b[k]/43) ;
        print_area(binfo->vram, binfo->scrnx,bmp[k],x*10,y*13,x*10+10,y*13+13);
        k++;
      }
    }
}

void to_show(){
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  int i, x, y ;
  int k ; // 1024x768
    k = 0  ;
    for (y = 0; y < 60; y++) { //10800
      for (x = 0; x < 108; x++){
       // 16 + bmp_r[k]/43 + 6* (bmp_g[k]/43) + 36* (bmp_b[k]/43) ;
        print_area(binfo->vram, binfo->scrnx,bmp[k],x*10,y*13,x*10+10,y*13+13);
        k++;
      }
    }

}


void to_printf_pdd(){
  // struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  // int x, y;
  // int k = 0 ;
  // for (y = 0; y < 75; y++) { //75
  //   for (x = 0; x < 100; x++){ //100
  //     print_area(binfo->vram, binfo->scrnx , bmp[k] , x+binfo->scrnx-500, y+215, x+binfo->scrnx-500, y+215);
  //     k++;
  //   }
  // }
}
