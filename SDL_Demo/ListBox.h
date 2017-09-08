#ifndef LIST_BOX_H
#define LIST_BOX_H

#include "Control.h"
#include <vector>

using std::vector;

class ListBox : public Control
{
	public:
		ListBox(int positionX, int positionY, int width, int height);

		void addItem(string item);
		void removeItem(int index);
		void setCurrent(int index);

		int getIndex(void);
		int getCount(void);

		virtual bool updateControl(MouseState &state);
		virtual void drawControl(void);
		virtual string getType(void);

	protected:
		int				index;
		vector<string>	items;
};


#endif