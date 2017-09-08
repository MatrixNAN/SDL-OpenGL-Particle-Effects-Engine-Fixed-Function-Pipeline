#include "Emitter.h"

Emitter::Emitter()
{
	lastTime		= -1;

	texture			= NULL;
	emissionRate	= 30.0f;
	emissionRadius	= 0.0f;

	life			= 2.0f;
	lifeRange		= 0.5f;

	size			= 5.0f;
	sizeRange		= 2.0f;

	saturation		= 1.0f;
	alpha			= 0.5f;

	spread			= 1.0f;

	gravity			= 0.0;
}

Emitter::~Emitter()
{
	for ( list<Particle *>::iterator it = particles.begin(); it != particles.end(); it++ )
	{
		delete (*it);
	}
}

void Emitter::Update(long time)
{
	if ( texture == NULL )
		return;

	if ( lastTime == -1.0f )
		lastTime = time;

	int numEmission = (int) ( (float) (time - lastTime) / 1000.0f * emissionRate );

	for (int i = 0; i < numEmission; i++)
		addParticle();

	if ( numEmission > 0 )
		lastTime = time;

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef( position.x, position.y, position.z );

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->texID);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

	if ( texture->getMode() == MODE_DARKEN )
	{
		glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);

		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_ALPHA);
	}
	else if ( texture->getMode() == MODE_NORMAL )
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);
	}

	for ( list<Particle *>::iterator it = particles.begin(); it != particles.end(); )
	{
		Particle *particle = (*it);

		particle->acceleration.y = -gravity;
		particle->acceleration += wind;
		particle->alpha = alpha;
		particle->rotation = rotation;

		particle->Update(time);

		if ( particle->active == false )
		{
			delete particle;

			it = particles.erase(it);
		}
		else
			it++;
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Emitter::setTexture(Texture *tex)
{
	texture = tex;
}

void Emitter::addParticle(void)
{
	Particle *particle = new Particle( (int)particles.size() );

	float r = frand() * saturation + ( 1 - saturation );
	float g = frand() * saturation + ( 1 - saturation );
	float b = frand() * saturation + ( 1 - saturation );

	particle->color = Vector3(r, g, b);

	particle->life = frand( life - lifeRange, life + lifeRange );
	particle->totalLife = particle->life;

	particle->velocity = Vector3( frand( -spread, spread ), frand( 3, 20 ), frand( -spread, spread ) );
	particle->acceleration = Vector3( 0.0f, -gravity, 0.0f );
	particle->size = frand( size - sizeRange, size + sizeRange );

	particle->position.x = frand( -emissionRadius, emissionRadius );
	particle->position.z = frand( -emissionRadius, emissionRadius );

	particles.push_back(particle);
}

float Emitter::frand(float start, float end)
{
	float num = (float) rand() / (float) RAND_MAX;

	return ( start + (end - start) * num );
}
