#include "CTriangle.h"

CTriangle::CTriangle(CVertex _p1, CVertex _p2, CVertex _p3)
{
	p1 = new CVertex(_p1.x,_p1.y,_p1.z);
	p2 = new CVertex(_p2.x,_p2.y,_p2.z);
	p3 = new CVertex(_p3.x,_p3.y,_p3.z);
}
void CTriangle::Render()
{
	glBegin( GL_TRIANGLES );
	glVertex3f( p1->x,p1->y,p1->z );
	glVertex3f( p2->x,p2->y,p2->z );
	glVertex3f( p3->x,p3->y,p3->z );
	glEnd( );
}
