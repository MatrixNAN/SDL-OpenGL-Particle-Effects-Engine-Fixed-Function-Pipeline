#include <stdlib.h>

#include "SDL.h"
#include "GLEngine.h"

#include "Button.h"
#include "ListBox.h"
#include "Slider.h"

#include "Emitter.h"

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	//#include <gl/OpenGL.h>
	#include <gl/GL.h>
	#include <gl/GLU.h>
	#include <gl/glext.h>
	//#include <GL\glut.h>
#endif

const GLsizei windowWidth = 1000;
const GLsizei windowHeight = 950;

const int leftMouseButton = 1;
const int middleMouseButton = 2;
const int rightMouseButton = 3;

const float rotateX_speed = 0.5f;
const float rotateY_speed = 0.5f;
const float zoomSpeed = 0.1f;

bool navigating = false;

GLfloat rotateX = 45.0f;
GLfloat rotateY = 45.0f;
GLfloat zoom = -60.0f;

Uint8 *keys = NULL; 

Light *mainLight = NULL;

Control *controlled = NULL;
MouseState state;

GLuint gridList = 0;

vector<Texture *> textureList;
vector<Emitter *> emitters;

ListBox *lstTextures = NULL;
ListBox *lstEmitters = NULL;

Slider *sldLife;
Slider *sldLifeRange;
Slider *sldSize;
Slider *sldSizeRange;
Slider *sldSpread;
Slider *sldSaturation;
Slider *sldEmissionRadius;
Slider *sldEmissionRate;
Slider *sldGravity;
Slider *sldOffsetX;
Slider *sldOffsetY;
Slider *sldOffsetZ;
Slider *sldVortex;
Slider *sldAlpha;

GLvoid establishProjectionMatrix(GLsizei width, GLsizei height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width /(GLfloat)height, 0.1f, 200.0f);
}

GLvoid setOrtho(GLsizei width, GLsizei height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(9, width, height, 0);
}

void updateControls(void)
{
	Emitter *currentEmitter = emitters[ lstEmitters->getIndex() ];

	sldLife->setValue				( &currentEmitter->life );
	sldLifeRange->setValue			( &currentEmitter->lifeRange );
	sldSize->setValue				( &currentEmitter->size );
	sldSizeRange->setValue			( &currentEmitter->sizeRange );
	sldSpread->setValue				( &currentEmitter->spread );
	sldSaturation->setValue			( &currentEmitter->saturation );
	sldEmissionRadius->setValue		( &currentEmitter->emissionRadius );
	sldEmissionRate->setValue		( &currentEmitter->emissionRate );
	sldGravity->setValue			( &currentEmitter->gravity );
	sldOffsetX->setValue			( &currentEmitter->position.x );
	sldOffsetY->setValue			( &currentEmitter->position.y );
	sldOffsetZ->setValue			( &currentEmitter->position.z );
	sldVortex->setValue				( &currentEmitter->rotation.y );
	sldAlpha->setValue				( &currentEmitter->alpha );
}

void updateEmitter(void)
{
	Emitter *currentEmitter = emitters[ lstEmitters->getIndex() ];

	currentEmitter->life =				sldLife->getValue();
	currentEmitter->lifeRange =			sldLifeRange->getValue();
	currentEmitter->size =				sldSize->getValue();
	currentEmitter->sizeRange =			sldSizeRange->getValue();
	currentEmitter->spread =			sldSpread->getValue();
	currentEmitter->saturation =		sldSaturation->getValue();
	currentEmitter->emissionRadius =	sldEmissionRadius->getValue();
	currentEmitter->emissionRate =		sldEmissionRate->getValue();
	currentEmitter->gravity =			sldGravity->getValue();
	currentEmitter->position.x =		sldOffsetX->getValue();
	currentEmitter->position.y =		sldOffsetY->getValue();
	currentEmitter->position.z =		sldOffsetZ->getValue();
	currentEmitter->rotation.y =		sldVortex->getValue();
	currentEmitter->alpha =				sldAlpha->getValue();
}

