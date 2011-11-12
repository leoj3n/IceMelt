/* Molecule header file. */
#include "../../../include/cyclone/particle.h"

class Molecule : public cyclone::Particle
{
	cyclone::real temp_; // what temperature is the molecule
	unsigned state_;

public:
	Molecule() : temp_( 0 ), state_( 0 ) {}; // default constructor

	void update( cyclone::real duration );

	void setTemp( cyclone::real t );
	
	cyclone::real getTemp();
};
