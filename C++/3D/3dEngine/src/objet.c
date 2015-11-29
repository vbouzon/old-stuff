#include "objet.h"

void arbre(float ox,float oy,float oz,float larg, unsigned int TEX )
{
glBindTexture( GL_TEXTURE_2D, TEX );
glColor4f(1,1,1,1);
larg*=3;
float haut=4;
glEnable(GL_ALPHA_TEST);
glAlphaFunc(GL_GREATER, 0.1);
glBegin(GL_TRIANGLES);
	glTexCoord2f(0,.99);glVertex3f(ox,oy+haut,oz-larg/2);
	glTexCoord2f(.99,.99);glVertex3f(ox,oy+haut,oz+larg/2);	
	glTexCoord2f(0,0);glVertex3f(ox,oy,oz-larg/2);
	
	glTexCoord2f(.99,.99);glVertex3f(ox,oy+haut,oz+larg/2);
	glTexCoord2f(0,0);glVertex3f(ox,oy,oz-larg/2);
	glTexCoord2f(.99,0);glVertex3f(ox,oy,oz+larg/2);
	
	glTexCoord2f(0,.99);glVertex3f(ox-larg/2,oy+haut,oz);
	glTexCoord2f(.99,.99);glVertex3f(ox+larg/2,oy+haut,oz);	
	glTexCoord2f(0,0);glVertex3f(ox-larg/2,oy,oz);
	
	glTexCoord2f(.99,.99);glVertex3f(ox+larg/2,oy+haut,oz);	
	glTexCoord2f(0,0);glVertex3f(ox-larg/2,oy,oz);
	glTexCoord2f(.99,0);glVertex3f(ox+larg/2,oy,oz);		
glEnd();
glColor4f(1,1,1,1);
}

void arbre(float ox,float oy,float oz, unsigned int TEX)
{float larg=0.2,haut=3;
oz=-oz;
glColor3f(.4,.4,0);
glDisable(GL_TEXTURE_2D);
GLUquadricObj *cylindre;
cylindre = gluNewQuadric();
glPushMatrix();
glRotatef(-90,1,0,0);
glTranslatef(ox,oz,oy);
gluCylinder(cylindre, larg, larg*0.1, 2*haut, 10, 2);
glPopMatrix();
glColor3f(0,.5,0);
glEnable(GL_TEXTURE_2D);
glBindTexture( GL_TEXTURE_2D, TEX );
/////////////
glPushMatrix();
glTranslatef(ox,oy+haut/2,-oz);
glRotatef(-45,0,1,0);
glBegin(GL_TRIANGLES);
branche(haut,larg,1.5);
glEnd();
glPopMatrix();
glPushMatrix();
glTranslatef(ox,oy+haut/2,-oz);
glBegin(GL_TRIANGLES);
branche(haut,larg,1.5);
glEnd();
glPopMatrix();
/////////////
glPushMatrix();
glTranslatef(ox,oy+haut*.9,-oz);
glRotatef(-60,0,1,0);
glScalef(.8,.8,.8);
glBegin(GL_TRIANGLES);
branche(haut,larg,4);
glEnd();
glPopMatrix();
glPushMatrix();
glTranslatef(ox,oy+haut*.9,-oz);
glScalef(.8,.8,.8);
glBegin(GL_TRIANGLES);	
branche(haut,larg,5);
glEnd();
glPopMatrix();
///////////////
glPushMatrix();
glTranslatef(ox,oy+haut*1.3,-oz);
glScalef(.5,.5,.5);
glBegin(GL_TRIANGLES);
branche(haut,larg,6);
glEnd();
glPopMatrix();
glPushMatrix();
glTranslatef(ox,oy+haut*1.3,-oz);
glRotatef(-45,0,1,0);
glScalef(.5,.5,.5);
glBegin(GL_TRIANGLES);
branche(haut,larg,7);
glEnd();
glPopMatrix();
///////////////
glPushMatrix();
glTranslatef(ox,oy+haut*1.6,-oz);
glRotatef(30,0,1,0);
glScalef(.35,.35,.35);
glBegin(GL_TRIANGLES);	
branche(haut,larg,8);
glEnd();
glPopMatrix();
glPushMatrix();
glTranslatef(ox,oy+haut*1.6,-oz);
glScalef(.35,.35,.35);
glBegin(GL_TRIANGLES);	
branche(haut,larg,9);
glEnd();
glPopMatrix();
///////////////
glPushMatrix();
glTranslatef(ox,oy+haut*1.8,-oz);
glRotatef(-45,0,1,0);
glScalef(.2,.2,.2);
glBegin(GL_TRIANGLES);	
branche(haut,larg,10);
glEnd();
glPopMatrix();
glPushMatrix();
glTranslatef(ox,oy+haut*1.8,-oz);
glScalef(.2,.2,.2);
glBegin(GL_TRIANGLES);	
branche(haut,larg,11);
glEnd();
glPopMatrix();
////////////

glPushMatrix();
glTranslatef(ox,oy+haut*2,-oz);
glRotatef(-30,0,1,0);
glScalef(.1,.1,.1);
glBegin(GL_TRIANGLES);	
branche(haut,larg,16);
glEnd();
glPopMatrix();

glPushMatrix();
glTranslatef(ox,oy+haut*2,-oz);
glScalef(.1,.1,.1);
glBegin(GL_TRIANGLES);	
branche(haut,larg,16);
glEnd();
glPopMatrix();
////////////
}


static void branche(float haut,float larg, float n ){
    glTexCoord2f(0,.0);glVertex3f(-1,0,0);
	glTexCoord2f(0.5,1);glVertex3f(0,-haut*n/6,+10*larg);	
	glTexCoord2f(1,0);glVertex3f(1,0,0);
	
	glTexCoord2f(0,.0);glVertex3f(-1,0,0);
	glTexCoord2f(0.5,1);glVertex3f(0,-haut*n/6,-10*larg);	
	glTexCoord2f(1,0);glVertex3f(+1,0,0);
	
	glTexCoord2f(0,.0);glVertex3f(0,0,-1);
	glTexCoord2f(0.5,1);glVertex3f(10*larg,-haut*n/6,0);	
	glTexCoord2f(1,0);glVertex3f(0,0,+1);
	
	glTexCoord2f(0,.0);glVertex3f(0,0,-1);
	glTexCoord2f(0.5,1);glVertex3f(-10*larg,-haut*n/6,0);	
	glTexCoord2f(1,0);glVertex3f(0,0,+1);
    }
