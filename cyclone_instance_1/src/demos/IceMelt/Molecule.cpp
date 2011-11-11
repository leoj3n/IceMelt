#include "Molecule.h";

bool Molecule::update( cyclone::real duration ) {
	integrate( duration );

	if (temp > 100.0f)
		state = 1;
	else
		state = 0;
}
