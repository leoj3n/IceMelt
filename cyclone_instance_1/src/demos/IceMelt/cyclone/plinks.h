/*
 * Interface for the particle links.
 *
 * Adapted from part of the Cyclone physics system.
 *
 * Now uses templates.
 *
 */

/**
 * This file contains classes representing the connections between
 * particles.
 */

#ifndef CYCLONE_PLINKS_H
#define CYCLONE_PLINKS_H

#include <cyclone/pcontacts.h>

namespace cyclone {

	/**
	 * Links connect two particles together, generating a contact if
	 * they violate the constraints of their link. It is used as a
	 * base class for cables and rods, and could be used as a base
	 * class for springs with a limit to their extension..
	 */
	template <class T>
	class ParticleLink : public ParticleContactGenerator
	{
	public:
		/**
		 * Holds the pair of particles that are connected by this link.
		 */
		T* particle[2];

	protected:
		real currentLength() const;

	public:
		/**
		 * Geneates the contacts to keep this link from being
		 * violated. This class can only ever generate a single
		 * contact, so the pointer can be a pointer to a single
		 * element, the limit parameter is assumed to be at least one
		 * (zero isn't valid) and the return value is either 0, if the
		 * cable wasn't over-extended, or one if a contact was needed.
		 *
		 * NB: This method is declared in the same way (as pure
		 * virtual) in the parent class, but is replicated here for
		 * documentation purposes.
		 */
		virtual unsigned addContact(ParticleContact *contact,
									unsigned limit) const = 0;
	};

	/**
	 * Cables link a pair of particles, generating a contact if they
	 * stray too far apart.
	 */
	template <class T>
	class ParticleCable : public ParticleLink<T>
	{
	public:
		real maxLength;
		real restitution;

	public:
		virtual unsigned addContact(ParticleContact *contact,
									unsigned limit) const;
	};

	/**
	 * Rods link a pair of particles, generating a contact if they
	 * stray too far apart or too close.
	 */
	template <class T>
	class ParticleRod : public ParticleLink<T>
	{
	public:
		real length;

	public:
		virtual unsigned addContact(ParticleContact *contact,
										unsigned limit) const;
	};

	/**
	 * Constraints are just like links, except they connect a particle to
	 * an immovable anchor point.
	 */
	template <class T>
	class ParticleConstraint : public ParticleContactGenerator
	{
	public:
		/**
		* Holds the particles connected by this constraint.
		*/
		T* particle;
		Vector3 anchor;

	protected:
		real currentLength() const;

	public:
		/**
		* Geneates the contacts to keep this link from being
		* violated. This class can only ever generate a single
		* contact, so the pointer can be a pointer to a single
		* element, the limit parameter is assumed to be at least one
		* (zero isn't valid) and the return value is either 0, if the
		* cable wasn't over-extended, or one if a contact was needed.
		*
		* NB: This method is declared in the same way (as pure
		* virtual) in the parent class, but is replicated here for
		* documentation purposes.
		*/
		virtual unsigned addContact(ParticleContact *contact,
			unsigned limit) const = 0;
	};

	/**
	* Cables link a particle to an anchor point, generating a contact if they
	* stray too far apart.
	*/
	template <class T>
	class ParticleCableConstraint : public ParticleConstraint<T>
	{
	public:
		real maxLength;
		real restitution;

	public:
		virtual unsigned addContact(ParticleContact *contact,
			unsigned limit) const;
	};

	/**
	* Rods link a particle to an anchor point, generating a contact if they
	* stray too far apart or too close.
	*/
	template <class T>
	class ParticleRodConstraint : public ParticleConstraint<T>
	{
	public:
		real length;

	public:
		virtual unsigned addContact(ParticleContact *contact,
			unsigned limit) const;
	};

	/*
	 * Implementation for particle links.
	 *
	 * Adapted from part of the Cyclone physics system.
	 *
	 * Now uses templates.
	 *
	 */
	template <class T>
	real ParticleLink<T>::currentLength() const {
		Vector3 relativePos = particle[0]->getPosition() - particle[1]->getPosition();
		return relativePos.magnitude();
	}

	template <class T>
	unsigned ParticleCable<T>::addContact(ParticleContact *contact,
										unsigned limit) const {
		// Find the length of the cable
		real length = currentLength();

		// Check if we're over-extended
		if (length < maxLength) return 0;

		// Otherwise return the contact
		contact->particle[0] = particle[0];
		contact->particle[1] = particle[1];

		// Calculate the normal
		Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
		normal.normalise();
		contact->contactNormal = normal;

		contact->penetration = length-maxLength;
		contact->restitution = restitution;

		return 1;
	}

	template <class T>
	unsigned ParticleRod<T>::addContact(ParticleContact *contact,
									  unsigned limit) const {
		// Find the length of the rod
		real currentLen = currentLength();

		// Check if we're over-extended
		if (currentLen == length) return 0;

		// Otherwise return the contact
		contact->particle[0] = particle[0];
		contact->particle[1] = particle[1];

		// Calculate the normal
		Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
		normal.normalise();

		// The contact normal depends on whether we're extending or compressing
		if( currentLen > length ) {
			contact->contactNormal = normal;
			contact->penetration = currentLen - length;
		} else {
			contact->contactNormal = normal * -1;
			contact->penetration = length - currentLen;
		}

		// Always use zero restitution (no bounciness)
		contact->restitution = 0;

		return 1;
	}

	template <class T>
	real ParticleConstraint<T>::currentLength() const {
		Vector3 relativePos = particle->getPosition() - anchor;
		return relativePos.magnitude();
	}

	template <class T>
	unsigned ParticleCableConstraint<T>::addContact(ParticleContact *contact,
									   unsigned limit) const {
		// Find the length of the cable
		real length = currentLength();

		// Check if we're over-extended
		if (length < maxLength) return 0;

		// Otherwise return the contact
		contact->particle[0] = particle;
		contact->particle[1] = 0;

		// Calculate the normal
		Vector3 normal = anchor - particle->getPosition();
		normal.normalise();
		contact->contactNormal = normal;

		contact->penetration = length-maxLength;
		contact->restitution = restitution;

		return 1;
	}

	template <class T>
	unsigned ParticleRodConstraint<T>::addContact(ParticleContact *contact,
									 unsigned limit) const {
		// Find the length of the rod
		real currentLen = currentLength();

		// Check if we're over-extended
		if (currentLen == length) return 0;

		// Otherwise return the contact
		contact->particle[0] = particle;
		contact->particle[1] = 0;

		// Calculate the normal
		Vector3 normal = anchor - particle->getPosition();
		normal.normalise();

		// The contact normal depends on whether we're extending or compressing
		if( currentLen > length ) {
			contact->contactNormal = normal;
			contact->penetration = currentLen - length;
		} else {
			contact->contactNormal = normal * -1;
			contact->penetration = length - currentLen;
		}

		// Always use zero restitution (no bounciness)
		contact->restitution = 0;

		return 1;
	}

} // namespace cyclone

#endif // CYCLONE_PLINKS_H