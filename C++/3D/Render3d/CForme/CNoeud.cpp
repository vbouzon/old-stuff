#include "CNoeud.h"

CNoeud::CNoeud(float _x,float _y, float _z)
{
	tx = _x;
	ty = _y;
	tz = _z;
	rx = 0;
	ry = 0;
	rz = 0;
}
void CNoeud::Rotate(float _x, float _y, float _z)
{
	rx += _x;
	ry += _y;
	rz += _z;
}
void CNoeud::Translate(float _x, float _y, float _z)
{
	tx += _x;
	ty += _y;
	tz += _z;
}
void CNoeud::Render()
{
	glPushMatrix();

	glTranslatef(tx,ty,tz);

	glRotatef(rx,1,0,0);
	glRotatef(ry,0,1,0);
	glRotatef(rz,0,0,1);

	Entity->Render();

	glPopMatrix();
}
void CNoeud::SetEntity(CTriangle *_Entity)
{
	Entity = _Entity;
}

