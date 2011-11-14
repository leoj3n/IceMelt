#include <cyclone/cyclone.h>
#include "Molecule.h"

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
const cyclone::real Molecule::getTemp() const {
	return this->temp_;
}

void Molecule::setState( unsigned s ) {
	if (s <= 1) this->state_ = s;
}
unsigned Molecule::getState() {
	return this->state_;
}
const unsigned Molecule::getState() const {
	return this->state_;
}