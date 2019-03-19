#include <todo.h>
#include <stdio.h>
#include <string.h>

/*
* @author : CasterWx
* @time : 2019/3/19/16:37
* @content : 此处添加一个说明，keychar.c的目的是为了解决键盘中断。一次按键分别会产生按下与弹起两个不同16进制中断号，
			过去的Antz版本中使用了起落标志位[buttonFlag]来让两次中断产生一次效果，但在具体的使用过程中出现了键盘输
			入过快导致的标志位混乱的情况。所以最后选择了强制根据中断码来使一次中断的内容为空。
*/
char* replace_char(char s[40]){
	char *chr = "" ;
	if (strcmp(s,"9E")==0){
		chr = "" ;
	}else if(strcmp(s,"B0")==0){
		chr = "" ;
	}else if(strcmp(s,"AE")==0){
		chr = "" ;
	}else if(strcmp(s,"A0")==0){
		chr = "" ;
	}else if(strcmp(s,"92")==0){
		chr = "" ;
	}else if(strcmp(s,"A1")==0){
		chr = "" ;
	}else if(strcmp(s,"A2")==0){
		chr = "" ;
	}else if(strcmp(s,"A3")==0){
		chr = "" ;
	}else if(strcmp(s,"97")==0){
		chr = "" ;
	}else if(strcmp(s,"A4")==0){
		chr = "" ;
	}else if(strcmp(s,"A5")==0){
		chr = "" ;
	}else if(strcmp(s,"A6")==0){
		chr = "" ;
	}else if(strcmp(s,"B2")==0){
		chr = "" ;
	}else if(strcmp(s,"B1")==0){
		chr = "" ;
	}else if(strcmp(s,"98")==0){
		chr = "" ;
	}else if(strcmp(s,"99")==0){
		chr = "" ;
	}else if(strcmp(s,"90")==0){
		chr = "" ;
	}else if(strcmp(s,"93")==0){
		chr = "" ;
	}else if(strcmp(s,"9F")==0){
		chr = "" ;
	}else if(strcmp(s,"94")==0){
		chr = "" ;
	}else if(strcmp(s,"96")==0){
		chr = "" ;
	}else if(strcmp(s,"AF")==0){
		chr = "" ;
	}else if(strcmp(s,"91")==0){
		chr = "" ;
	}else if(strcmp(s,"AD")==0){
		chr = "" ;
	}else if(strcmp(s,"95")==0){
		chr = "" ;
	}else if(strcmp(s,"AC")==0){
		chr = "" ;
	}else if(strcmp(s,"B9")==0){
		chr = "" ;
	}else if((strcmp(s,"A7")==0)){
		chr = "" ;
	}else if((strcmp(s,"B4")==0)){
		chr = "" ;
	}else if((strcmp(s,"B3")==0)){
		chr = "" ;
	}else if((strcmp(s,"9A")==0)){
		chr = "" ;
	}else if((strcmp(s,"9B")==0)){
		chr = "" ;
	}else if((strcmp(s,"1A")==0)){
		chr = "[" ;
	}else if((strcmp(s,"1B")==0)){
		chr = "]" ;
	}else if((strcmp(s,"33")==0)){
		chr = "," ;
	}else if((strcmp(s,"27")==0)){
		chr = ";" ;
	}else if((strcmp(s,"34")==0)){
		chr = "." ;
	}else if((strcmp(s,"1E")==0)){
		chr = "a" ;
	}else if((strcmp(s,"30")==0)){
		chr = "b" ;
	}else if((strcmp(s,"2E")==0)){
		chr = "c" ;
	}else if((strcmp(s,"20")==0)){
		chr = "d" ;
	}else if((strcmp(s,"12")==0)){
		chr = "e" ;
	}else if((strcmp(s,"21")==0)){
		chr = "f" ;
	}else if((strcmp(s,"22")==0)){
		chr = "g" ;
	}else if((strcmp(s,"23")==0)){
		chr = "h" ;
	}else if((strcmp(s,"17")==0)){
		chr = "i" ;
	}else if((strcmp(s,"24")==0)){
		chr = "j" ;
	}else if((strcmp(s,"25")==0)){
		chr = "k" ;
	}else if((strcmp(s,"26")==0)){
		chr = "l" ;
	}else if((strcmp(s,"32")==0)){
		chr = "m" ;
	}else if((strcmp(s,"31")==0)){
		chr = "n" ;
	}else if((strcmp(s,"18")==0)){
		chr = "o" ;
	}else if((strcmp(s,"19")==0)){
		chr = "p" ;
	}else if((strcmp(s,"10")==0)){
		chr = "q" ;
	}else if((strcmp(s,"13")==0)){
		chr = "r" ;
	}else if((strcmp(s,"1F")==0)){
		chr = "s" ;
	}else if((strcmp(s,"14")==0)){
		chr = "t" ;
	}else if((strcmp(s,"16")==0)){
		chr = "u" ;
	}else if((strcmp(s,"2F")==0)){
		chr = "v" ;
	}else if((strcmp(s,"11")==0)){
		chr = "w" ;
	}else if((strcmp(s,"2D")==0)){
		chr = "x" ;
	}else if((strcmp(s,"15")==0)){
		chr = "y" ;
	}else if((strcmp(s,"2C")==0)){
		chr = "z" ;
	}else if((strcmp(s,"39")==0)){// 空格
		chr = " " ;
	}else if(strcmp(s,"9C")==0){ // 回车的弹起中断
	/**
	*@author : CasterWx
	*@time : 2019/3/19/16:43
	*@content : 此处本来是将回车的chr设置为空的，但是为了方便vimShow排版，此处将回车的chr设置为"\n"。
	**/
		// old Code  : chr = "" ;
		chr = "";
		//  fix :显示bug
	}else if(strcmp(s,"BB")==0){
		chr = "" ;
	}
	return chr ;
}
