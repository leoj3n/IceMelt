#include "IceMelt.h"

void Bond::update( cyclone::real duration ) {
	 // checks the molecules to see if this link should be destroyed
}

void Bond::draw() {
	const cyclone::Vector3 &p0 = particle[0]->getPosition();
	const cyclone::Vector3 &p1 = particle[1]->getPosition();

	glBegin( GL_LINES );
	glColor3f( 0, 0, 1 );
	glVertex3f( p0.x, p0.y, p0.z );
	glVertex3f( p1.x, p1.y, p1.z );
	glEnd();
}