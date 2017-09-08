#include "ListBox.h"

const int itemHeight = 20;

ListBox::ListBox(int positionX, int positionY, int width, int height) :
		Control(positionX, positionY, width, height)
{
	index = 0;
}

void ListBox::addItem(string item)
{
	items.push_back(item);
}

void ListBox::removeItem(int deleteIndex)
{
	int i = 0;
	for( vector<string>::iterator it = items.begin(); it != items.end(); it++ )
	{
		if( i == deleteIndex )
		{
			items.erase( it );
			break;
		}
		i++;
	}
	if( index >= (int)items.size() )
		index = (int)items.size() - 1;
}

void ListBox::setCurrent(int i)
{
	index = i;
}

int ListBox::getIndex(void)
{
	return index;
}

int ListBox::getCount(void)
{
	return (int)items.size();
}

bool ListBox::updateControl(MouseState &state)
{
	Control::updateControl(state);

	int x = state.x;
	int y = state.y;

	if ( inside == true && state.LeftButtonDown )
	{
		int tempIndex = (y - posY) / itemHeight;

		if ( tempIndex >= 0 && tempIndex < (int) items.size() )
		{
			index = tempIndex;
			return true;
		}
	}

	return false;
}

void ListBox::drawControl(void)
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

		if( index >= 0 )
		{
			glColor4f( 0.3f, 0.3f, 1.0f, 0.5f );

			int currentY = posY + index * itemHeight;

			glBegin(GL_QUADS);
				glVertex2d( posX + width,	currentY );
				glVertex2d( posX,			currentY );
				glVertex2d( posX,			currentY + itemHeight );
				glVertex2d( posX + width,	currentY + itemHeight );
			glEnd();
		}

		glColor4f( 0.7f, 0.7f, 0.7f, 1.0f );

		for( int i = 0; i < (int)items.size(); i++ )
		{
			iGLEngine->drawText( posX + 2, posY + 2 + i * itemHeight, items[i].data() );
		}

	glEnd();
}

string ListBox::getType(void)
{
	return "listbox";
}

