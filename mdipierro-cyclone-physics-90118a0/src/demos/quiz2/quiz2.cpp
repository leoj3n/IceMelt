/*
 * Quiz 2 program
 */

#if defined( _MSC_VER )
#include <gl/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <cyclone/cyclone.h>
#include "../app.h"
#include "../timing.h"

class Quiz2 : public Application
{
	cyclone::Particle particle1;
	cyclone::Particle particle2;

	cyclone::ParticleForceRegistry registry;

	// draws sphere
	void drawSphere( cyclone::Vector3 pos );

public:
	// creates a new demo object
	Quiz2();

	// returns the window title for the demo
	virtual const char* getTitle();

	// update the particle positions
	virtual void update();

	// display the particles
	virtual void display();
}; // class Quiz2

// method definitions:

// CONSTRUCTOR
Quiz2::Quiz2() {
	// first particle
	particle1.setMass( 100 );
	particle1.setPosition( -10.0f, 0, 0 );
	particle1.setDamping( 1 );

	// second particle
	particle2.setMass( 100 );
	particle2.setPosition( 10.0f, 0, 0 );
	particle2.setDamping( 1 );

	// springs
	registry.add( &particle1, new cyclone::ParticleSpring( &particle2, 0.01f, 15.0f ) );
	registry.add( &particle2, new cyclone::ParticleSpring( &particle1, 0.01f, 15.0f ) );
}

// UPDATE
void Quiz2::update() {
	// Find the duration of the last frame in seconds
	float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
	if (duration <= 0.0f) return;

	registry.updateForces( duration );
	particle1.integrate( duration );
	particle2.integrate( duration );

	Application::update();
}

// DRAW
void Quiz2::display() {
	// Clear the viewport and set the camera direction
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    gluLookAt( 0.0, 0.0, 10.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );

	glColor3f( 0, 0, 1.0f );
	drawSphere( particle1.getPosition() );

	glColor3f( 1.0f, 0, 0 );
	drawSphere( particle2.getPosition() );
}

// draw sphere function
void Quiz2::drawSphere( cyclone::Vector3 pos ) {
	glPushMatrix();
	glTranslatef( pos.x, pos.y, pos.z );
	glutSolidSphere( 0.25f, 20, 10 );
	glPopMatrix();
}

// returns program title
const char* Quiz2::getTitle() {
	return "Cyclone > Quiz 2";
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication() {
	return new Quiz2();
}
