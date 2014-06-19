#include "RotaryContentObject.h"

#include <math.h>
using namespace std;
using namespace ci;
using namespace ci::app;
using namespace guiObject;

RotaryContentObject::RotaryContentObject() : BaseGuiObject(),
mSelected(false)
{

}

RotaryContentObject::~RotaryContentObject(){

}


void RotaryContentObject::setup(cinder::Vec2f pos, cinder::Vec2f size){
	gui_setPosition(pos);
	gui_setSize(size.x, size.y);


}


void RotaryContentObject::draw(){
	
	gl::color(0, 1, 0);
	if (mSelected){
		gl::drawSolidRoundedRect(gui_getRect(), 10.0f, 5);
	}
	else{
		gl::drawStrokedRoundedRect(gui_getRect(), 10.0f, 5);
	}
}