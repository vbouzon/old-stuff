#include <math.h>
#include <stdio.h>
//#include "util_srf_gcc.c"

void make_map(char* file,short maptype,float map[257][257]);
void load_map(char* file,float map[257][257]);
int load_ter(char* file,float map[257][257]);
int load_arbre(char* file,int arbr[50][4]);
