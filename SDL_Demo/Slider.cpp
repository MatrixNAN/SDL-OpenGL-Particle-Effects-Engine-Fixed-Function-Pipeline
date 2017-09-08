#include "Slider.h"

Slider::Slider(string lbl, float in_min, float in_max, int positionX, int positionY, int width, int height) :
		Control(positionX, positionY, width, height)
{
	defaultValue = 0.0f;

	current = NULL;

	min = in_min;
	max = in_max;

	label = lbl;

	dragging = false;
}

float Slider::getValue( void )
{
	return current;
}

void Slider::setValue( float *value )
{
	current = *value;

	if ( current != NULL )
	{
		defaultValue = current;
	}
}

bool Slider::updateControl(MouseState &state)
{
	Control::updateControl(state);

	int x = state.x;
	int y = state.y;

	if ( inside == true )
	{
		if ( state.LeftButtonDown )
		{
			dragging = true;
		}
		if ( state.RightButtonDown )
		{
			current = defaultValue;
		}
	}
	if ( !(state.LeftButtonDown) )
	{
		dragging = false;
	}
	if ( dragging == true )
	{
		current = (float)(x - posX) / (float)width * (max - min) + min;

		if ( current > max )
			current = max;
		else if ( current < min )
			current = min;
	}

	return dragging;
}

void Slider::drawControl(void)
{
	glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_TEXTURE_2D);

		glColor4f(0.7f, 0.7f, 0.7f, 0.8f);
		glBegin(GL_QUADS);
			glVertex2d( posX + width,	posY );
			glVertex2d( posX,			posY );
			glVertex2d( posX,			posY + height );
			glVertex2d( posX + width,	posY + height );
		glEnd();

		if ( inside == true )
		{
			glColor4f( 0.0f, 0.0f, 1.0f, 0.8f );
			glLineWidth( 1.0f );
		}
		else
		{
			glColor4f( 0.7f, 0.7f, 0.7f, 0.5f );
			glLineWidth( 2.0f );
		}

		glBegin(GL_LINE_STRIP);
			glVertex2d( posX + width,	posY );
			glVertex2d( posX,			posY );
			glVertex2d( posX,			posY + height );
			glVertex2d( posX + width,	posY + height );
			glVertex2d( posX + width,	posY );
		glEnd();
		
	int currentX = (int) ((current - min) / (max -min) * (width - tickSize) + posX);

	glColor4f(0.3f, 0.3f, 1.0f, 0.5f);
	glBegin(GL_QUADS);
		glVertex2d( currentX + tickSize,	posY );
		glVertex2d( currentX,				posY );
		glVertex2d( currentX,				posY + height );
		glVertex2d( currentX + tickSize,	posY + height );
	glEnd();

	glColor4f( 0.7f, 0.7f, 0.7f, 1.0f );
	iGLEngine->drawText(posX + 2, posY + 2, label.data());
}

string Slider::getType(void)
{
	return "slider";
}

