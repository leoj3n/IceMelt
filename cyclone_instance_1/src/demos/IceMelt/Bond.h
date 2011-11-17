#ifndef BOND_H
#define BOND_H

#include "IceMelt.h"

class Bond : public cyclone::ParticleRod
{
	Bond( const Bond& ); // no copy constructor
	Bond& operator=( const Bond& ); // no assignment operator

public:
	// Constructors:

	Bond() {}; // default constructor
	~Bond() {}; // destructor

	// Methods:

	void update( cyclone::real duration );
	void draw();
};

#endif