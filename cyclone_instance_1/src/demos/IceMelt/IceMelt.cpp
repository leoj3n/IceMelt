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
#include "app.h" // includes all other .h files
#include "../timing.h"

#define BASE_MASS 1
#define ICE_DIST 0.5f
#define ICE_CROSS_DIST = sqrt( 2.0f ) * ICE_DIST

class IceMelt : public MassAggregateApplication
{
	Bond *rods_; // keep track of all rods

	const unsigned cubeDepth_;		//--> may need to make non-const for melting
	const unsigned rodCount_;		//--> may need to make non-const for melting
	const unsigned moleculeCount_;	//--> may need to make non-const for melting

	// for using space to reset the cube
	Molecule moleculeArrayOrig_;
	bool spaceFlag_;

	void drawSphere( Molecule &m ); // draws sphere

public:
	IceMelt();
	IceMelt( const unsigned depth );

	virtual void update(); // update the molecule positions
	virtual void display();
	virtual void key( unsigned char key );
	virtual const char* getTitle();
}; // class IceMelt

// DEFAULT CONSTRUCTOR
IceMelt::IceMelt() 
:
cubeDepth_( 0 ),
rodCount_( 0 ),
moleculeCount_( 0 ),
MassAggregateApplication( 0 ) {}

// CONSTRUCTOR
IceMelt::IceMelt( const unsigned depth ) 
:
cubeDepth_( depth ),
rodCount_( 2 * depth * depth * (depth - 1) + depth * depth * (depth - 1) ),
moleculeCount_( depth * depth * depth ),
MassAggregateApplication( depth * depth * depth ) {
	moleculeArrayOrig_ = moleculeArray;
	rods_ = new Bond[rodCount_];
	spaceFlag_ = false;

	const int SHEET = cubeDepth_ * cubeDepth_; // @TODO add comments about this variable name
	const int c = cubeDepth_ - 1;

	int count = 0;
	for( unsigned i = 0; i < cubeDepth_; i++ ) {
		for( unsigned j = 0; j < cubeDepth_; j++ ) {
			for( unsigned k = 0; k < cubeDepth_; k++ ) {
				moleculeArray[count].setPosition( ICE_DIST * k, ICE_DIST * i, ICE_DIST * j );
				moleculeArray[count].setMass( BASE_MASS );
				moleculeArray[count].setVelocity( 0, 0, 0 );
				moleculeArray[count].setDamping( 0.9f );
				moleculeArray[count].setAcceleration( cyclone::Vector3( 0, -1.0f, 0 ) ); // cyclone::Vector3::GRAVITY
				moleculeArray[count++].clearAccumulator();
			}
		}
	}

	count = 0;
	int rodNumber = 0;
	for( unsigned i = 0; i < cubeDepth_; i++ ) {
		for( unsigned j = 0; j < cubeDepth_; j++ ) {
			for( unsigned k = 1; k < cubeDepth_; k++ ) { // link all molecules in row
				rods_[rodNumber].particle[0] = &moleculeArray[count];
				rods_[rodNumber].particle[1] = &moleculeArray[count+1];
				rods_[rodNumber].length = ICE_DIST;
				rodNumber++;
				count++;
				if( rodNumber >= rodCount_ )
                {
                    break;
                }
			}

			if( j != cubeDepth_ - 1 ) {
				int x = 1;
				int temp = c;

				for( unsigned k = cubeDepth_; k > 0; k-- ) { // j connector
					rods_[rodNumber].particle[0] = &moleculeArray[count-temp];
					rods_[rodNumber].particle[1] = &moleculeArray[count+x];
					rods_[rodNumber].length = ICE_DIST;
					rodNumber++;
					if( rodNumber >= rodCount_ )
                    {
                        break;
                    }
                    x++;
                    temp--;
				}
				//back cross down
                for( k = 0; k < c; k++ ) {
                    //good works!
                    rods[rodNumber].particle[0] = &particleArray[count-k];
                    rods[rodNumber].particle[1] = &particleArray[count-k+c];
                    rods[rodNumber].length = ICE_CROSS_DIST;
                    rodNumber++;
                }
			}
			count++;
		}

		int p, q;

		if( i != cubeDepth_ - 1 ){
			p = 0;
			q = SHEET;

			for( unsigned j = 0; j < (unsigned)SHEET; j++ ) {
				rods_[rodNumber].particle[0] = &moleculeArray[count+p];
				rods_[rodNumber].particle[1] = &moleculeArray[count-q];
				rods_[rodNumber].length = ICE_DIST;
				rodNumber++;
				if (rodNumber >= rodCount) break;
				p++;
				q--;
			}

			for( int j = 0; j < SHEET; j++ ) {
                //crossing the fronts
                //not last in row
                if( j % cubeDepth ) {
                    //forward cross
                    rods[rodNumber].particle[0] = &particleArray[count-1-j];
                    rods[rodNumber].particle[1] = &particleArray[count-j+SHEET];
                    rods[rodNumber].length = ICE_CROSS_DIST;
                    rodNumber++;
                }

                const int rowMarker = j / cubeDepth_;

                //crossing the sides
                if( rowMarker > 0 ) {
                    rods[rodNumber].particle[0] = &particleArray[count-1-j];
                    rods[rodNumber].particle[1] = &particleArray[count-1-j+SHEET+cubeDepth];
                    rods[rodNumber].length = ICE_CROSS_DIST;
                    rodNumber++;
                }
            }
		}
	}

	for( unsigned i = 0; i < rodCount_; i++ ) {
        world.getContactGenerators().push_back( &rods_[i] );
    }
}

