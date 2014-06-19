#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Leap.h"
#include "LeapMath.h"

#include "TouchManager.h"
#include "Mouse.h"
#include "Touch/TouchDrivers/Leap/LeapDriver.h"
#include "RotarySelector.h"
#include "RotaryContentObject.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LeapUIApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

	Leap::Controller leapController;
	//LeapListener	leapListener;

	std::shared_ptr<class RotarySelector> mRotary;
	std::shared_ptr<RotaryContentObject> contentObj1, contentObj2, contentObj3, contentObj4;
	LeapDriver mLeapConnection;
	Mouse mMouseConnection;
};

void LeapUIApp::setup()
{
	mMouseConnection.connect(TouchManager::getInstance());
	mLeapConnection.connect(TouchManager::getInstance());
	leapController.addListener(mLeapConnection);
	
	mRotary = std::shared_ptr<RotarySelector>(new RotarySelector());
	mRotary->setup(Vec2f(500, 500),100.0f,100.0f);
	

contentObj1 = std::shared_ptr<RotaryContentObject>(new RotaryContentObject());
	contentObj1->setup(Vec2f(800, 10),Vec2f(400,400));
	mRotary->addSectionRange(0, 90, contentObj1);



contentObj2 = std::shared_ptr<RotaryContentObject>(new RotaryContentObject());
	contentObj2->setup(Vec2f(800, 800), Vec2f(400, 400));
	mRotary->addSectionRange(90, 180, contentObj2);

	 contentObj3 = std::shared_ptr<RotaryContentObject>(new RotaryContentObject());
	contentObj3->setup(Vec2f(0, 800), Vec2f(400, 400));
	mRotary->addSectionRange(180, 270, contentObj3);

	contentObj4 = std::shared_ptr<RotaryContentObject>(new RotaryContentObject());
	contentObj4->setup(Vec2f(0, 0), Vec2f(400, 400));
	mRotary->addSectionRange(270, 360, contentObj4);

}

void LeapUIApp::mouseDown( MouseEvent event )
{
}

void LeapUIApp::update()
{
}

void LeapUIApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	mLeapConnection.draw();
	mRotary->draw();

	contentObj1->draw();
	contentObj2->draw(); 
	contentObj3->draw();
	contentObj4->draw();
}

CINDER_APP_NATIVE( LeapUIApp, RendererGl )
