#ifndef CSTATE_H
#define CSTATE_H

class CState
{
public:

	virtual void Init() = 0;
	virtual void Update(float time) = 0;
	virtual void UpdateInput(float time) = 0;
};

#endif
