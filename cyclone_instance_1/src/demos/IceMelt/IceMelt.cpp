/*-------------------------------------------------------------------------*
 *---																	---*
 *---	IceMelt.cpp														---*
 *---																	---*
 *---	This program simulates ice melting using Cyclone.				---*
 *---																	---*
 *---	----	----	----	----	----	----	----	----	---*
 *---																	---*
 *---	Version 2.2								2011 November 20		---*
 *---																	---*
 *---	Joel Kuczmarski			Dan Wojo		Derek Hearn				---*
 *---																	---*
 *-------------------------------------------------------------------------*/

#include "IceMelt.h" // includes all other .h files

#define RECORD_AT_START false
#define FORCED_AT_START false
#define FORCED_DURATION 0.016f

#define BASE_MASS 1
#define ICE_DIST 0.8f
#define ICE_CROSS_DIST sqrt( 2.0f ) * ICE_DIST

// CONSTRUCTOR
IceMelt::IceMelt( const unsigned depth ) 
:
cubeDepth_( depth ),
bondCount_( 2 * cubeDepth_ * cubeDepth_ * (cubeDepth_ - 1) 
		+ cubeDepth_ * cubeDepth_ * (cubeDepth_ - 1) 
		+ 3 * cubeDepth_ * (cubeDepth_ - 1) * (cubeDepth_ - 1) ),
moleculeCount_( depth * depth * depth ),
world_( (depth * depth * depth) * 10 ) {
	// initialize non-const variables
	molecules_ = new Molecule[moleculeCount_];
	moleculesOrig_ = new Molecule[moleculeCount_];
	bonds_ = new Bond[bondCount_];
	bondsOrig_ = new Bond[bondCount_];
	spaceFlag_ = false;
	zeroFlag_ = FORCED_AT_START;
	recording_ = RECORD_AT_START;
	forcedDuration_ = FORCED_DURATION;

	SHGetSpecialFolderPath( 0, path_, CSIDL_DESKTOPDIRECTORY, false ); // set path_ to desktop path
	sprintf_s( path_, "%s\\IceMelt_Render", path_ ); // add to path_

	const unsigned SHEET = cubeDepth_ * cubeDepth_; // @TODO add comments about this variable name
	const unsigned c = cubeDepth_ - 1;

	// add molecules to world
	for (unsigned i = 0; i < moleculeCount_; i++) world_.getParticles().push_back( molecules_ + i );
	groundContactGenerator_.init( &world_.getParticles() );
	world_.getContactGenerators().push_back( &groundContactGenerator_ );

	int count = 0;
	for( unsigned i = 0; i < cubeDepth_; i++ ) {
		for( unsigned j = 0; j < cubeDepth_; j++ ) {
			for( unsigned k = 0; k < cubeDepth_; k++ ) {
				molecules_[count].setPosition( ICE_DIST * k, ICE_DIST * i, ICE_DIST * j );
				molecules_[count].setMass( BASE_MASS );
				molecules_[count].setVelocity( 0, 0, 0 );
				molecules_[count].setDamping( 0.9f );
				molecules_[count].setAcceleration( cyclone::Vector3( 0, -1.0f, 0 ) ); //cyclone::Vector3::GRAVITY
				molecules_[count++].clearAccumulator();
			}
		}
	}

	for (unsigned i = 0; i < moleculeCount_; i++)
		moleculesOrig_[i] = molecules_[i]; // store original state

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
		bondsOrig_[i] = bonds_[i]; // store original state
		// below: pushes back a Bond which is a ParticleRod which is a ParticleLink which is a ParticleContactGenerator
        world_.getContactGenerators().push_back( &bonds_[i] );
    }

	origContactGenerators_ = world_.getContactGenerators(); // store original state
}

// DESTRUCTOR
IceMelt::~IceMelt() {
    if (bonds_) delete[] bonds_;
	if (bondsOrig_) delete[] bondsOrig_;
	if (molecules_) delete[] molecules_;
	if (moleculesOrig_) delete[] moleculesOrig_;
}

// graphics initialization
void IceMelt::initGraphics() {
	glutReshapeWindow( 600, 800 ); // window size
	glutPositionWindow( 20, 20 ); // window position
	
	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    setView();
}

