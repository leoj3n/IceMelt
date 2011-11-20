/*
 * Interface for the particle / mass aggregate world structure.
 *
 * Adapted from part of the Cyclone physics system.
 *
 * Now uses templates.
 *
 */

/**
 * This file contains the definitions for a structure to hold any number o
 * particle masses, and their connections.
 */
#ifndef CYCLONE_PWORLD_H
#define CYCLONE_PWORLD_H

#include <cstdlib>
#include <cyclone/pfgen.h>
#include "plinks.h"

namespace cyclone {

	/**
	 * Keeps track of a set of particles, and provides the means to
	 * update them all.
	 */
	template <class T>
	class ParticleWorld
	{
	public:
		typedef std::vector<T*> Particles;
		typedef std::vector<ParticleContactGenerator*> ContactGenerators;

	protected:
		// Holds the particles
		Particles particles;

		/**
		 * True if the world should calculate the number of iterations
		 * to give the contact resolver at each frame.
		 */
		bool calculateIterations;

		// Holds the force generators for the particles in this world.
		ParticleForceRegistry registry;

		// Holds the resolver for contacts.
		ParticleContactResolver resolver;

		// Contact generators.
		ContactGenerators contactGenerators;

		// Holds the list of contacts.
		ParticleContact *contacts;

		/**
		 * Holds the maximum number of contacts allowed (i.e. the
		 * size of the contacts array).
		 */
		unsigned maxContacts;

	public:

		/**
		 * Creates a new particle simulator that can handle up to the
		 * given number of contacts per frame. You can also optionally
		 * give a number of contact-resolution iterations to use. If you
		 * don't give a number of iterations, then twice the number of
		 * contacts will be used.
		 */
		ParticleWorld( unsigned maxContacts, unsigned iterations = 0 );

		// Deletes the simulator.
		~ParticleWorld();

		/**
		 * Calls each of the registered contact generators to report
		 * their contacts. Returns the number of generated contacts.
		 */
		unsigned generateContacts();

		/**
		 * Integrates all the particles in this world forward in time
		 * by the given duration.
		 */
		void integrate( real duration );

		// Processes all the physics for the particle world.
		void runPhysics( real duration );

		/**
		 * Initializes the world for a simulation frame. This clears
		 * the force accumulators for particles in the world. After
		 * calling this, the particles can have their forces for this
		 * frame added.
		 */
		void startFrame();

		// Returns the list of particles.
		Particles& getParticles();

		// Returns the list of contact generators.
		ContactGenerators& getContactGenerators();

		// Returns the force registry.
		ParticleForceRegistry& getForceRegistry();
	}; // class ParticleWorld

	/**
	 * A contact generator that takes an STL vector of particle pointers and
	 * collides them against the ground.
	 */
	template <class T>
	class GroundContacts : public ParticleContactGenerator
	{
		typename ParticleWorld<T>::Particles *particles;

	public:
		void init( typename ParticleWorld<T>::Particles *particles );

		virtual unsigned addContact( ParticleContact *contact, unsigned limit ) const;
	};

	/*
	 * Implementation for random number generation.
	 *
	 * Adapted from part of the Cyclone physics system.
	 *
	 * Now uses templates.
	 *
	 */
	template <class T>
	ParticleWorld<T>::ParticleWorld( unsigned maxContacts, unsigned iterations )
	:
	resolver( iterations ),
	maxContacts( maxContacts ) {
		contacts = new ParticleContact[maxContacts];
		calculateIterations = (iterations == 0);
	}

	template <class T>
	ParticleWorld<T>::~ParticleWorld() {
		delete[] contacts;
	}

	template <class T>
	void ParticleWorld<T>::startFrame() {
		// Remove all forces from the accumulator    
		for (Particles::iterator p = particles.begin(); p != particles.end(); p++)
			(*p)->clearAccumulator();
	}

	template <class T>
	unsigned ParticleWorld<T>::generateContacts() {
		unsigned limit = maxContacts;
		ParticleContact *nextContact = contacts;

		for( ContactGenerators::iterator g = contactGenerators.begin(); g != contactGenerators.end(); g++ ) {
			unsigned used =(*g)->addContact( nextContact, limit );
			limit -= used;
			nextContact += used;

			// We've run out of contacts to fill. This means we're missing
			// contacts.
			if (limit <= 0) break;
		}

		// Return the number of contacts used.
		return maxContacts - limit;
	}

	template <class T>
	void ParticleWorld<T>::integrate( real duration ) {
		// Remove all forces from the accumulator
		for (Particles::iterator p = particles.begin(); p != particles.end(); p++)
			(*p)->integrate( duration );
	}

	template <class T>
	void ParticleWorld<T>::runPhysics( real duration ) {
		// First apply the force generators
		registry.updateForces(duration);

		// Then integrate the objects
		integrate(duration);

		// Generate contacts
		unsigned usedContacts = generateContacts();

		// And process them
		if( usedContacts ) {
			if (calculateIterations) resolver.setIterations( usedContacts * 2 );
			resolver.resolveContacts( contacts, usedContacts, duration );
		}
	}

	template <class T>
	typename ParticleWorld<T>::Particles& ParticleWorld<T>::getParticles() {
		return particles;
	}

	template <class T>
	typename ParticleWorld<T>::ContactGenerators& ParticleWorld<T>::getContactGenerators() {
		return contactGenerators;
	}

	template <class T>
	ParticleForceRegistry& ParticleWorld<T>::getForceRegistry() {
		return registry;
	}

	template <class T>
	void GroundContacts<T>::init( typename ParticleWorld<T>::Particles *particles ) {
		GroundContacts<T>::particles = particles;
	}

	template <class T>
	unsigned GroundContacts<T>::addContact( ParticleContact *contact, unsigned limit ) const {
		unsigned count = 0;
		for( ParticleWorld<T>::Particles::iterator p = particles->begin(); p != particles->end(); p++ ) {
			real y = (*p)->getPosition().y;
			if( y < 0.0f ) {
				contact->contactNormal = Vector3::UP;
				contact->particle[0] = *p;
				contact->particle[1] = NULL;
				contact->penetration = -y;
				contact->restitution = 0.2f;
				contact++;
				count++;
			}

			if (count >= limit) return count;
		}
		return count;
	}

} // namespace cyclone

#endif // CYCLONE_PWORLD_H