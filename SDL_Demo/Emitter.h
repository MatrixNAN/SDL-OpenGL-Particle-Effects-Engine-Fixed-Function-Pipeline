#ifndef EMITTER_H
#define EMITTER_H

#include <list>
#include "Particle.h"

using std::list;

class Emitter
{
	public:
		Emitter();
		~Emitter();

		void Update(long time);
		void setTexture(Texture *texture);

	public:
		list<Particle *>	particles; 
		Texture				*texture;

		long				lastTime;

		/*****************************************/
		/*				Attributes				 */
		/*****************************************/

		float			life;
		float			lifeRange;

		float			size;
		float			sizeRange;

		float			spread;

		float			gravity;

		Vector3			position;
		Vector3			wind;
		Vector3			rotation;

		float			alpha;
		float			saturation;
		float			emissionRate;
		float			emissionRadius;

	protected:
		void addParticle(void);
		float frand(float start = 0, float end = 1);
};

#endif