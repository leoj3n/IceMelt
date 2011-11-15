#include "IceMelt.h"

void Molecule::update( cyclone::real duration ) {
	temp_ += duration / 10; // temporary hack for testing color
	if (temp_ >= 1) state_ = 1;
}

void Molecule::draw() {
    cyclone::Vector3 pos = getPosition();

	glColor3f( temp_, 0, 0 );

    glPushMatrix();
    glTranslatef( pos.x, pos.y, pos.z );
    glutSolidSphere( 0.1f, 20, 10 );
    glPopMatrix();
}

// Accessors:

cyclone::real Molecule::getTemp() {
	return temp_;
}
const cyclone::real Molecule::getTemp() const {
	return temp_;
}

unsigned Molecule::getState() {
	return state_;
}
const unsigned Molecule::getState() const {
	return state_;
}

// Mutators:

void Molecule::setTemp( cyclone::real t ) {
	if (t <= 1) temp_ = t;
}

void Molecule::setState( unsigned s ) {
	if (s <= 1) state_ = s;
}
