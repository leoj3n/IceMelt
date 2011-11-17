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

#include "FreeImage/FreeImage.h"
#include <ShlObj.h> // for file creation
#include <stdio.h> // for sprintf_s

class IceMelt : public Application
{
	cyclone::ParticleWorld world_;
	cyclone::GroundContacts groundContactGenerator_;
    Molecule* molecules_;
	Bond* bonds_;

	const unsigned cubeDepth_;		//--> may need to make non-const for melting
	const unsigned bondCount_;		//--> may need to make non-const for melting
	const unsigned moleculeCount_;	//--> may need to make non-const for melting

	// variables used for reseting the cube
	Molecule* moleculesOrig_;
	bool spaceFlag_;

	// variables for changing duration
	float duration_;
	float forcedDuration_;
	bool zeroFlag_;

	// variables used for screenshots
	unsigned recID_;
	bool recording_;
	char path_[MAX_PATH];
	char file_[MAX_PATH];

	IceMelt(); // no default constructor
	IceMelt( const IceMelt& ); // no copy constructor
	IceMelt& operator=( const IceMelt& ); // no assignment operator

public:
	// Constructors:

	explicit IceMelt( const unsigned depth ); // constructor
	~IceMelt(); // destructor

	// Methods:

	virtual void initGraphics();
	virtual void update();
	virtual void display();
	virtual void key( unsigned char key );
	virtual const char* getTitle();
};

#endif