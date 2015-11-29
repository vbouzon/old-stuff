// this file contain the function used for the geomipmapping

#include "write.h"

void write_triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{float d=9;
	
glColor3f(.7,1,.7);
////////////////////////////////////////
if(y1-y2>d || y1-y2<-d ||y3-y2>d || y3-y2<-d ||y1-y3>d || y1-y3<-d)glColor3f(.7,.7,0.0);
 ////////////////////////////////////////
        if(x1>x2)
	{glTexCoord2f(1, 0);
	glVertex3f(x1,y1,z1);
	glTexCoord2f(0, 1);
	glVertex3f(x2,y2,z2);
	glTexCoord2f(1, 1);
	glVertex3f(x3,y3,z3);}
	else
	{
	glTexCoord2f(0, 0);
	glVertex3f(x1,y1,z1);
	glTexCoord2f(0,1);
	glVertex3f(x2,y2,z2);
	glTexCoord2f(1,0);
	glVertex3f(x3,y3,z3);}
	
}
void write_bloc(int i, int j,float map[257][257],int d,short test[16][16])
{
        int ii,jj,i0,i1,i2,i4,j0,j1,j2,j4;
        /* Les blocs sont de taille 16x16*/
        if(d<4)
        {
	for(ii=0;ii<=15;ii++)
        {
                i0=ii+i*16;
                i1=i0+1;
                for(jj=0;jj<=15;jj++)
                {
                        j0=jj+j*16;
                        j1=j0+1;
                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                       (float)(i0),map[i0][j1]/25.5,(float)(j1),
                                       (float)(i1),map[i1][j0]/25.5,(float)(j0));

                        write_triangle((float)(i1),map[i1][j0]/25.5,(float)(j0),
                                       (float)(i0),map[i0][j1]/25.5,(float)(j1),
                                       (float)(i1),map[i1][j1]/25.5,(float)(j1));
                }
        }
	test[i][j]=1;
	}
        else
        if(d<16)
        {
	for(ii=2;ii<=12;ii+=2)
        {
                i0=ii+i*16;
                //i1 inutile.
                i2=i0+2;
                for(jj=2;jj<=12;jj+=2)
                {
                        j0=jj+j*16;
                        j2=j0+2;
                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
                }
        }
	test[i][j]=2;
        }
	else
        {
        for(ii=4;ii<=8;ii+=4)
        {
                i0=ii+i*16;
                i4=i0+4;
                for(jj=4;jj<=8;jj+=4)
                {
                        j0=jj+j*16;
                        j4=j0+4;
                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                       (float)(i0),map[i0][j4]/25.5,(float)(j4),
                                       (float)(i4),map[i4][j0]/25.5,(float)(j0));

                        write_triangle((float)(i4),map[i4][j0]/25.5,(float)(j0),
                                       (float)(i0),map[i0][j4]/25.5,(float)(j4),
                                       (float)(i4),map[i4][j4]/25.5,(float)(j4));
                }
        }
	test[i][j]=3;
	}
}

