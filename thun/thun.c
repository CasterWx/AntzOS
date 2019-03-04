#include <todo.h>
#include <stdio.h>
#include <string.h>
#include <bmp.h>


int Step2_MidFilter() {
	unsigned int *image1 = bmp ;
	unsigned int image2[2] = {};
	return 0;
}

void Sort(unsigned int*data, int dsize) {
	unsigned int temp = 0;
  int i,j ;
	for (i = 0; i < dsize; i++) {
		for (j = dsize - 1; j > i; j--) {
			if (data[j] < data[j - 1]) {
				temp = data[j];
				data[j] = data[j - 1];
				data[j - 1] = temp;
			}
		}
	}
}


void to_printf_dijkstra(){
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  int x, y;
  int k = 0 ; //100 115 yuan   // 100 108
  for (y = 0; y < 108; y++) { //10800
    for (x = 0; x < 100; x++){
      int rgb_num = bmp[k] ; // 16 + bmp_r[k]/43 + 6* (bmp_g[k]/43) + 36* (bmp_b[k]/43) ;
      if (rgb_num>255/2){
        print_area(binfo->vram, binfo->scrnx , 7 , x+binfo->scrnx-100, y, x+binfo->scrnx-100, y);
      }else {
        print_area(binfo->vram, binfo->scrnx , 0 , x+binfo->scrnx-100, y, x+binfo->scrnx-100, y);
      }
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
