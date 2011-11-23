#ifndef MOLECULE_H
#define MOLECULE_H

#include "IceMelt.h"

class Molecule : public cyclone::Particle
{
	cyclone::real size_; // draw size of the molecule
	cyclone::real temp_; // temperature of the molecule
	unsigned state_;

public:
	// Constructors:

	Molecule() : temp_( 0 ), state_( 1 ) {} // default constructor
	Molecule( const Molecule& source ); // copy constructor
	Molecule& operator=( Molecule& source ); // assignment operator
	~Molecule() {} // destructor

	// Methods:

	void update( cyclone::real duration );
	void draw();

	// Accessors:

	const cyclone::real getSize() const;
	const cyclone::real getTemp() const;
	const unsigned getState() const;

	// Muatators:

	void setSize( cyclone::real s );
	void setTemp( cyclone::real t );
	void setState( unsigned s );
};

#endif