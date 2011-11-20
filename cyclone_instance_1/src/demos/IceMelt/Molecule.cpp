#include "IceMelt.h"

// Constructors:

// copy constructor
Molecule::Molecule( const Molecule &source ) {
	temp_ = source.temp_;
	state_ = source.state_;
	inverseMass = source.inverseMass;
	damping = source.damping;
	position = source.position;
	velocity = source.velocity;
	acceleration = source.acceleration;
}

// assignment operator
Molecule& Molecule::operator=( Molecule& source ) {
	if( this != &source ) {
		temp_ = source.temp_;
		state_ = source.state_;
		inverseMass = source.inverseMass;
		damping = source.damping;
		position = source.position;
		velocity = source.velocity;
		acceleration = source.acceleration;
		this->clearAccumulator();
	}
	return *this;
}

void Molecule::update( cyclone::real duration ) {
	temp_ += duration / (rand() % 50 + 1); // temporary hack for testing color
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

const cyclone::real Molecule::getTemp() const {
	return temp_;
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
