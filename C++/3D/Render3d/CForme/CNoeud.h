#ifndef	CNOEUD_H
#define	CNOEUD_H

#include "CTriangle.h"

class CNoeud
{
	float tx,ty,tz,rx,ry,rz;
	CTriangle *Entity;

public:
	CNoeud(float _x,float _y, float _z);

	void Translate(float _x,float _y, float _z);
	void Rotate(float _x,float _y, float _z);
	void SetEntity(CTriangle *_Entity);
	void Render();
};

#endif