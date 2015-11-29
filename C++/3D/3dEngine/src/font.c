// this file load all the textures in memory
// and create a function that permit writing font using font.bmp
#include "font.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


void BuildFont(unsigned int base,unsigned int texturebmp)
{
    unsigned int loop; /* Loop variable               */
    float cx;    /* Holds Our X Character Coord */
    float cy;    /* Holds Our Y Character Coord */

    
    base  = glGenLists( 256 );
    //glBindTexture( GL_TEXTURE_2D, texturebmp );
    for ( loop = 0; loop < 256; loop++ )
        {
	    

	    /* X Position Of Current Character */
	    cx = 1 - ( float )( loop-1 % 16 ) / 16;
	    /* Y Position Of Current Character */
	    cy = 1 - ( float )( loop / 16 ) / 16;
	    if((float)(loop%16)==0)cy+=(float)1/16;
	    
		
            /* Start Building A List */
	    glNewList( base + ( 255 - loop ), GL_COMPILE );
	      
	      glBegin( GL_QUADS ); 
       glTexCoord2f( cx - 0.0625, cy );
		glVertex2i( 0, 0 );

		glTexCoord2f( cx, cy );
		glVertex2i( 8, 0 );

		glTexCoord2f( cx, cy - 0.0625 );
		glVertex2i( 8, 8 );
		
		glTexCoord2f( cx - 0.0625, cy - 0.0625);
		glVertex2i( 0, 8 );
	      glEnd( );
	      glTranslated( 5, 0, 0 );
	    glEndList( );
        }
}


void glPrint( int x, int y, char *string, int set ,unsigned int base,unsigned int texturebmp)
{int i;
    if(set>1)set = 0;
    glBindTexture( GL_TEXTURE_2D, texturebmp );
    glDisable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION );
    glPushMatrix( );
    glLoadIdentity( );
    glOrtho( 0, 512, 0, 512, -1, 1 );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix( );
    glLoadIdentity( );
    glTranslated( x, y, 1 );
    glListBase( base - 32 + ( 128 * set ) );
    glCallLists( strlen( string ), GL_BYTE, string );
    glMatrixMode( GL_PROJECTION );
    glPopMatrix( );
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix( );
    glEnable( GL_DEPTH_TEST );
}








