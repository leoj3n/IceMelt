/* Molecule header file. */
#ifndef MOLECULE_H
#define MOLECULE_H

#include "../../../include/cyclone/particle.h"

class Molecule : public cyclone::Particle
{
	cyclone::real temp_; // temperature of the molecule
	unsigned state_;

public:
	Molecule() : temp_( 0 ), state_( 0 ) {}; // default constructor

	virtual void update( cyclone::real duration );

	void setTemp( cyclone::real t );
	cyclone::real getTemp();
	const cyclone::real getTemp() const;

	void setState( unsigned s );
	unsigned getState();
	const unsigned getState() const;

	Molecule& operator=(const Molecule &source) {
		if (this != &source) { // if not self
			temp_ = source.getTemp();
			state_ = source.getState();
		}

		return *this;
	}
};
#endif