void write_bords(float map[257][257],short test[16][16])
	{
	int i,ii,j,jj,i0,i1,i2,j0,j1,j2;
	for(i=0;i<=15;i++)
		{
		for(j=0;j<=15;j++)
			{
			if(test[i][j]==2)
			{
			if(i>0 && test[i-1][j]==1 )
				{
				ii=0;
                                i0=ii+i*16;
                                //i1=ii+1+i*16;
                                i2=ii+2+i*16;
                                for(jj=2;jj<=12;jj+=2)
				        {
                                        j0=jj+j*16;
                                        j1=j0+1;
                                        j2=j1+1;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j1]/25.5,(float)(j1),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

				        write_triangle((float)(i0),map[i0][j1]/25.5,(float)(j1),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                jj=14;
                                j0=jj+j*16;
                                j1=j0+1;
                                j2=j1+1;

                                // Affichage du demi angle en bas à gauche.
                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j1]/25.5,(float)(j1),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));

				write_triangle((float)(i0),map[i0][j1]/25.5,(float)(j1),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
                                jj=0;
                                j0=jj+j*16;
                                j1=jj+1+j*16;
                                j2=jj+2+j*16;
                                // Affichage du demi angle en haut à gauche inversé !
                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j1]/25.5,(float)(j1),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
				write_triangle((float)(i0),map[i0][j1]/25.5,(float)(j1),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
				}
			else
				{
                                ii=0;
                                i0=ii+i*16;
                                //i1=ii+1+i*16;
                                i2=ii+2+i*16;
                                for(jj=2;jj<=12;jj+=2)
				        {
                                        j0=jj+j*16;
                                        j1=jj+1+j*16;
                                        j2=jj+2+j*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                jj=14;
                                j0=jj+j*16;
                                j1=jj+1+j*16;
                                j2=jj+2+j*16;

                                // Affichage du demi angle en bas à gauche.
                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
                                jj=0;
                                j0=jj+j*16;
                                j1=jj+1+j*16;
                                j2=jj+2+j*16;
                                // Affichage du demi angle en haut à gauche inversé !
                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
				}

			if(i<15 && test[i+1][j]==1 )
				{
                                ii=14;
                                i0=ii+i*16;
                                //i1=ii+1+i*16;
                                i2=ii+2+i*16;
                                for(jj=2;jj<=12;jj+=2)
				        {
                                        j0=jj+j*16;
                                        j1=jj+1+j*16;
                                        j2=jj+2+j*16;
                                        write_triangle((float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j1]/25.5,(float)(j1));

				        write_triangle((float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j1]/25.5,(float)(j1),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));
				        }
                                jj=14;
                                j0=jj+j*16;
                                j1=jj+1+j*16;
                                j2=jj+2+j*16;

                                // Affichage du demi angle en bas à droite inversé.
                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j1]/25.5,(float)(j1));

				write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i2),map[i2][j1]/25.5,(float)(j1),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
                                jj=0;
                                j0=jj+j*16;
                                j1=jj+1+j*16;
                                j2=jj+2+j*16;
                                // Affichage du demi angle en haut à droite.
                                write_triangle((float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j1]/25.5,(float)(j1));

				write_triangle((float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j1]/25.5,(float)(j1),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
				}
			else
				{
				ii=14;
                                i0=ii+i*16;
                                //i1=ii+1+i*16;
                                i2=ii+2+i*16;
                                for(jj=2;jj<=12;jj+=2)
				        {
                                        j0=jj+j*16;
                                        j1=jj+1+j*16;
                                        j2=jj+2+j*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                jj=0;
                                j0=jj+j*16;
                                j1=jj+1+j*16;
                                j2=jj+2+j*16;

                                write_triangle((float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
                                jj=14;
                                j0=jj+j*16;
                                j1=jj+1+j*16;
                                j2=jj+2+j*16;

                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
				}

			if(j>0 && test[i][j-1]==1 )
				{
				jj=0;
                                j0=jj+j*16;
                                //j1=jj+1+j*16;
                                j2=jj+2+j*16;
                                for(ii=2;ii<=12;ii+=2)
				        {
                                        i0=ii+i*16;
                                        i1=ii+1+i*16;
                                        i2=ii+2+i*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i1),map[i1][j0]/25.5,(float)(j0));

				        write_triangle((float)(i1),map[i1][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                ii=14;
                                i0=ii+i*16;
                                i1=ii+1+i*16;
                                i2=ii+2+i*16;

                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i1),map[i1][j0]/25.5,(float)(j0));

				write_triangle((float)(i1),map[i1][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
                                ii=0;
                                i0=ii+i*16;
                                i1=ii+1+i*16;
                                i2=ii+2+i*16;

                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i1),map[i1][j0]/25.5,(float)(j0));

				write_triangle((float)(i1),map[i1][j0]/25.5,(float)(j0),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
				}
                        else
				{
                                jj=0;
                                j0=jj+j*16;
                                //j1=jj+1+j*16;
                                j2=jj+2+j*16;
                                for(ii=2;ii<=12;ii+=2)
				        {
                                        i0=ii+i*16;
                                        i1=ii+1+i*16;
                                        i2=ii+2+i*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                ii=14;
                                i0=ii+i*16;
                                i1=ii+1+i*16;
                                i2=ii+2+i*16;

                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
                                ii=0;
                                i0=ii+i*16;
                                i1=ii+1+i*16;
                                i2=ii+2+i*16;

                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
				}
			if(j<15 && test[i][j+1]==1 )
                                {
				jj=14;
                                j0=jj+j*16;
                                //j1=jj+1+j*16;
                                j2=jj+2+j*16;
                                for(ii=2;ii<=12;ii+=2)
				        {
                                        i0=ii+i*16;
                                        i1=ii+1+i*16;
                                        i2=ii+2+i*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

				        write_triangle((float)(i1),map[i1][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i1),map[i1][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                ii=14;
                                i0=ii+i*16;
                                i1=ii+1+i*16;
                                i2=ii+2+i*16;

                                write_triangle((float)(i1),map[i1][j2]/25.5,(float)(j2),
                                               (float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2));

				write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i1),map[i1][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
                                ii=0;
                                i0=ii+i*16;
                                i1=ii+1+i*16;
                                i2=ii+2+i*16;

                                write_triangle((float)(i1),map[i1][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2));

				write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                               (float)(i1),map[i1][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
				}
                        else
				{
                                jj=14;
                                j0=jj+j*16;
                                //j1=jj+1+j*16;
                                j2=jj+2+j*16;
                                for(ii=2;ii<=12;ii+=2)
				        {
                                        i0=ii+i*16;
                                        i1=ii+1+i*16;
                                        i2=ii+2+i*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                ii=0;
                                i0=ii+i*16;
                                i1=ii+1+i*16;
                                i2=ii+2+i*16;

                                write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
                                ii=14;
                                i0=ii+i*16;
                                i1=ii+1+i*16;
                                i2=ii+2+i*16;

                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
				}

                        }

                        // bord des blocs l'ordre 3
                        if(test[i][j]==3)
			{
			if(i>0 && test[i-1][j]==2 )
				{
				ii=0;
                                i0=ii+i*16;
                                //i1=ii+1+i*16;
                                i2=ii+4+i*16;
                                for(jj=4;jj<=8;jj+=4)
				        {
                                        j0=jj+j*16;
                                        j1=jj+2+j*16;
                                        j2=jj+4+j*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j1]/25.5,(float)(j1),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

				        write_triangle((float)(i0),map[i0][j1]/25.5,(float)(j1),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                jj=12;
                                j0=jj+j*16;
                                j1=jj+2+j*16;
                                j2=jj+4+j*16;

                                // Affichage du demi angle en bas à gauche.
                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j1]/25.5,(float)(j1),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));

				write_triangle((float)(i0),map[i0][j1]/25.5,(float)(j1),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
                                jj=0;
                                j0=jj+j*16;
                                j1=jj+2+j*16;
                                j2=jj+4+j*16;
                                // Affichage du demi angle en haut à gauche inversé !
                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j1]/25.5,(float)(j1),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
				write_triangle((float)(i0),map[i0][j1]/25.5,(float)(j1),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
				}
			else
				{
                                ii=0;
                                i0=ii+i*16;
                                //i1=ii+1+i*16;
                                i2=ii+4+i*16;
                                for(jj=4;jj<=8;jj+=4)
				        {
                                        j0=jj+j*16;
                                        j1=jj+2+j*16;
                                        j2=jj+4+j*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                jj=12;
                                j0=jj+j*16;
                                j1=jj+2+j*16;
                                j2=jj+4+j*16;

                                // Affichage du demi angle en bas à gauche.
                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
                                jj=0;
                                j0=jj+j*16;
                                j1=jj+2+j*16;
                                j2=jj+4+j*16;
                                // Affichage du demi angle en haut à gauche inversé !
                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
				}

			if(i<15 && test[i+1][j]==2 )
				{
                                ii=12;
                                i0=ii+i*16;
                                //i1=ii+1+i*16;
                                i2=ii+4+i*16;
                                for(jj=4;jj<=8;jj+=4)
				        {
                                        j0=jj+j*16;
                                        j1=jj+2+j*16;
                                        j2=jj+4+j*16;
                                        write_triangle((float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j1]/25.5,(float)(j1));

				        write_triangle((float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j1]/25.5,(float)(j1),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));
				        }
                                jj=12;
                                j0=jj+j*16;
                                j1=jj+2+j*16;
                                j2=jj+4+j*16;

                                // Affichage du demi angle en bas à droite inversé.
                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j1]/25.5,(float)(j1));

				write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i2),map[i2][j1]/25.5,(float)(j1),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
                                jj=0;
                                j0=jj+j*16;
                                j1=jj+2+j*16;
                                j2=jj+4+j*16;
                                // Affichage du demi angle en haut à droite.
                                write_triangle((float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j1]/25.5,(float)(j1));

				write_triangle((float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j1]/25.5,(float)(j1),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
				}
			else
				{
				ii=12;
                                i0=ii+i*16;
                                //i1=ii+1+i*16;
                                i2=ii+4+i*16;
                                for(jj=4;jj<=8;jj+=4)
				        {
                                        j0=jj+j*16;
                                        j1=jj+2+j*16;
                                        j2=jj+4+j*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                jj=0;
                                j0=jj+j*16;
                                j1=jj+2+j*16;
                                j2=jj+4+j*16;

                                write_triangle((float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
                                jj=12;
                                j0=jj+j*16;
                                j1=jj+2+j*16;
                                j2=jj+4+j*16;

                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
				}

			if(j>0 && test[i][j-1]==2 )
				{
				jj=0;
                                j0=jj+j*16;
                                //j1=jj+1+j*16;
                                j2=jj+4+j*16;
                                for(ii=4;ii<=8;ii+=4)
				        {
                                        i0=ii+i*16;
                                        i1=ii+2+i*16;
                                        i2=ii+4+i*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i1),map[i1][j0]/25.5,(float)(j0));

				        write_triangle((float)(i1),map[i1][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                ii=12;
                                i0=ii+i*16;
                                i1=ii+2+i*16;
                                i2=ii+4+i*16;

                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i1),map[i1][j0]/25.5,(float)(j0));

				write_triangle((float)(i1),map[i1][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
                                ii=0;
                                i0=ii+i*16;
                                i1=ii+2+i*16;
                                i2=ii+4+i*16;

                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i1),map[i1][j0]/25.5,(float)(j0));

				write_triangle((float)(i1),map[i1][j0]/25.5,(float)(j0),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
				}
                        else
				{
                                jj=0;
                                j0=jj+j*16;
                                //j1=jj+1+j*16;
                                j2=jj+4+j*16;
                                for(ii=4;ii<=8;ii+=4)
				        {
                                        i0=ii+i*16;
                                        i1=ii+2+i*16;
                                        i2=ii+4+i*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                ii=12;
                                i0=ii+i*16;
                                i1=ii+2+i*16;
                                i2=ii+4+i*16;

                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
                                ii=0;
                                i0=ii+i*16;
                                i1=ii+2+i*16;
                                i2=ii+4+i*16;

                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0));
				}
			if(j<15 && test[i][j+1]==2 )
                                {
				jj=12;
                                j0=jj+j*16;
                                //j1=jj+1+j*16;
                                j2=jj+4+j*16;
                                for(ii=4;ii<=8;ii+=4)
				        {
                                        i0=ii+i*16;
                                        i1=ii+2+i*16;
                                        i2=ii+4+i*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

				        write_triangle((float)(i1),map[i1][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i1),map[i1][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                ii=12;
                                i0=ii+i*16;
                                i1=ii+2+i*16;
                                i2=ii+4+i*16;

                                write_triangle((float)(i1),map[i1][j2]/25.5,(float)(j2),
                                               (float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2));

				write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i1),map[i1][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
                                ii=0;
                                i0=ii+i*16;
                                i1=ii+2+i*16;
                                i2=ii+4+i*16;

                                write_triangle((float)(i1),map[i1][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2));

				write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                               (float)(i1),map[i1][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
				}
                        else
				{
                                jj=12;
                                j0=jj+j*16;
                                //j1=jj+1+j*16;
                                j2=jj+4+j*16;
                                for(ii=4;ii<=8;ii+=4)
				        {
                                        i0=ii+i*16;
                                        i1=ii+2+i*16;
                                        i2=ii+4+i*16;
                                        write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j0]/25.5,(float)(j0));

                                        write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                                       (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                                       (float)(i2),map[i2][j2]/25.5,(float)(j2));
				        }
                                ii=0;
                                i0=ii+i*16;
                                i1=ii+2+i*16;
                                i2=ii+4+i*16;

                                write_triangle((float)(i2),map[i2][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
                                ii=12;
                                i0=ii+i*16;
                                i1=ii+2+i*16;
                                i2=ii+4+i*16;

                                write_triangle((float)(i0),map[i0][j0]/25.5,(float)(j0),
                                               (float)(i0),map[i0][j2]/25.5,(float)(j2),
                                               (float)(i2),map[i2][j2]/25.5,(float)(j2));
				}

                        }
			}
		}


	}