void initializeControls(void)
{
	textureList.push_back( new Texture("Textures/flare01t.tga", "Flare01") );
	textureList.push_back( new Texture("Textures/flare01tCol1.tga", "Flare01Col1") );
	textureList.push_back( new Texture("Textures/flare01tCol2.tga", "Flare01Col2") );
	textureList.push_back( new Texture("Textures/flare01tCol3.tga", "Flare01Col3") );
	textureList.push_back( new Texture("Textures/flare02t.tga", "Flare02", MODE_DARKEN) );
	textureList.push_back( new Texture("Textures/flare03t.tga", "Flare03") );
	textureList.push_back( new Texture("Textures/flare04t.tga", "Flare04") );
	textureList.push_back( new Texture("Textures/flare05t.tga", "Flare05") );
	textureList.push_back( new Texture("Textures/flare06t.tga", "Flare06") );
	textureList.push_back( new Texture("Textures/flare07t.tga", "Flare07") );
	textureList.push_back( new Texture("Textures/flare08t.tga", "Flare08") );
	textureList.push_back( new Texture("Textures/flare09t.tga", "Flare09") );
	textureList.push_back( new Texture("Textures/flare10t.tga", "Flare10") );
	textureList.push_back( new Texture("Textures/flare11t.tga", "Flare11") );
	textureList.push_back( new Texture("Textures/flare12t.tga", "Flare12") );
	textureList.push_back( new Texture("Textures/flare13t.tga", "Flare13") );

	Emitter *emitter = new Emitter();
	emitters.push_back(emitter);
	emitter->setTexture(textureList[0]);

	sldLife = (Slider *)addControl				( new Slider("Life", 0.0f, 10.0f, 0, 0, 250, 20) );
	sldLifeRange = (Slider *)addControl			( new Slider("Life Range", 0.0f, 3.0f, 0, 0, 250, 20) );
	sldSize = (Slider *)addControl				( new Slider("Size", 0.0f, 10.0f, 0, 0, 250, 20) );
	sldSizeRange = (Slider *)addControl			( new Slider("Size Range", 0.0f, 5.0f, 0, 0, 250, 20) );
	sldSpread = (Slider *)addControl			( new Slider("Spread", 0.0f, 10.0f, 0, 0, 250, 20) );
	sldSaturation = (Slider *)addControl		( new Slider("Saturation", 0.0f, 1.0f, 0, 0, 250, 20) );
	sldEmissionRadius = (Slider *)addControl	( new Slider("EmissionRadius", 0.0f, 10.0f, 0, 0, 250, 20) );
	sldEmissionRate = (Slider *)addControl		( new Slider("EmissionRate", 0.0f, 1000.0f, 0, 0, 250, 20) );
	sldGravity = (Slider *)addControl			( new Slider("Gravity", 0.0f, 20.0f, 0, 0, 250, 20) );
	sldOffsetX = (Slider *)addControl			( new Slider("OffsetX", -15.0f, 15.0f, 0, 0, 250, 20) );
	sldOffsetY = (Slider *)addControl			( new Slider("OffsetY", 0.0f, 15.0f, 0, 0, 250, 20) );
	sldOffsetZ = (Slider *)addControl			( new Slider("OffsetZ", -15.0f, 15.0f, 0, 0, 250, 20) );
	sldVortex = (Slider *)addControl			( new Slider("Vortex", -50.0f, 50.0f, 0, 0, 250, 20) );
	sldAlpha = (Slider *)addControl				( new Slider("Alpha", 0.0f, 1.0f, 0, 0, 250, 20) );
	
	lstTextures = (ListBox *)addControl( new ListBox(0, 0, 250, 350) );
	
	for(int i = 0; i < (int)textureList.size(); i++ )
		lstTextures->addItem( textureList[i]->name );
	
	lstEmitters = (ListBox *)addControl( new ListBox(0, 0, 250, 100) );
	
	for(int i = 0; i < (int)emitters.size(); i++ )
	{
		char text[80];
		sprintf_s(text, "Emitter %d", i + 1);
		lstEmitters->addItem( text );
	}

	addControl( new Button("Add", 0, 0, 100, 20) );
	addControl( new Button("Delete", 0, 0, 100, 20) );

	updateControls();
}

GLvoid initGL(GLsizei width, GLsizei height)
{
	iGLEngine->Initialize(width, height);

	establishProjectionMatrix(width, height);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	mainLight = new Light(LIGHT_SPOT);
	mainLight->setDiffuse(1.0f,1.0f,1.0f,1.0f);
	mainLight->setPosition(5.0,100.0,5.0);

	initializeControls();
}

