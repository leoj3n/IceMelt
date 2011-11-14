/* Bond header file. */
#ifndef BOND_H
#define BOND_H

#include "../../../include/cyclone/plinks.h"
#include "Molecule.h"

class Bond : public cyclone::ParticleRod
{
private:

public:
	Molecule *molecules;

	void update();	// checks the molecules to see if this link should be destroyed
};
#endif