/*-------------------------------------------------------------------------*
 *---																	---*
 *---	main.cpp														---*
 *---																	---*
 *---	This program simulates ice melting using Cyclone.				---*
 *---																	---*
 *---	----	----	----	----	----	----	----	----	---*
 *---																	---*
 *---	Version 1.0								2011 November 8			---*
 *---																	---*
 *---	Joel Kuczmarski			Dan Wojo		Derek Hearn				---*
 *---																	---*
 *-------------------------------------------------------------------------*/
/*
#if defined( _MSC_VER )
	#include <gl/glut.h>
#else
	#include <GLUT/glut.h>
#endif

#include <cyclone/cyclone.h>
#include "../app.h"
#include "../timing.h"

#include "Molecule.h" // molecule header file
//#include "Surface.h" // surface header file
// #define PARTICLE_COUNT 8

class IceMelt : public MassAggregateApplication
{
	cyclone::ParticleWorld world; // the world
	cyclone::Particle *molecules; // keep track of all molecules
	cyclone::GroundContacts groundContactGenerator;
	
	//[delete me?] cyclone::ParticleForceRegistry registry;

	// draws sphere
	void drawSphere( cyclone::Particle p );

public:
	// creates a new demo object
	IceMelt();
//	IceMelt(const int PARTICLE_COUNT);

	// returns the window title for the demo
	virtual const char* getTitle();

	// update the particle positions
	virtual void update();

	// display the particles
	virtual void display();
}; // class IceMelt

// method definitions:

// CONSTRUCTOR
//IceMelt::IceMelt()
//:
//MassAggregateApplication( 2 ),
//world( 20 ) {
//	particleArray[0].setPosition( 0, 0, 1 );
//    particleArray[1].setPosition( 0, 0, -1 );
//	
//    for( unsigned i = 0; i < 2; i++ ) {
//        particleArray[i].setMass( 100 );
//        particleArray[i].setVelocity( 0, 0, 0 );
//        particleArray[i].setDamping( 0.9f );
//        particleArray[i].setAcceleration( cyclone::Vector3::GRAVITY );
//        particleArray[i].clearAccumulator();
//    }
//
//	// springs
//	//registry.add( &particle1, new cyclone::ParticleSpring( &particle2, 0.50f, 2.0f ) );
//	//registry.add( &particle2, new cyclone::ParticleSpring( &particle1, 0.50f, 2.0f ) );
//}

IceMelt::IceMelt()
:
MassAggregateApplication( 2 ),
world( 20 ) {
	particleArray[0].setPosition( 0, 0, 1 );
    particleArray[1].setPosition( 0, 0, -1 );
	
    for( unsigned i = 0; i < 2; i++ ) {
        particleArray[i].setMass( 100 );
        particleArray[i].setVelocity( 0, 0, 0 );
        particleArray[i].setDamping( 0.9f );
        particleArray[i].setAcceleration( cyclone::Vector3::GRAVITY );
        particleArray[i].clearAccumulator();
    }

	// springs
	//registry.add( &particle1, new cyclone::ParticleSpring( &particle2, 0.50f, 2.0f ) );
	//registry.add( &particle2, new cyclone::ParticleSpring( &particle1, 0.50f, 2.0f ) );
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

//	registry.updateForces( duration );
//	particle1.integrate( duration );
//	particle2.integrate( duration );

    Application::update();
}

// DRAW
void IceMelt::display() {
	// Clear the viewport and set the camera direction
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    gluLookAt( 0.0, 0.0, 10.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );

	glColor3f( 0, 0, 1.0f );
	drawSphere( particleArray[0] );

	glColor3f( 1.0f, 0, 0 );
	drawSphere( particleArray[1] );
}

// draw sphere function
void IceMelt::drawSphere( cyclone::Particle p ) {
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

//Application* getApplication() {
//	return new IceMelt();
//}








#include "Molecule.h" // molecule header file

#if defined(_MSC_VER)
#include <gl/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <cyclone/cyclone.h>
#include "../app.h"
#include "../timing.h"

#include <stdio.h>
#include <cassert>

#define ROD_COUNT 15

#define BASE_MASS 1
#define EXTRA_MASS 10

/**
 * The main demo class definition.
 */
class PlatformDemo : public MassAggregateApplication
{
    cyclone::ParticleRod *rods;

    cyclone::Vector3 massPos;
    cyclone::Vector3 massDisplayPos;

    /**
     * Updates particle masses to take into account the mass
     * that's on the platform.
     */
    void updateAdditionalMass();

public:
    /** Creates a new demo object. */
    PlatformDemo();
	PlatformDemo(const int cubeDepth);
    virtual ~PlatformDemo();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Display the particles. */
    virtual void display();

    /** Update the particle positions. */
    virtual void update();