void handleEvent(Control *control)
{
	Emitter *emitter = emitters[ lstEmitters->getIndex() ];

	if ( control->getType() == "button" )
	{
		Button *button = (Button *)control;
	
		if ( button->getLabel() == "Add" && (int)emitters.size() < 6 )
		{
			Emitter *emitter = new Emitter();
			emitters.push_back(emitter);

			emitter->setTexture( textureList[lstTextures->getIndex()] );

			char text[80];
			sprintf_s(text, "Emitter %d", (int)emitters.size());
			lstEmitters->addItem(text);
		}
		else if ( button->getLabel() == "Delete" && (int)emitters.size() > 1 )
		{
			int i = 0;

			for ( vector<Emitter *>::iterator it = emitters.begin(); it != emitters.end(); it++ )
			{
				if ( i == lstEmitters->getIndex() )
				{
					Emitter *emitter = (*it);
					emitters.erase(it);
					delete emitter;

					break;
				}
				i++;
			}
			lstEmitters->removeItem( lstEmitters->getIndex() );			
		}
	}
	else if ( control == lstTextures )
	{
		emitter->setTexture( textureList[ lstTextures->getIndex() ] );
	}
	else if ( control == lstEmitters )
	{
		int currentEmitter = lstEmitters->getIndex();

		for ( int i = 0; i < (int) textureList.size(); i++ ) 
		{
			if ( emitters[currentEmitter]->texture == textureList[i] )
			{
				lstTextures->setCurrent(i);
				break;
			}
		}
		updateControls();
	}
	updateEmitter();
}

GLvoid displayFPS(GLvoid)
{
	static long lastTime = SDL_GetTicks(); 
	static long loops = 0;
	static GLfloat fps = 0.0f;

	int newTime = SDL_GetTicks();
	
	if(newTime - lastTime > 100) 
	{
		float newFPS = (float)loops / float(newTime / lastTime) * 1000.0f;  

		fps = (fps + newFPS) / 2.0f;

		lastTime = newTime;

		loops = 0;
	}

//	iGLEngine->drawText(5, 5, "ABCDEFG   abcedfg", fps);
//	iGLEngine->drawText(5, 5, "ABCDEFGHIJKLMNOP", fps);
//	iGLEngine->drawText(5, 5, "abcdefghijklmnop", fps);
	iGLEngine->drawText(5, 5, "OpenGL Demo - %.2f", fps);

	loops++;
}

