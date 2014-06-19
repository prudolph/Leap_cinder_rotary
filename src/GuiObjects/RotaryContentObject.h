#include "BaseGuiObject.h"
class RotaryContentObject : public guiObject::BaseGuiObject{
public:
	RotaryContentObject();
	~RotaryContentObject();
	virtual void setup(cinder::Vec2f pos, cinder::Vec2f size);


	//virtual	void				gui_touchesBeganHandler(int touchID, cinder::Vec2f touchPnt);
	//virtual void				gui_touchesMovedHandler(int touchID, cinder::Vec2f touchPnt);
	//virtual void				gui_touchesEndedHandler(int touchID, cinder::Vec2f touchPnt);

	virtual void draw();
	void setSelected(bool state){ mSelected = state; };
protected:
	bool mSelected;
};