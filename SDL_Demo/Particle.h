#ifndef PARTICLE_H
#define PARTICLE_H

#include "GLEngine.h"
#include "Vector3.h"

class Particle
{
	public:
		Particle(long id);

		void Update(long time);

	public:
		long		lastTime;

		long		id;

		Vector3		color;
		
		Vector3		position;
		Vector3		velocity;
		Vector3		acceleration;
		Vector3		rotation;

		float		totalLife;
		float		life;

		float		alpha;
		float		size;

		float		bounciness;
		bool		active;

	private:
		void Rotate(float angle, float &x, float &y);
};

#endif