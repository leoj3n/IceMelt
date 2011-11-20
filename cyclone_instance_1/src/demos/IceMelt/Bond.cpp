#include "IceMelt.h"

// Constructors:

// copy constructor
Bond::Bond( const Bond &source ) {
	temp_ = source.temp_;
	state_ = source.state_;
	particle[0] = source.particle[0];
	particle[1] = source.particle[1];
	length = source.length;
}

// assignment operator
Bond& Bond::operator=( Bond& source ) {
	if( this != &source ) {
		temp_ = source.temp_;
		state_ = source.state_;
		particle[0] = source.particle[0];
		particle[1] = source.particle[1];
		length = source.length;
	}
	return *this;
}

// Methods:

void Bond::update( cyclone::real duration ) {
	// checks to see if this link should be destroyed
	if (temp_ > 0.8f) state_ = 0; // temporary if-statement to test destruction

	// average the temperatures of the bonded molecules to get bond temperature
	temp_ = (particle[0]->getTemp() + particle[0]->getTemp()) / 2;
}

void Bond::draw() {
	const cyclone::Vector3 &p0 = particle[0]->getPosition();
	const cyclone::Vector3 &p1 = particle[1]->getPosition();

	glBegin( GL_LINES );
	glColor3f( 0, 0, temp_ ); // gets more blue depending on temperature of connected molecules
	glVertex3f( p0.x, p0.y, p0.z );
	glVertex3f( p1.x, p1.y, p1.z );
	glEnd();
}

// Accessors:

const cyclone::real Bond::getTemp() const {
	return temp_;
}

const unsigned Bond::getState() const {
	return state_;
}

// Mutators:

void Bond::setTemp( cyclone::real t ) {
	if (t <= 1) temp_ = t;
}

void Bond::setState( unsigned s ) {
	if (s <= 1) state_ = s;
}
