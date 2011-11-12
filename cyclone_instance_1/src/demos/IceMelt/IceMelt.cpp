/*-------------------------------------------------------------------------*
 *---																	---*
 *---	IceMelt.cpp														---*
 *---																	---*
 *---	This program simulates ice melting using Cyclone.				---*
 *---																	---*
 *---	----	----	----	----	----	----	----	----	---*
 *---																	---*
 *---	Version 1.0								2011 November 11		---*
 *---																	---*
 *---	Joel Kuczmarski			Dan Wojo		Derek Hearn				---*
 *---																	---*
 *-------------------------------------------------------------------------*/

#if defined( _MSC_VER )
	#include <gl/glut.h>
#else
	#include <GLUT/glut.h>
#endif

#include <cyclone/cyclone.h>
#include "app.h"
#include "../timing.h"

#define NUM_MOLECULES 2

class IceMelt : public MassAggregateApplication
{
	//cyclone::ParticleRod *rods;
	Molecule *molecules; // keep track of all molecules

	// draws sphere
	void drawSphere( Molecule &m );

public:
	// creates a new demo object
	IceMelt();

	// returns the window title for the demo
	virtual const char* getTitle();

	// update the particle positions
	virtual void update();

	// display the particles
	virtual void display();
}; // class IceMelt

// method definitions:

// CONSTRUCTOR
IceMelt::IceMelt()
:
MassAggregateApplication( 2 ) {
	particleArray[0].setPosition( 10, 10, 0 );
    particleArray[1].setPosition( 0, 8, 0 );
	
	// setup molecules
    for( unsigned i = 0; i < NUM_MOLECULES; i++ ) {
		Molecule &m = particleArray[i];
        m.setMass( 100 );
        m.setVelocity( 0, 0, 0 );
        m.setDamping( 0.9f );
        m.setAcceleration( cyclone::Vector3::GRAVITY );
        m.clearAccumulator();
    }
}

// UPDATE
void IceMelt::update() {
	// Clear accumulators
    world.startFrame();

	// Find the duration of the last frame in seconds
	float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
	if (duration <= 0.0f) return;

	// update molecules
	for( unsigned i = 0; i < NUM_MOLECULES; i++ ) {
		Molecule &m = particleArray[i];
		m.update( duration );
	}

	// Run the simulation
    world.runPhysics( duration );

    Application::update();
}

// DRAW
void IceMelt::display() {
	// Clear the viewport and set the camera direction
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    gluLookAt( 0.0, 0.0, 10.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );

	for( unsigned i = 0; i < NUM_MOLECULES; i++ ) {
		Molecule &m = particleArray[i];
		glColor3f( m.getTemp(), 0, 0 );
		drawSphere( m );
	}
}

// draw sphere function
void IceMelt::drawSphere( Molecule &m ) {
	glPushMatrix();
	cyclone::Vector3 pos = m.getPosition();
	glTranslatef( pos.x, pos.y, pos.z );
	glutSolidSphere( (m.getMass() / 4) * 0.01f, 10, 10 );
	glPopMatrix();
}

// returns program title
const char* IceMelt::getTitle() {
	return "Cyclone > Ice Melt";
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication() {
	return new IceMelt();
}
