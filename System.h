#include <vector>

/* A particle is defined inside PhysEnv.h as
struct tParticle
{
	tVector pos;		// Position of Particle
    tVector v;			// Velocity of Particle
	tVector f;			// Force Acting on Particle
	float	oneOverM;	// 1 / Mass of Particle
};
*/
#include "PhysEnv.h"


/* System of particles (masses that are connected together using springs)
* In this class, we encapsulate a vector of particles.

* When implementing different integrators, we often need to perform sum/average/... of the derivatives of the system at different time steps. 
* In our case, the derivatives are the velocity (first derivative) and acceleration/force (second derivative) of each particle in the system.

* Below are some helper functions that can make your life easier when doing such arithmetic operations
*/
class System
{
	vector<tParticle> particles;

public:
	/* Constructor 1
	* Create a system of n particles
	* The system is initially empty and acts as a place holder for later operations
	* For more information about implementing constructors, visit https://www.cplusplus.com/doc/tutorial/classes/
	*/
	System(int n)
	{
		this->particles = vector<tParticle>(n);
	}
	
	/* Constructor 2
	* Creates a system of particles and fills it using the input array `tParticle* sys`.
	*/
	System(tParticle* sys, int n)
	{
		this->particles = vector<tParticle>(n);
		memcpy(this->particles.data(), sys, n*sizeof(tParticle));
	}
	
	/* Overload for the plus operator
	*
	* This method is called when you perform System s3 = s1 + s2 
	* This is a syntactic sugar for saying s3 = s1.operator+(s2)
	* In this case, "this" becomes a pointer to s1 and "other" becomes the object s2.
	* For more information, visit https://www.cplusplus.com/doc/tutorial/templates/

	* In this method, we simply loop over all particles do element-wise sum of the derivatives in the two systems
	* Although we usually don't need to sum positions, we do this for convenience.
	*/
	System operator +(const System &other)
	{
		int n = this->particles.size();

		System ret(n);
		for (int i = 0; i < n; i++)	
		{
			ret.particles[i].oneOverM = this->particles[i].oneOverM;

			ret.particles[i].pos.x = this->particles[i].pos.x + other.particles[i].pos.x;
			ret.particles[i].pos.y = this->particles[i].pos.y + other.particles[i].pos.y;
			ret.particles[i].pos.z = this->particles[i].pos.z + other.particles[i].pos.z;

			ret.particles[i].v.x = this->particles[i].v.x + other.particles[i].v.x;
			ret.particles[i].v.y = this->particles[i].v.y + other.particles[i].v.y;
			ret.particles[i].v.z = this->particles[i].v.z + other.particles[i].v.z;

			ret.particles[i].f.x = this->particles[i].f.x + other.particles[i].f.x;
			ret.particles[i].f.y = this->particles[i].f.y + other.particles[i].f.y;
			ret.particles[i].f.z = this->particles[i].f.z + other.particles[i].f.z;
		}

		return ret;
	}

	/* TODO
	* In a similar fashion, fill in the other operators
	* If you want to make your code more compact, feel free to use functions defined in MathDefs.h
	*/
	
	System operator -(const System &other)
	{
		int n = this->particles.size();

		System ret(n);
		for (int i = 0; i < n; i++)
		{
			ret.particles[i].oneOverM = this->particles[i].oneOverM;

			ret.particles[i].pos.x = this->particles[i].pos.x - other.particles[i].pos.x;
			ret.particles[i].pos.y = this->particles[i].pos.y - other.particles[i].pos.y;
			ret.particles[i].pos.z = this->particles[i].pos.z - other.particles[i].pos.z;

			ret.particles[i].v.x = this->particles[i].v.x - other.particles[i].v.x;
			ret.particles[i].v.y = this->particles[i].v.y - other.particles[i].v.y;
			ret.particles[i].v.z = this->particles[i].v.z - other.particles[i].v.z;

			ret.particles[i].f.x = this->particles[i].f.x - other.particles[i].f.x;
			ret.particles[i].f.y = this->particles[i].f.y - other.particles[i].f.y;
			ret.particles[i].f.z = this->particles[i].f.z - other.particles[i].f.z;
		}

		return ret;
	}
	
	System operator *(float k)
	{
		int n = this->particles.size();

		System ret(n);
		for (int i = 0; i < n; i++)
		{
			ret.particles[i].oneOverM = this->particles[i].oneOverM;

			ret.particles[i].pos.x = k * this->particles[i].pos.x;
			ret.particles[i].pos.y = k * this->particles[i].pos.y;
			ret.particles[i].pos.z = k * this->particles[i].pos.z;

			ret.particles[i].v.x = k * this->particles[i].v.x;
			ret.particles[i].v.y = k * this->particles[i].v.y;
			ret.particles[i].v.z = k * this->particles[i].v.z;

			ret.particles[i].f.x = k * this->particles[i].f.x;
			ret.particles[i].f.y = k * this->particles[i].f.y;
			ret.particles[i].f.z = k * this->particles[i].f.z;
		}

		return ret;
	}

	System operator /(float k)
	{
		int n = this->particles.size();

		System ret(n);
		for (int i = 0; i < n; i++)
		{
			ret.particles[i].oneOverM = this->particles[i].oneOverM;

			ret.particles[i].pos.x = k / this->particles[i].pos.x;
			ret.particles[i].pos.y = k / this->particles[i].pos.y;
			ret.particles[i].pos.z = k / this->particles[i].pos.z;

			ret.particles[i].v.x = k / this->particles[i].v.x;
			ret.particles[i].v.y = k / this->particles[i].v.y;
			ret.particles[i].v.z = k / this->particles[i].v.z;

			ret.particles[i].f.x = k / this->particles[i].f.x;
			ret.particles[i].f.y = k / this->particles[i].f.y;
			ret.particles[i].f.z = k / this->particles[i].f.z;
		}

		return ret;
	}


	/*
	* Functions like IntegrateSysOverTime and ComputeForces inside PhysEnv.cpp take their input as tParticle*
	* For compatibility, we need to be able to cast our system of particles to tParticle* to be able to pass our System as an argument to these functions
	*/
	operator tParticle*()
	{
		return this->particles.data();
	}

	/*
	* You might find this method useful. 
	* You might need to call s.fillOut(m_TargetSys) somewhere, 
	* which will copy the particles in the system "s" to the array of particles "m_TargetSys"
	*/
	void fillOut(tParticle* sys)
	{
		int n = this->particles.size();
		memcpy(sys, this->particles.data(), n*sizeof(tParticle));
	}
};
