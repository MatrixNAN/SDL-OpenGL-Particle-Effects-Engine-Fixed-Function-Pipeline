#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <gl/GL.h>
	//#include <gl/OpenGL.h>
	#include <gl/GLU.h>
	//#include <GL\glut.h>
#endif

#include <string>
#include <vector>
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

enum TEXTURE_MODES
{
	MODE_NORMAL = 0,
	MODE_DARKEN,
};

struct TGA_Header
{
	GLubyte		ID_Length;
	GLubyte		ColorMapType;
	GLubyte		ImageType;
	GLubyte		ColorMapSpecification[5];
	GLshort		xOrigin;
	GLshort		yOrigin;
	GLshort		ImageWidth;
	GLshort		ImageHeight;
	GLubyte		pixelDepth;

};

class Texture
{
	// Member Functions
	public:
		Texture(string filename, string name = "", int mode = MODE_NORMAL);
		~Texture();

		int getMode(void);

	// Member Variables
	public:
		unsigned char	*imageData;
		unsigned int	bpp;
		unsigned int	width;
		unsigned int	height;
		unsigned int	texID;
		string			name;

		static vector<Texture *> textures;
	//
	private:
		bool	loadTGA(string filename);
		bool	createTexture(unsigned char *imageData, int width, int height, int type);

		int		mode;
};
#endif