    /** Handle a key press. */
    virtual void key(unsigned char key);
};

// Method definitions
PlatformDemo::PlatformDemo()
:
MassAggregateApplication(6), rods(0),
massPos(0,0,0.5f)
{
    // Create the masses and connections.
    particleArray[0].setPosition(0,0,1);
    particleArray[1].setPosition(0,0,-1);
    particleArray[2].setPosition(-3,2,1);
    particleArray[3].setPosition(-3,2,-1);
    particleArray[4].setPosition(4,2,1);
    particleArray[5].setPosition(4,2,-1);
    for (unsigned i = 0; i < 6; i++)
    {
        particleArray[i].setMass(BASE_MASS);
        particleArray[i].setVelocity(0,0,0);
        particleArray[i].setDamping(0.9f);
        particleArray[i].setAcceleration(cyclone::Vector3::GRAVITY);
        particleArray[i].clearAccumulator();
    }

    rods = new cyclone::ParticleRod[ROD_COUNT];

    rods[0].particle[0] = &particleArray[0];
    rods[0].particle[1] = &particleArray[1];
    rods[0].length = 2;
    rods[1].particle[0] = &particleArray[2];
    rods[1].particle[1] = &particleArray[3];
    rods[1].length = 2;
    rods[2].particle[0] = &particleArray[4];
    rods[2].particle[1] = &particleArray[5];
    rods[2].length = 2;

    rods[3].particle[0] = &particleArray[2];
    rods[3].particle[1] = &particleArray[4];
    rods[3].length = 7;
    rods[4].particle[0] = &particleArray[3];
    rods[4].particle[1] = &particleArray[5];
    rods[4].length = 7;

    rods[5].particle[0] = &particleArray[0];
    rods[5].particle[1] = &particleArray[2];
    rods[5].length = 3.606;
    rods[6].particle[0] = &particleArray[1];
    rods[6].particle[1] = &particleArray[3];
    rods[6].length = 3.606;

    rods[7].particle[0] = &particleArray[0];
    rods[7].particle[1] = &particleArray[4];
    rods[7].length = 4.472;
    rods[8].particle[0] = &particleArray[1];
    rods[8].particle[1] = &particleArray[5];
    rods[8].length = 4.472;

    rods[9].particle[0] = &particleArray[0];
    rods[9].particle[1] = &particleArray[3];
    rods[9].length = 4.123;
    rods[10].particle[0] = &particleArray[2];
    rods[10].particle[1] = &particleArray[5];
    rods[10].length = 7.28;
    rods[11].particle[0] = &particleArray[4];
    rods[11].particle[1] = &particleArray[1];
    rods[11].length = 4.899;
    rods[12].particle[0] = &particleArray[1];
    rods[12].particle[1] = &particleArray[2];
    rods[12].length = 4.123;
    rods[13].particle[0] = &particleArray[3];
    rods[13].particle[1] = &particleArray[4];
    rods[13].length = 7.28;
    rods[14].particle[0] = &particleArray[5];
    rods[14].particle[1] = &particleArray[0];
    rods[14].length = 4.899;

    for (unsigned i = 0; i < ROD_COUNT; i++)
    {
        world.getContactGenerators().push_back(&rods[i]);
    }

    updateAdditionalMass();
}


// Modified for Ice
PlatformDemo::PlatformDemo(const int cubeDepth)
:MassAggregateApplication(cubeDepth*cubeDepth*cubeDepth), 
rods(0),
massPos(0,0,0.5f)
{
	const float ICE_DIST = 0.5f;
	const int NUM_PARTICLES = cubeDepth*cubeDepth*cubeDepth;
	const int NUM_RODS = 2*cubeDepth*cubeDepth*(cubeDepth-1)+cubeDepth*cubeDepth*(cubeDepth-1);


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

	rods = new cyclone::ParticleRod[NUM_RODS];

	count = 0;
	int rodNumber = 0;
	for(int i = 0; i < cubeDepth; i++)
	{
		for(int j = 0; j < cubeDepth; j++)
		{
			//single line . - . - .
			for(int k = 1; k < cubeDepth; k++)
			{
				rods[rodNumber].particle[0] = &particleArray[count];
				rods[rodNumber].particle[1] = &particleArray[count+1];
				rods[rodNumber].length = ICE_DIST;
				rodNumber++;
				count++;
			}
			/*After Horizontal Rods added */
			/*.  .  .
			|  |  |
			.  .  .*/
			if(j != cubeDepth-1){

				int x = 0;
				for(int k = cubeDepth; k > 0; k--)
				{
					rods[rodNumber].particle[count-k] = &particleArray[count-k];
					rods[rodNumber].particle[count+x] = &particleArray[count+x];
					rods[rodNumber].length = ICE_DIST;
					rodNumber++;
					x++;
				}

			}
		}
//		if(i != cubeDepth-1){
			//single line . - . - .
			for(int k = 1; k < cubeDepth; k++)
			{
				rods[rodNumber].particle[0] = &particleArray[count];
				rods[rodNumber].particle[1] = &particleArray[count+1];
				rods[rodNumber].length = ICE_DIST;
				rodNumber++;
				count++;
			}
			/*After Horizontal Rods added */
			/*.  .  .
			|  |  |
			.  .  .*/
			if(i != cubeDepth-1){

				int x = 0;
				for(int k = cubeDepth; k > 0; k--)
				{
					rods[rodNumber].particle[count-k] = &particleArray[count-k];
					rods[rodNumber].particle[count+x] = &particleArray[count+x];
					rods[rodNumber].length = ICE_DIST;
					rodNumber++;
					x++;
				}

//			}
			/*			int x = 0;
			for(int j = cubeDepth; j > 0; j--)
			{
			rods[rodNumber].particle[count-j] = &particleArray[count-j];
			rods[rodNumber].particle[count+x] = &particleArray[count+x];
			rods[rodNumber].length = ICE_DIST;
			rodNumber++;
			x++;
			} */
		}

	}

/*	for (int i = 0; i < NUM_RODS; i++)
    {
        world.getContactGenerators().push_back(&rods[i]);
    }
*/
//    updateAdditionalMass();
}

