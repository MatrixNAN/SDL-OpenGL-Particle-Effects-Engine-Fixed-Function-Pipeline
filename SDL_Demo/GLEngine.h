#ifndef GL_ENGINE_H
#define GL_ENGINE_H

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

#include "Light.h"
#include "Texture.h"

#define iGLEngine GLEngine::getEngine()

class GLEngine
{
	public:
		GLEngine();
		~GLEngine();
		static GLvoid Uninitialize(GLvoid);
		static GLEngine *getEngine(GLvoid);

		GLvoid Initialize(GLint width, GLint height);

		GLvoid buildTextureFont(GLvoid);
		GLvoid drawText(GLint x, GLint y, const char *text, ...);

		GLuint getTextWidth(const char *text);
		GLuint getTextHeight(const char *text);

	private:
		GLuint	fontBase;

		Texture	*fontTexture;

};

#endif