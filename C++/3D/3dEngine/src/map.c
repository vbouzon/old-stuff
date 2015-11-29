// this file load the landscape in memory
#include "map.h"
#include "util_srf_gcc.h"

void make_map(char* file,short maptype,float map[257][257])
{
int i,j;FILE *F1;float val;F1=fopen(file,"w");
for(i=0;i<=256;i++){for(j=0;j<=256;j++){
	if(maptype==1){val=10*(1+cos(((float)i*5/(25.0))*2*M_PI))+30*(1+cos(((float)i*1/(25.0))*2*M_PI))+30*(1+cos(((float)j*1/(25.0))*2*M_PI))+10*(1+cos(((float)j*5/(25.0))*2*M_PI));}
	if(maptype==2){if (j<130 && j>120 && i>120 && i<130 ) val=255; else val=1;}
	if(maptype==3){if (!i || !j || j==240 || i==240) val=255; else val=0;}

fprintf(F1,"%.0f\t",val);}fprintf(F1,"\n");}fclose(F1);
}




void load_map(char* file,float map[257][257])
{
	int i,j;
	FILE *F1;
	F1=fopen(file,"r");
	for(i=0;i<=256;i++)
	{
		for(j=0;j<=256;j++)
		{
			fscanf(F1,"%f\t",&map[i][j]);
		}
		fscanf(F1,"\n");
	}
	fclose(F1);
}
int load_ter(char* file,float map[257][257])
{
	float ** img;
	int l,h,i,j;
	
	img=f_2D_Read_image_data_gcc("terrain.ras",&l,&h);
	
	if (l!=257 || h!=257) {printf("Erreur d'initialisation de la map...\n La taille est de l=%d h=%d la map doit faire 256x256\n",l,h); return(-1);}
	for(i=0;i<h;i++)
	{ 
		for(j=0;j<l;j++)
		{
			map[i][j]=img[i][j];
//			printf("val : %d\n",map[i][j]);
		}
	}
	
	//for(i=0;i<h;i++) free(img[i]);             /* on libere la ram*/
	//free(img);
	return(1);
}
int load_arbre(char* file,int arbr[50][4])
{
    int i;
	FILE *F1;
	F1=fopen(file,"r");
	for(i=0;i<50;i++)
	{
			fscanf(F1,"%d-%d-%d-%d\n",&arbr[i][0],&arbr[i][1],&arbr[i][2],&arbr[i][3]);
	}
	fclose(F1);
    
}
