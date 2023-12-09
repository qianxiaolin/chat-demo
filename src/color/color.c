/*
 * 	file:color.c
 * 	description:lib to make print with different color and position
 * 	author:qianxiao
 * 	date:2023/12/8
 */
#include<stdio.h>
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define LIGHT_GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[0;34m"
#define WHITE "\033[0;37m"

typedef const char* Color;
void clean_screen(){
	printf("\033[2J");
}
void clean_line(){
}
void set_cursor_xy(int x,int y){
	fprintf(stdout,"\033[%d;%dH",x,y);
}
#define  print_color_text(color,str)\
	fprintf(stdout,"%s%s\n",(color),(str));
int main(){
	print_color_text(RED,"hello");
	print_color_text(LIGHT_GREEN,"hello");
	print_color_text(GREEN,"hello");
	print_color_text(YELLOW,"hello");
	print_color_text(BLUE,"hello");
	set_cursor_xy(9,0);
	clean_screen();
	return 0;
}
