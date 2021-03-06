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
	if (getPosition().y < 0.1f) temp_ += duration / (rand() % 30 + 15);

	if (temp_ >= 1) state_ = 1;
}

void Molecule::draw() {
    cyclone::Vector3 pos = getPosition();

	glColor3f( temp_, 0, 0 );

    glPushMatrix();
    glTranslatef( pos.x, pos.y, pos.z );
    glutSolidSphere( size_, 20, 10 );
    glPopMatrix();
}

// Accessors:

const cyclone::real Molecule::getSize() const {
	return size_;
}

const cyclone::real Molecule::getTemp() const {
	return temp_;
}

const unsigned Molecule::getState() const {
	return state_;
}

// Mutators:

void Molecule::setSize( cyclone::real s ) {
	size_ = s;
}

void Molecule::setTemp( cyclone::real t ) {
	if (t <= 1) temp_ = t;
}

void Molecule::setState( unsigned s ) {
	if (s == 1 || s == 0) state_ = s;
}
