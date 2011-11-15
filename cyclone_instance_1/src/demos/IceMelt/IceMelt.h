#ifndef ICEMELT_H
#define ICEMELT_H

#if defined( _MSC_VER )
	#include <gl/glut.h>
#else
	#include <GLUT/glut.h>
#endif

#include "../app.h"
#include "../timing.h"

#include "Molecule.h" // extends Particle
#include "Bond.h" // extends Rod

class IceMelt : public Application
{
	cyclone::ParticleWorld world_;
	cyclone::GroundContacts groundContactGenerator_;
    Molecule *molecules_; // keep track of all molecules
	Bond *bonds_; // keep track of all rods

	const unsigned cubeDepth_;		//--> may need to make non-const for melting
	const unsigned bondCount_;		//--> may need to make non-const for melting
	const unsigned moleculeCount_;	//--> may need to make non-const for melting

	// for using space to reset the cube
	Molecule moleculesOrig_;
	bool spaceFlag_;

public:
	IceMelt();
	IceMelt( const unsigned depth );
	virtual ~IceMelt();
	virtual void update(); // update the molecule positions
	virtual void display();
	virtual void key( unsigned char key );
	virtual const char* getTitle();
}; // class IceMelt

#endif