PlatformDemo::~PlatformDemo()
{
    if (rods) delete[] rods;
}

void PlatformDemo::updateAdditionalMass()
{
    for (unsigned i = 2; i < 6; i++)
    {
        particleArray[i].setMass(BASE_MASS);
    }

    // Find the coordinates of the mass as an index and proportion
    cyclone::real xp = massPos.x;
    if (xp < 0) xp = 0;
    if (xp > 1) xp = 1;

    cyclone::real zp = massPos.z;
    if (zp < 0) zp = 0;
    if (zp > 1) zp = 1;

    // Calculate where to draw the mass
    massDisplayPos.clear();

    // Add the proportion to the correct masses
    particleArray[2].setMass(BASE_MASS + EXTRA_MASS*(1-xp)*(1-zp));
    massDisplayPos.addScaledVector(
        particleArray[2].getPosition(), (1-xp)*(1-zp)
        );

    if (xp > 0)
    {
        particleArray[4].setMass(BASE_MASS + EXTRA_MASS*xp*(1-zp));
        massDisplayPos.addScaledVector(
            particleArray[4].getPosition(), xp*(1-zp)
            );

        if (zp > 0)
        {
            particleArray[5].setMass(BASE_MASS + EXTRA_MASS*xp*zp);
            massDisplayPos.addScaledVector(
                particleArray[5].getPosition(), xp*zp
                );
        }
    }
    if (zp > 0)
    {
        particleArray[3].setMass(BASE_MASS + EXTRA_MASS*(1-xp)*zp);
        massDisplayPos.addScaledVector(
            particleArray[3].getPosition(), (1-xp)*zp
            );
    }
}

void PlatformDemo::display()
{
    MassAggregateApplication::display();

    glBegin(GL_LINES);
    glColor3f(0,0,1);
/*    for (unsigned i = 0; i < ROD_COUNT; i++)
    {
        cyclone::Particle **particles = rods[i].particle;
        const cyclone::Vector3 &p0 = particles[0]->getPosition();
        const cyclone::Vector3 &p1 = particles[1]->getPosition();
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
    } */
    glEnd();

    glColor3f(1,0,0);
    glPushMatrix();
    glTranslatef(massDisplayPos.x, massDisplayPos.y+0.25f, massDisplayPos.z);
    glutSolidSphere(0.25f, 20, 10);
    glPopMatrix();
}

void PlatformDemo::update()
{
    MassAggregateApplication::update();

    updateAdditionalMass();
}

const char* PlatformDemo::getTitle()
{
    return "Cyclone > Platform Demo";
}

void PlatformDemo::key(unsigned char key)
{
    switch(key)
    {
    case 'w': case 'W':
        massPos.z += 0.1f;
        if (massPos.z > 1.0f) massPos.z = 1.0f;
        break;
    case 's': case 'S':
        massPos.z -= 0.1f;
        if (massPos.z < 0.0f) massPos.z = 0.0f;
        break;
    case 'a': case 'A':
        massPos.x -= 0.1f;
        if (massPos.x < 0.0f) massPos.x = 0.0f;
        break;
    case 'd': case 'D':
        massPos.x += 0.1f;
        if (massPos.x > 1.0f) massPos.x = 1.0f;
        break;

    default:
        MassAggregateApplication::key(key);
    }
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
    return new PlatformDemo(4);
}

