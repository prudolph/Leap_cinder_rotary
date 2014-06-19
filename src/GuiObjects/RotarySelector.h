#include "BaseButton.h"
class RotarySelector : public BaseButton{
public:
	struct RotarySection{
		int id;
		float low,high;
		guiObject::GuiObjectRef contentObject;
	};

	RotarySelector();
	~RotarySelector();
	virtual void setup(cinder::Vec2f centerPnt,float radius,float width);


	virtual	void				gui_touchesBeganHandler(int touchID, cinder::Vec2f touchPnt);
	virtual void				gui_touchesMovedHandler(int touchID, cinder::Vec2f touchPnt);
	//virtual void				gui_touchesEndedHandler(int touchID, cinder::Vec2f touchPnt);

	
	virtual void update();
	virtual void draw();
	void addSectionRange(float low, float high, guiObject::GuiObjectRef obj);
protected:
	bool containsPoint(cinder::Vec2f point);
	void updateSection();
	float mRadius,mWidth;
	RotarySection mCurrentSection;
	cinder::Anim<float> mRotation;
	cinder::Vec2f mCenterPoint;
	std::vector<RotarySection> mSectionRanges;
};