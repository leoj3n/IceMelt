/*-------------------------------------------------------------------------*
 *---																	---*
 *---	IceMelt.cpp														---*
 *---																	---*
 *---	This program simulates ice melting using Cyclone.				---*
 *---																	---*
 *---	----	----	----	----	----	----	----	----	---*
 *---																	---*
 *---	Version 1.0								2011 November 15		---*
 *---																	---*
 *---	Joel Kuczmarski			Dan Wojo		Derek Hearn				---*
 *---																	---*
 *-------------------------------------------------------------------------*/

#include "IceMelt.h" // includes all other .h files

#define BASE_MASS 1
#define ICE_DIST 0.5f
#define ICE_CROSS_DIST sqrt( 2.0f ) * ICE_DIST

// DEFAULT CONSTRUCTOR
IceMelt::IceMelt() 
:
cubeDepth_( 0 ),
bondCount_( 0 ),
moleculeCount_( 0 ),
world_( 0 ) {}

// CONSTRUCTOR
IceMelt::IceMelt( const unsigned depth ) 
:
cubeDepth_( depth ),
bondCount_( 2 * cubeDepth_ * cubeDepth_ * (cubeDepth_ - 1) 
			+ cubeDepth_ * cubeDepth_ * (cubeDepth_ - 1) 
			+ 3 * cubeDepth_ * (cubeDepth_ - 1) * (cubeDepth_ - 1) ),
moleculeCount_( depth * depth * depth ),
world_( (depth * depth * depth) * 10 ) {
	molecules_ = new Molecule[moleculeCount_];
    for( unsigned i = 0; i < moleculeCount_; i++ ) {
        world_.getParticles().push_back( molecules_ + i );
    }
    groundContactGenerator_.init( &world_.getParticles() );
    world_.getContactGenerators().push_back( &groundContactGenerator_ );

	//moleculesOrig_ = molecules_; // needs assingment operator and copy constructor
	bonds_ = new Bond[bondCount_];
	spaceFlag_ = false;

	const unsigned SHEET = cubeDepth_ * cubeDepth_; // @TODO add comments about this variable name
	const unsigned c = cubeDepth_ - 1;

	int count = 0;
	for( unsigned i = 0; i < cubeDepth_; i++ ) {
		for( unsigned j = 0; j < cubeDepth_; j++ ) {
			for( unsigned k = 0; k < cubeDepth_; k++ ) {
				molecules_[count].setPosition( ICE_DIST * k, ICE_DIST * i, ICE_DIST * j );
				molecules_[count].setMass( BASE_MASS );
				molecules_[count].setVelocity( 0, 0, 0 );
				molecules_[count].setDamping( 0.9f );
				molecules_[count].setAcceleration( cyclone::Vector3::GRAVITY );
				molecules_[count++].clearAccumulator();
			}
		}
	}

	count = 0;
	unsigned rodNumber = 0;
	for( unsigned i = 0; i < cubeDepth_; i++ ) {
		for( unsigned j = 0; j < cubeDepth_; j++ ) {
			for( unsigned k = 1; k < cubeDepth_; k++ ) {
				// link all molecules in row
				bonds_[rodNumber].particle[0] = &molecules_[count];
				bonds_[rodNumber].particle[1] = &molecules_[count+1];
				bonds_[rodNumber].length = ICE_DIST;
				rodNumber++;
				count++;
				if (rodNumber >= bondCount_) break;
			}

			if( j != cubeDepth_ - 1 ) {
				unsigned x = 1;
				int y = c;

				// j connector
				for( unsigned k = cubeDepth_; k > 0; k-- ) {
					bonds_[rodNumber].particle[0] = &molecules_[count-y];
					bonds_[rodNumber].particle[1] = &molecules_[count+x];
					bonds_[rodNumber].length = ICE_DIST;
					rodNumber++;
					if (rodNumber >= bondCount_) break;
                    x++;
                    y--;
				}

				// back cross down
                for( unsigned k = 0; k < c; k++ ) {
                    bonds_[rodNumber].particle[0] = &molecules_[count-k];
                    bonds_[rodNumber].particle[1] = &molecules_[count-k+c];
                    bonds_[rodNumber].length = ICE_CROSS_DIST;
                    rodNumber++;
                }
			}
			count++;
		}

		int p, q;

		if( i != cubeDepth_ - 1 ) {
			p = 0;
			q = SHEET;

			for( unsigned j = 0; j < SHEET; j++ ) {
				bonds_[rodNumber].particle[0] = &molecules_[count+p];
				bonds_[rodNumber].particle[1] = &molecules_[count-q];
				bonds_[rodNumber].length = ICE_DIST;
				rodNumber++;
				if (rodNumber >= bondCount_) break;
				p++;
				q--;
			}

			for( unsigned j = 0; j < SHEET; j++ ) { // crossing the fronts, not last in row
                if( j % cubeDepth_ ) {
                    // forward cross
                    bonds_[rodNumber].particle[0] = &molecules_[count-1-j];
                    bonds_[rodNumber].particle[1] = &molecules_[count-j+SHEET];
                    bonds_[rodNumber].length = ICE_CROSS_DIST;
                    rodNumber++;
                }

                // crossing the sides
                if( (j / cubeDepth_) > 0 ) {
                    bonds_[rodNumber].particle[0] = &molecules_[count-1-j];
                    bonds_[rodNumber].particle[1] = &molecules_[count-1-j+SHEET+cubeDepth_];
                    bonds_[rodNumber].length = ICE_CROSS_DIST;
                    rodNumber++;
                }
            }
		}
	}

	for( unsigned i = 0; i < bondCount_; i++ ) {
        world_.getContactGenerators().push_back( &bonds_[i] );
    }
}

// DESTRUCTOR
IceMelt::~IceMelt() {
    if (bonds_) delete[] bonds_;
	delete[] molecules_;
}

// UPDATE
void IceMelt::update() {
	// Clear accumulators
    world_.startFrame();
    // Find the duration of the last frame in seconds
    float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
    if (duration <= 0.0f) return;
    // Run the simulation
    world_.runPhysics( duration );
    Application::update();

	// update molecules
	for( unsigned i = 0; i < moleculeCount_; i++ ) {
		molecules_[i].update( duration );
	}

	// update bonds
	for( unsigned i = 0; i < bondCount_; i++ ) {
		bonds_[i].update( duration );
	}
}

// DRAW
void IceMelt::display() {
	// Clear the view port and set the camera direction
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    gluLookAt( 0.0, 3.5, 8.0,  0.0, 3.5, 0.0,  0.0, 1.0, 0.0 );

	// draw the molecules
	for (unsigned i = 0; i < moleculeCount_; i++) molecules_[i].draw();

	// draw the bonds
	for (unsigned i = 0; i < bondCount_; i++) bonds_[i].draw();
}

// detect key press
void IceMelt::key( unsigned char key ) {
    switch( key ) {
		case ' ': // spacebar: change molecule acceleration (press once, don't press and hold)
			for( unsigned i = 0; i < moleculeCount_; i++ ) {
				Molecule &m = molecules_[i];

				if (spaceFlag_)
					m.setAcceleration( cyclone::Vector3::GRAVITY );
				else
					m.setAcceleration( cyclone::Vector3( 0, (i % 2), 0 ) ); // mod causes a small amount of tilt
			}
			spaceFlag_ = (spaceFlag_ ? false : true);
			break;
    }
}

// returns program title
const char* IceMelt::getTitle() {
	return "Cyclone > Ice Melt";
}

// create an application object
Application* getApplication() {
	return new IceMelt( 3 ); // depth gets passed as an unsigned int
}
