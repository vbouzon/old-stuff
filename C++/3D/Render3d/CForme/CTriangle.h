#ifndef CTRIANGLE_H
#define CTRIANGLE_H

#include <windows.h>
#include <GL/gl.h>
#include "Base.h"

class CTriangle
{
	CVertex *p1,*p2,*p3;

public:
	CTriangle(CVertex _p1, CVertex _p2, CVertex _p3);
	~CTriangle();

	void Render();
};

#endif