// UPDATE
void IceMelt::update() {
    world_.startFrame(); // clear accumulators
    
	duration_ = (float)TimingData::get().lastFrameDuration * 0.001f; // duration of the last frame in seconds
    if (duration_ <= 0.0f) return; // duration can't be negative

	if (zeroFlag_)
		duration_ = forcedDuration_; // override actual duration

	if( recording_ ) {
		sprintf_s( file_, "%s\\%d", path_, recID_ ); // set output directory (overwrites garbage)
		SHCreateDirectoryEx( 0, file_, NULL ); // create file_ directory if not already existing
		sprintf_s( file_, "%s\\frame%d.png", file_, (int)TimingData::get().frameNumber ); // set output filename based on frame
	}

	// add and remove contact generators as needed
	cyclone::ParticleWorld<Molecule>::ContactGenerators* cgs = &world_.getContactGenerators();
	cyclone::ParticleWorld<Molecule>::ContactGenerators::iterator g;
	for( unsigned i = 0; i < bondCount_; i++ ) {
		g = std::find( cgs->begin(), cgs->end(), &bonds_[i] );
		if( g == cgs->end() ) { // isn't in cgs
			if (bonds_[i].getState()) // has state of 1
				cgs->push_back( &bonds_[i] ); // add Bond contact generator to vector of contact generators
		} else { // is in cgs
			if (!bonds_[i].getState()) // has state of 0
				cgs->erase( g ); // remove Bond contact generator from vector of contact generators
		}
	}
	
    world_.runPhysics( duration_ ); // run the simulation
    Application::update();
	
	// update molecules
	for (unsigned i = 0; i < moleculeCount_; i++)
		if (molecules_[i].getState()) molecules_[i].update( duration_ );

	// update bonds
	for (unsigned i = 0; i < bondCount_; i++)
		if (bonds_[i].getState()) bonds_[i].update( duration_ );
}

// DRAW
void IceMelt::display() {
	// Clear the view port and set the camera direction
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    gluLookAt( 5.0, 2.0, 8.0,  0.5, 2.0, 0.0,  0.0, 1.0, 0.0 );

	// draw the molecules
	for (unsigned i = 0; i < moleculeCount_; i++)
		if (molecules_[i].getState()) molecules_[i].draw();

	// draw the bonds
	for (unsigned i = 0; i < bondCount_; i++)
		if (bonds_[i].getState()) bonds_[i].draw();

	// this HUD will be captured in screenshots
	char hud_top[50];
	sprintf_s( hud_top, "Delta Time:  %f [%s]", duration_, (zeroFlag_ ? "FORCED" : "NORMAL") );
	glColor3f( 0, 0, 0 );
	renderText( 10.0f, (height - 20.0f), hud_top );

	if( recording_ ) {
		BYTE* pixels = new BYTE[4 * width * height]; // make the BYTE array, factor of $ because it's RBGA
		glReadPixels( 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels ); // read screen pixels from the frame buffer into pixels_
		FIBITMAP* image = FreeImage_ConvertFromRawBits( pixels, width, height, 4 * width, 32, 
														FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false ); // convert
		delete pixels;

		FreeImage_Save( FIF_PNG, image, file_, 0 ); // save image_ to file_
		FreeImage_Unload( image );

		// this HUD won't be captured in screenshots
		char hud_bot[MAX_PATH + 50];
		sprintf_s( hud_bot, "File:  %s", file_ );
		glColor3f( 0, 0, 1.0f );
		renderText( 10.0f, 20.0f, hud_bot );
	}
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
		case '1': // #1: reset
				spaceFlag_ = false; // spacebar effect gets removed
				for (unsigned i = 0; i < moleculeCount_; i++) molecules_[i] = moleculesOrig_[i]; // restore original state
				for (unsigned i = 0; i < bondCount_; i++) bonds_[i] = bondsOrig_[i]; // restore original state
				world_.getContactGenerators().assign( origContactGenerators_.begin(), origContactGenerators_.end() ); // restore original state
			break;
		case '2': // #2: record
			recID_ = TimingData::getTime(); // set unique recording ID
			recording_ = (recording_ ? false : true);
			glutSetWindowTitle( getTitle() ); // update window title
			break;
		case '9': // 0: reset forced duration
			forcedDuration_ = FORCED_DURATION;
			break;
		case '0': // 0: override duration
			zeroFlag_ = (zeroFlag_ ? false : true);
			break;
		case '=': // =: increment duration
			forcedDuration_ += 0.001f;
			break;
		case '-': // -: decrement duration
			if (forcedDuration_ > 0.001f)
				forcedDuration_ -= 0.001f;
			else
				forcedDuration_ -= 0.0001f; // decrement slower

			if (forcedDuration_ <= 0.0f) forcedDuration_ = 0.0001f; // avoid zero duration
			break;
		case '5': // 5: tests reinstantiation of Bond between two Molecules
			unsigned i = 9;
			// set this Bond's first particle to a random particle
			bonds_[i].particle[0] = &molecules_[rand() % moleculeCount_];
			// reset the Bond
			bonds_[i].particle[0]->setTemp( 0 ); // reset temperature of first molecule
			bonds_[i].particle[1]->setTemp( 0.5f ); // set temperature of second molecule
			bonds_[i].setState( 1 ); // reset state of rod
			bonds_[i].setTemp( 0 ); // reset temperature of rod
			break;
    }
}

// returns program title
const char* IceMelt::getTitle() {
	if (recording_)
		return "[RECORDING] Cyclone > Ice Melt";
	else
		return "Cyclone > Ice Melt";
}

// create an application object
Application* getApplication() {
	return new IceMelt( 4 ); // depth gets passed as an unsigned int
}
