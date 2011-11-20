#ifndef ICEMELT_H
#define ICEMELT_H

#if defined( _MSC_VER )
	#include <gl/glut.h>
#else
	#include <GLUT/glut.h>
#endif

#include "cyclone/app.h"
#include "../timing.h"

#include "cyclone/plinks.h" // uses templates
#include "cyclone/pworld.h" // uses templates

#include "Molecule.h" // extends Particle
#include "Bond.h" // extends Rod

#include "FreeImage/FreeImage.h"
#include <ShlObj.h> // for file creation
#include <stdio.h> // for sprintf_s

class IceMelt : public Application
{
	cyclone::ParticleWorld<Molecule> world_;
	cyclone::GroundContacts<Molecule> groundContactGenerator_;
	Molecule* molecules_;
	Bond* bonds_;

	const unsigned cubeDepth_; // only used in constructor
	// bonds and molecules don't get deleted,
	// instead they have their state set to 0 so they can be reused later.
	const unsigned bondCount_;
	const unsigned moleculeCount_;

	// variables used for reseting the cube
	cyclone::ParticleWorld<Molecule>::ContactGenerators origContactGenerators_;
	Molecule* moleculesOrig_;
	Bond* bondsOrig_;
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