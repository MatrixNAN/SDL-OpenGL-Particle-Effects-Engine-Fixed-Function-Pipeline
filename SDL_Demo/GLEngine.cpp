#include "GLEngine.h"

const int fontSize = 15;
const int fontSpace = 12;

GLEngine::GLEngine()
{

}

GLEngine::~GLEngine()
{

}

GLvoid GLEngine::Uninitialize(GLvoid)
{
	delete GLEngine::getEngine();
}

GLEngine *GLEngine::getEngine(GLvoid)
{
	static GLEngine *engine = new GLEngine();

	return engine;
}

GLvoid GLEngine::Initialize(GLint width, GLint height)
{
	Light::Initialize();

	fontTexture = new Texture("Textures/Font2.tga", "Font");

	buildTextureFont();
}

GLvoid GLEngine::buildTextureFont(GLvoid)
{
	fontBase = glGenLists(256);

	glBindTexture(GL_TEXTURE_2D, fontTexture->texID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for (int i = 0; i < 256; i++)
	{
		float cx = (float)(i % 16) / 16.0f;
		float cy = (float)(-i / 16) / 16.0f;

		glNewList(fontBase + i, GL_COMPILE);

		glBegin(GL_QUADS);
			glTexCoord2f(cx,			1 - cy);			glVertex2i( 0, fontSize );
			glTexCoord2f(cx + 0.0625f,	1 - cy);			glVertex2i( fontSize, fontSize );
			glTexCoord2f(cx + 0.0625f,	1 - cy - 0.0625f);	glVertex2i( fontSize, 0 );
			glTexCoord2f(cx,			1 - cy - 0.0625f);	glVertex2i( 0, 0 );
		glEnd();

		glTranslated(fontSpace, 0, 0);

		glEndList();
	}
}

GLvoid GLEngine::drawText(GLint x, GLint y, const char *in_text, ...)
{
	char text[256];

	va_list		ap;

	va_start(ap, in_text);
		vsprintf_s(text, in_text, ap);
	va_end(ap);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glBindTexture(GL_TEXTURE_2D, fontTexture->texID);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();

	glTranslated(x, y, 0);
	glListBase(fontBase - 16);
	glCallLists( (GLsizei)strlen(text), GL_BYTE, text);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

GLuint GLEngine::getTextWidth(const char *text)
{
	return GLuint( strlen(text) * fontSpace );
}

GLuint GLEngine::getTextHeight(const char *text)
{
	return ( fontSize );
}


