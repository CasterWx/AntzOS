#include <todo.h>
#include <stdio.h>
#include <string.h>
#include <bmp.h>

// 2019/3/18/17点47分 此处莫名报错，得在void前加一个分号才可以 ，报错信息
// 已解决，头文件的问题。
/*
make.exe[2]: Entering directory `C:/Users/13252/Desktop/AntzS/AntzOS'
cc1.exe -I include/ -Os -Wall -quiet -o thun/thun.gas thun/thun.c
thun/thun.c:6: parse error before "void"
make.exe[2]: *** [thun/thun.gas] Error 1
*/

void to_printf_dijkstra(){
    to_show() ;
}

void to_show(){
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  int i, x, y , m , n;
  int k ; // 1024x768
    for (y = 0; y < 60; y++) { //10800
      for (x = 0; x < 108; x++){
        // 16 + bmp_r[k]/43 + 6* (bmp_g[k]/43) + 36* (bmp_b[k]/43); 24色RGB转换公式
        /**
          * 取得为bmp[y][x]，接下来需要在(x*10,y*13)~(x*10+10,y*13+13)区间填充这个颜色。
          **/
        for(m=y*13; m<y*13+13; m++){
            for(n=x*10; n<x*10+10; n++){
                // int fy1 = (((x*10+10)-n)/10)*bmp[y+1][x] + ((n-x*10)/10)*bmp[y+1][x+1];
                // int fy2 = (((x*10+10)-n)/10)*bmp[y][x] + ((n-x*10)/10)*bmp[y][x+1];
                // int rgbnum = ((m-y*13)/(13))*fy1 + ((y*13+13-m)/13)*fy2 ;
                double fy1 = (((double)(x*10.0+10.0)-n)/10.0)*bmp[y+1][x] + ((double)(n-x*10.0)/10.0)*bmp[y+1][x+1];
                double fy2 = (((double)(x*10.0+10)-n)/10.0)*bmp[y][x] + ((double)(n-x*10.0)/10.0)*bmp[y][x+1];
                int rgbnum = (int)(((double)(m-y*13.0)/(13.0))*fy1 + ((double)(y*13.0+13.0-m)/13.0)*fy2 );
                print_area(binfo->vram, binfo->scrnx,rgbnum,n,m,n,m);
            }
        }     
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
