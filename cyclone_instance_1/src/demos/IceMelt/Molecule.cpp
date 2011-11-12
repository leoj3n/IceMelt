#include "Molecule.h"
#include <cyclone/cyclone.h>

void Molecule::update( cyclone::real duration ) {
	integrate( duration );

	this->setTemp( this->getTemp() + (duration / 10) ); // temporary for testing color
}

void Molecule::setTemp( cyclone::real t ) {
	if (t <= 1) this->temp_ = t;
}

cyclone::real Molecule::getTemp() {
	return this->temp_;
}