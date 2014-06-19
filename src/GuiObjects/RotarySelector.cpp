#include "RotarySelector.h"
#include "RotaryContentObject.h"

#include <math.h>
using namespace std;
using namespace ci;
using namespace ci::app;

RotarySelector::RotarySelector() : BaseButton(),
mRadius(0.0f),
mWidth(0.0f),
mCenterPoint(Vec2f(0.0f, 0.0f)),
mRotation(25.0f)
{

}

RotarySelector::~RotarySelector(){

}

void RotarySelector::setup(Vec2f centerPnt, float radius, float width){
	gui_registerWithTouchMngr();
	mRadius = radius;
	
	mWidth = width;

	float outerRim = (radius + width)/2.0f;
	mCenterPoint = centerPnt - Vec2f(outerRim, outerRim);
	gui_setPosition(mCenterPoint);
	float diameter = 2 * (radius + width);
	gui_setSize(diameter, diameter);



}
void RotarySelector::addSectionRange(float low, float high, guiObject::GuiObjectRef obj){
	RotarySection section;
	section.id = (int)mSectionRanges.size();
	section.low = low;
	section.high = high;
	section.contentObject = obj;
	mSectionRanges.push_back(section);
}

void	RotarySelector::gui_touchesBeganHandler(int touchID, cinder::Vec2f touchPnt){
	//keep track of the touch position so we can later test how far it has moved
	//THis object only takes one touch
	if (gui_ObjectTouchIDs.empty() && containsPoint(touchPnt)){
		gui_ObjectTouchIDs.push_back(touchID);
	}

	mRotation = 360 + fmod((atan2(touchPnt.y - gui_getCenter().y, touchPnt.x - gui_getCenter().x) * 180.0f / M_PI - 270), 360);
	
	
	
}
void RotarySelector::gui_touchesMovedHandler(int touchID, cinder::Vec2f touchPnt){
	//if the touch point is out of the space of the button = touch canceled.
	if (!gui_ObjectTouchIDs.empty()){
		int currentTouchId = gui_ObjectTouchIDs.front();
		
		
		mRotation=360+fmod(( atan2(touchPnt.y - gui_getCenter().y, touchPnt.x - gui_getCenter().x ) * 180.0f / M_PI-270) , 360);
		updateSection();
	
	}
}
void RotarySelector::updateSection(){
	
	for (RotarySection section : mSectionRanges){
	
		if (mRotation > section.low && mRotation <= section.high){
			if (mCurrentSection.id != section.id){
				std::shared_ptr<RotaryContentObject> curObj = std::dynamic_pointer_cast<RotaryContentObject>(mCurrentSection.contentObject);
				if (curObj)curObj->setSelected(false);
			
				std::shared_ptr<RotaryContentObject>  newObj =std::dynamic_pointer_cast<RotaryContentObject>(section.contentObject);
				if (newObj)newObj->setSelected(true);

				mCurrentSection = section;
			}
			break;
		}
	}



}
/*
void	RotarySelector:: gui_touchesEndedHandler(int touchID, cinder::Vec2f touchPnt){
	if (!gui_ObjectTouchIDs.empty()){
		int currentTouchId = gui_ObjectTouchIDs.front();
		if (currentTouchId == touchID){
			//clear the touch ids
			gui_ObjectTouchIDs.clear();
		}

		//if the touch wasn't canceled already by going out of bounds, then trigger the function call
		if (!mTouchCanceled){
		}
		mTouchCanceled = false;
	}


}
*/


bool RotarySelector::containsPoint(cinder::Vec2f point){
	bool inside = false;
	//Check if the point is inside the outer circle
	if (pow((point.x - gui_getCenter().x), 2.0f) + pow((point.y - gui_getCenter().y), 2.0f) < pow(mRadius + mWidth, 2.0f)){
	//	if (pow((point.x - gui_getCenter().x), 2.0f) + pow((point.y - gui_getCenter().y), 2.0f) > pow(mRadius, 2.0f)){
			inside = true;
		}
	//}

	return inside;
}

void RotarySelector::update(){

}

void RotarySelector::draw(){
	
	gl::color(1, 0, 0);
	if (gui_getNumTouches()>0)	gl::color(1, 1, 0);

	
	gl::lineWidth(5.0f);
	gl::pushMatrices();
		gl::translate(gui_getCenter());
			gl::rotate(mRotation);
		gl::translate(-gui_getCenter());
		//Draw Dial
		gl::drawStrokedCircle(gui_getCenter(),mRadius);
		gl::drawStrokedCircle(gui_getCenter(), mRadius + mWidth);
	
		//Draw Button
		gl::color(0, 1, 1);
		gl::drawStrokedCircle(gui_getCenter() + Vec2f(0.0f, -(mRadius + mWidth/2.0f )), 20.0f);

	gl::popMatrices();

}