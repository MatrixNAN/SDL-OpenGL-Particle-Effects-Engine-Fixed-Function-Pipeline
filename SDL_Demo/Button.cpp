#include "Button.h"

Button::Button(string lbl, int positionX, int positionY, int width, int height) :
			Control(positionX, positionY, width, height)
{
	label = lbl;

	down = false;
}

string Button::getLabel(void)
{
	return label;
}

bool Button::updateControl(MouseState &state)
{
	Control::updateControl(state);

	if ( inside == true )
	{
		if ( state.LeftButtonDown )
			down = true;
		else if ( down == true )
		{
			down = false;

			return true;
		}
	}

	return false;
}

void Button::drawControl(void)
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
		
		glColor4f( 0.7f, 0.7f, 0.7f, 1.0f );
		int textX = posX + (width - iGLEngine->getTextWidth( label.data() )) / 2;
		int textY = posY + (height - iGLEngine->getTextHeight( label.data() )) / 2;

		iGLEngine->drawText(textX, textY, label.data());

	glEnd();
}

string Button::getType(void)
{
	return "button";
}

