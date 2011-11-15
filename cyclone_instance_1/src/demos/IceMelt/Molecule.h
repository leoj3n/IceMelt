#ifndef MOLECULE_H
#define MOLECULE_H

#include "IceMelt.h"

class Molecule : public cyclone::Particle
{
	cyclone::real temp_; // temperature of the molecule
	unsigned state_;

public:
	Molecule() : temp_( 0 ), state_( 0 ) {}; // default constructor

	void update( cyclone::real duration );

	void draw();

	// Accessors:

	cyclone::real getTemp();
	const cyclone::real getTemp() const;

	unsigned getState();
	const unsigned getState() const;

	// Muatators:

	void setTemp( cyclone::real t );
	void setState( unsigned s );

	// Operators:

	Molecule& operator=(const Molecule &source) {
		if (this != &source) { // if not self
			temp_ = source.getTemp();
			state_ = source.getState();
		}

		return *this;
	}
};

#endif