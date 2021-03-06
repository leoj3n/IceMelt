// Bond, James Bond.

#ifndef BOND_H
#define BOND_H

#include "IceMelt.h"

class Bond : public cyclone::ParticleRod<Molecule>
{
	cyclone::real conductivity_; // conductivity of the bond
	cyclone::real temp_; // temperature of the bond
	unsigned state_; // if 0, the world should delete this and remove any references to it

public:
	// Constructors:

	Bond() : conductivity_( 0.001f ), temp_( 0 ), state_( 1 ) {} // default constructor
	Bond( const Bond& source ); // copy constructor
	Bond& operator=( Bond& source ); // assignment operator
	~Bond() {} // destructor

	// Methods:

	void update( cyclone::real duration );
	void draw();

	// Accessors:

	const cyclone::real getConductivity() const;
	const cyclone::real getTemp() const;
	const unsigned getState() const;

	// Muatators:

	void setConductivity( cyclone::real c );
	void setTemp( cyclone::real t );
	void setState( unsigned s );
};

#endif