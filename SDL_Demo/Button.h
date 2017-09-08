#ifndef BUTTON_H
#define BUTTON_H

#include "Control.h"

class Button : public Control
{
	public:
		Button(string label, int positionX, int positionY, int width, int height);

		virtual bool updateControl(MouseState &state);
		virtual void drawControl(void);
		virtual string getType(void);
		
		string getLabel(void);

	protected:
		bool		down;
		string		label;

};

#endif
