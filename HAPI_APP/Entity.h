#pragma once

// Include the HAPI Sprites header to get access to all of the HAPI Sprites interface
#include <HAPISprites_lib.h>

// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

class CEntity
{
public:
	CEntity();
	~CEntity();
	Sprite* getSprite() { return _sprite; }
private:

	virtual void update() = 0;
	Sprite* _sprite;
	Point pos;
	unsigned int frameNum = 0;
	float angle = 0.0f;


};
