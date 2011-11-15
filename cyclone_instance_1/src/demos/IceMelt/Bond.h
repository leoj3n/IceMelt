#ifndef BOND_H
#define BOND_H

#include "IceMelt.h"

class Bond : public cyclone::ParticleRod
{
private:

public:
	void update( cyclone::real duration );
	void draw();
};

#endif