
//+---------------------------------------------------------------------------
//
//  Bluecadet Interactive
//  
//	Developer: Paul Rudolph
//	Date:		2014 - 1 - 15
//  Contents:  Code for TUIO driver to be used with the touch manager class
//  Comments:
//
//----------------------------------------------------------------------------

#pragma once
#include "cinder/app/AppBasic.h"
#include "Leap.h"
#include <mutex>
#include "TouchManager.h"

class LeapDriver : public Leap::Listener {
public:
	LeapDriver();
	~LeapDriver();
	void connect(TouchManager* touchMngerPntr );
	void disconnect();

	//LEAP Functions
	virtual void onInit(const Leap::Controller&);
	virtual void onConnect(const Leap::Controller&);
	virtual void onDisconnect(const Leap::Controller&);
	virtual void onExit(const Leap::Controller&);
	virtual void onFrame(const Leap::Controller&);
	virtual void onFocusGained(const Leap::Controller&);
	virtual void onFocusLost(const Leap::Controller&);
	virtual void onDeviceChange(const Leap::Controller&);
	virtual void onServiceConnect(const Leap::Controller&);
	virtual void onServiceDisconnect(const Leap::Controller&);

	virtual void draw();
private:

	struct FingerObj{
		int id;
		cinder::Vec2f position;
		bool touchDown;
		FingerObj() {
			id = -1;
			touchDown = false;
			position = cinder::Vec2f(0.0f, 0.0f);

		}
	};

	std::map<int, FingerObj> mFingerMap;
	std::mutex mFingerMapMutex;

   
	void leapTouchesBegan(cinder::app::TouchEvent event);
    void leapTouchesMoved(cinder::app::TouchEvent event);
    void leapTouchesEnded(cinder::app::TouchEvent event);
   TouchManager *mTouchManagerPntr; 


};