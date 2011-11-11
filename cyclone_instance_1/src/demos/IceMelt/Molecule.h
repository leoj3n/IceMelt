/* Molecule header file. */
#include "../../../include/cyclone/particle.h"

class Molecule : public cyclone::Particle
{
public:
	cyclone::real temp; // what temperature is the molecule
	unsigned state;
  // cyclone::ParticleRod *rods;

   void update( cyclone::real duration );
private:

};