GLvoid drawCube(GLvoid)
{
	glBegin(GL_QUADS);
		// Top Face
		//glColor3f(1.0f, 0.5f, 0.0f);
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f,1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		// Bottom Face
		//glColor3f(0.0f, 1.0f, 0.0f);
		glNormal3f(0,-1,0);
		glTexCoord2f(1.0f,1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		// Front Face
		//glColor3f(1.0f, 0.0f, 0.0f);
		glNormal3f(0,0,1);
		glTexCoord2f(1.0f,1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		// Back Face
		//glColor3f(1.0f, 0.0f, 0.5f);
		glNormal3f(0,0,-1);
		glTexCoord2f(1.0f,1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		// Left Face
		//glColor3f(0.0f, 0.0f, 1.0f);
		glNormal3f(1,0,0);
		glTexCoord2f(1.0f,1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f,0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		// Right Face
		//glColor3f(1.0f, 0.0f, 1.0f);
		glNormal3f(-1,0,0);
		glTexCoord2f(1.0f,1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f,1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f,0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();

}

GLvoid drawGrid(GLvoid)
{
	const float width = 80.0f;
	const float height = 80.0f;
	const int divisions = 100;

	float incX = width / (float) divisions;
	float incY = height / (float) divisions;

	glColor3f(0.5f, 0.5f, 0.5f);
	glNormal3f(0,1,0);
	for (float x = -width / 2; x < width / 2; x += incX)
	{
		for (float y = -height / 2; y < height / 2; y += incY)
		{
			glBegin(GL_TRIANGLE_STRIP);
			// Top Face
				glVertex3f(x + incX, 0.0f, y + incY);
				glVertex3f(x,		 0.0f, y + incY);
				glVertex3f(x + incX, 0.0f, y);
				glVertex3f(x,		 0.0f, y);
			glEnd();
		}
	}
}

GLvoid drawControls(GLvoid)
{
	for ( list<Control *>::iterator it = Control::controls.begin(); it != Control::controls.end(); it++ )
	{
		Control *control = (*it);

		control->drawControl();

		if ( (controlled != NULL && controlled != control) || navigating )
			continue;

		if ( control->updateControl(state) )
		{
			controlled = control;

			handleEvent(control);
		}
		else if ( control == controlled )
		{
			controlled = NULL;
		}
		else
		{
			// Control has been updated ... but no messages to handle
		}
	}
}

GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// 1st Pass 3D Perspective
	establishProjectionMatrix(windowWidth, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,zoom);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);
	
	for(int i = 0; i < (int)Light::lights.size(); i++)
		Light::lights[i]->updateLight();

	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	drawGrid();

	glEnable(GL_BLEND);

	for(int i = 0; i < (int)emitters.size(); i++)
		emitters[i]->Update( SDL_GetTicks() );

	glColor3f(1.0f, 1.0f, 1.0f);

	glEnable(GL_BLEND);
/*	
	if (gridList == 0)
	{
		gridList = glGenLists(1);
		glNewList(gridList, GL_COMPILE_AND_EXECUTE);


		for (GLfloat x = -20.0f; x < 20.0f; x += 2.5f)
		{
			for (GLfloat y = -20.0f; y < 20.0f; y += 2.5f)
			{
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();

				glTranslatef(x,y,0.0f);
				drawCube();

				glPopMatrix();
			}
		}
	
		glEndList();
	}
	else
	{
		glCallList(gridList);
	}
*/
	// 2nd Pass 2D Overlay
	glDisable(GL_LIGHTING);
	setOrtho(windowWidth, windowHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	displayFPS();
	drawControls();

	glFlush();

	// this line is modified depending on which window manager is used...
	SDL_GL_SwapBuffers();
}

GLboolean updateMouseNavigation(GLvoid)
{
	static int lastX = -1;
	static int lastY = -1;

	if ( lastX == -1 && lastY == -1 )
	{
		lastX = state.x;
		lastY = state.y;
	}
	
	int changeX = lastX - state.x;
	int changeY = lastY - state.y;

	lastX = state.x;
	lastY = state.y;

	if ( state.LeftButtonDown && controlled == NULL )
	{
		SDL_WM_GrabInput(SDL_GRAB_ON);	
		rotateX -= (float)changeY * rotateX_speed;
		rotateY -= (float)changeX * rotateY_speed;

		// Keeps From Rotating Below The Particle Engine Grid
		if( rotateX < 1.0f )
			rotateX = 1.0f;

		navigating = true;
	}
	else if ( state.RightButtonDown && controlled == NULL )
	{
		SDL_WM_GrabInput(SDL_GRAB_ON);	
		zoom -= (float)changeX * zoomSpeed;

		navigating = true;		
	}
	else
	{
		SDL_WM_GrabInput(SDL_GRAB_OFF);	

		navigating = false;				
	}

	return false;
}

GLboolean checkKeys(GLvoid)
{
	static long lastTime = SDL_GetTicks();
	const GLfloat speed = 1.0f;
	const long updateTime = 10;
	long newTime = SDL_GetTicks();

	if (newTime - lastTime > updateTime)
	{
		if(keys[SDLK_ESCAPE])
			return true;

		if(keys[SDLK_LEFT])
			rotateY -= speed;

		if(keys[SDLK_RIGHT])
			rotateY += speed;

		if(keys[SDLK_UP])
			rotateX -= speed;

		if(keys[SDLK_DOWN])
			rotateX += speed;
	}

	return false;
}

int main(int argc,  char **argv)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// error
		fprintf(stderr, "Unable to initalize SDL %s", SDL_GetError());
		exit(1);
	}

	if(SDL_SetVideoMode(windowWidth, windowHeight, 0, SDL_OPENGL) == NULL)
	{
		// error
		fprintf(stderr, "Unable to Create OpenGL Scene %s", SDL_GetError());
		exit(2);
	}

	
	initGL(windowWidth, windowHeight);

	int done = 0;

	while(!done)
	{
		SDL_GetMouseState(&state.x, &state.y);
		
		state.LeftButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(leftMouseButton);
		state.MiddleButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(middleMouseButton);
		state.RightButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(rightMouseButton);

		drawScene();
		SDL_Event event;
		while (SDL_PollEvent (&event))
		{
			if(event.type == SDL_QUIT)
			{
				done = 1;
			}

			keys = SDL_GetKeyState(NULL);
		}

		if(checkKeys() || updateMouseNavigation())
		{
			done = 1;
		}
	}
	for ( list<Control *>::iterator it = Control::controls.begin(); it != Control::controls.end(); it++ )
	{
		delete(*it);
		
		it = Control::controls.begin();
	}

	GLEngine::Uninitialize();
	SDL_Quit();

	return 1;
}