// UPDATE
void IceMelt::update() {
    world.startFrame(); // clear accumulators

	// find the duration of the last frame in seconds
	float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
	if (duration <= 0.0f) return;

	// update molecules
	for( unsigned i = 0; i < moleculeCount_; i++ ) {
		Molecule &m = moleculeArray[i];
		m.update( duration );
	}

	// run the simulation
    world.runPhysics( duration );

    Application::update();
}

// DRAW
void IceMelt::display() {
	// clear the viewport and set the camera direction
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	gluLookAt( 0.0, 0.0, 10.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );

	/*glColor3f( 0, 0, 1.0f );
	drawSphere( moleculeArray[0] );

	glColor3f( 1.0f, 0, 0 );
	drawSphere( moleculeArray[1] );*/

	glBegin( GL_LINES );
	glColor3f( 0, 0, 1 );
	for( unsigned i = 0; i < rodCount_; i++ ) {
		cyclone::Particle *(*particles) = rods_[i].particle;
		const cyclone::Vector3 &p0 = particles[0]->getPosition();
		const cyclone::Vector3 &p1 = particles[1]->getPosition();
		glVertex3f( p0.x, p0.y, p0.z );
		glVertex3f( p1.x, p1.y, p1.z );
	}
	glEnd();
	glPopMatrix();
}

// draw sphere function
void IceMelt::drawSphere( Molecule &m ) {
	glPushMatrix();
	cyclone::Vector3 pos = m.getPosition();
	glTranslatef( pos.x, pos.y, pos.z );
	glutSolidSphere( m.getMass() * 0.01f, 10, 10 );
	glPopMatrix();
}

// detect key press
void IceMelt::key( unsigned char key )
{
    switch( key ) {
		case ' ': // change molecule acceleration
			for( unsigned i = 0; i < moleculeCount_; i++ ) {
				Molecule &m = moleculeArray[i];

				if (spaceFlag_)
					m.setAcceleration( cyclone::Vector3( 0, -1.0f, 0 ) );
				else
					//m.setAcceleration( cyclone::Vector3( 0, ((i % 2) * 0.1f), 0 ) );
					m.setPosition( ICE_DIST * i, ICE_DIST * i, ICE_DIST * i );
			}
			spaceFlag_ = (spaceFlag_ ? false : true);
			break;
    }
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
	return new IceMelt( 3 ); // depth gets passed as an unsigned int
}
