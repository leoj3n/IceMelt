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

#include "bond.h"	// new Rod class	-> Includes Molecule.h
//#include "Surface.h" // surface header file

#define BASE_MASS 1

class IceMelt : public MassAggregateApplication
{
	bond *rods;
	Molecule *molecules; // keep track of all molecules

	// draws sphere
	void drawSphere( cyclone::Particle &p );

public:
	// creates a new demo object
	IceMelt();
	IceMelt(const int cubeDepth);

	int numRods;
	void setRodNum(const int newNum);

	// returns the window title for the demo
	virtual const char* getTitle();

	// update the particle positions
	virtual void update();

	// display the particles
	virtual void display();
}; // class IceMelt

// method definitions:
IceMelt::IceMelt() 
:MassAggregateApplication(0), 
rods(0)
{
}
// CONSTRUCTOR
IceMelt::IceMelt(const int cubeDepth) 
:MassAggregateApplication(cubeDepth*cubeDepth*cubeDepth), 
rods(0)
{
	const float ICE_DIST = 0.5f;
	const int NUM_PARTICLES = cubeDepth*cubeDepth*cubeDepth;
	int NUM_RODS = 2*cubeDepth*cubeDepth*(cubeDepth-1)+cubeDepth*cubeDepth*(cubeDepth-1);

	int count = 0;
	for(int i = 0; i < cubeDepth; i++)
	{
		for(int j = 0; j < cubeDepth; j++)
		{
			for(int k = 0; k < cubeDepth; k++)
			{
				particleArray[count].setPosition(ICE_DIST*i,ICE_DIST*j,ICE_DIST*k);
				particleArray[count].setMass(BASE_MASS);
			
	particleArray[count].setVelocity(0,0,0);
				particleArray[count].setDamping(0.9f);
				particleArray[count].setAcceleration(cyclone::Vector3::GRAVITY);
				particleArray[count++].clearAccumulator();
			}
		}
	}

	rods = new bond[NUM_RODS];
	this->numRods = numRods;

	count = 0;
	int rodNumber = 0;
	for(int i = 0; i < cubeDepth; i++)
	{
		for(int j = 0; j < cubeDepth; j++)
		{
			for(int k = 1; k < cubeDepth; k++)
			{
				rods[rodNumber].particle[0] = &particleArray[count];
				rods[rodNumber].particle[1] = &particleArray[count+1];
				rods[rodNumber].length = ICE_DIST;
				rodNumber++;
				count++;
			}

			if(j != cubeDepth-1)
			{
				int x = 1;
				int c = cubeDepth-1;
				for(int k = cubeDepth; k > 0; k--)
				{

					rods[rodNumber].particle[0] = &particleArray[count-c];
					rods[rodNumber].particle[1] = &particleArray[count+x];
					rods[rodNumber].length = ICE_DIST;
					rodNumber++;
					x++;
					c--;
				}
			}
			count++;
		}

		int SHEET = cubeDepth*cubeDepth;
		int p, q;

		if(i != cubeDepth-1)
		{
			p = 0;
			q = SHEET;

			for(int j = 0; j < SHEET; j++)
			{
				rods[rodNumber].particle[0] = &particleArray[count+p];
				rods[rodNumber].particle[1] = &particleArray[count-q];
				rods[rodNumber].length = ICE_DIST;
				rodNumber++;
				p++;
				q--;
			}
		}
	}

	for (unsigned i = 0; i < NUM_RODS; i++)
    {
        world.getContactGenerators().push_back(&rods[i]);
    }

//MassAggregateApplication( 2 ) {
//	particleArray[0].setPosition( 10, 10, 0 );
//   particleArray[1].setPosition( 0, 8, 0 );
	
/*    for( unsigned i = 0; i < 2; i++ ) {
        particleArray[i].setMass( 100 );
        particleArray[i].setVelocity( 0, 0, 0 );
        particleArray[i].setDamping( 0.9f );
        particleArray[i].setAcceleration( cyclone::Vector3::GRAVITY );
        particleArray[i].clearAccumulator();
    }*/
}

// UPDATE
void IceMelt::update() {
	// Clear accumulators
    world.startFrame();

	// Find the duration of the last frame in seconds
	float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
	if (duration <= 0.0f) return;

	// Run the simulation
    world.runPhysics( duration );

    Application::update();
}

// DRAW
void IceMelt::display() {
	// Clear the viewport and set the camera direction
 /*   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    gluLookAt( 0.0, 0.0, 10.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );

	glColor3f( 0, 0, 1.0f );
	drawSphere( particleArray[0] );

	glColor3f( 1.0f, 0, 0 );
	drawSphere( particleArray[1] );*/

	    glBegin(GL_LINES);
    glColor3f(0,0,1);
	for (unsigned i = 0; i < this->numRods; i++)
	{
		cyclone::Particle **particles = rods[i].particle;
		const cyclone::Vector3 &p0 = particles[0]->getPosition();
		const cyclone::Vector3 &p1 = particles[1]->getPosition();
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
	}
    glEnd();
    glPopMatrix();
}

// draw sphere function
void IceMelt::drawSphere( cyclone::Particle &p ) {
	glPushMatrix();
	cyclone::Vector3 pos = p.getPosition();
	glTranslatef( pos.x, pos.y, pos.z );
	glutSolidSphere( (p.getMass() / 4) * 0.01f, 10, 10 );
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
