
#include "LeapDriver.h"

using namespace std;
using namespace ci;
using namespace ci::app;

using namespace Leap;

LeapDriver::LeapDriver(){
		mTouchManagerPntr=NULL;
		
}

void LeapDriver::connect(TouchManager *touchMngerPntr){
	//Connect to the application window touch event signals
	ci::app::WindowRef window = cinder::app::getWindow();

	// Connect to the Touch Manager
	mTouchManagerPntr = touchMngerPntr;
}

LeapDriver::~LeapDriver(){
	//disconnect();
}

void LeapDriver::disconnect(){
	//Disconnect from the TUIO TOUCH signals
	
	// Remove the pointer to the touch manager
	mTouchManagerPntr=NULL;
}

   
void LeapDriver::leapTouchesBegan(cinder::app::TouchEvent event){
	//console()<<"TUIO touches BEGAN"<<endl;
	if(mTouchManagerPntr){
		for(TouchEvent::Touch touch :event.getTouches() ){
			mTouchManagerPntr->touchesBegan(touch.getId(),touch.getPos());
		}
		
	}
}
void LeapDriver::leapTouchesMoved(cinder::app::TouchEvent event){
	//console()<<"TUIO touches MOVED"<<endl;
	if(mTouchManagerPntr){
		for(TouchEvent::Touch touch :event.getTouches() ){
			mTouchManagerPntr->touchesMoved(touch.getId(),touch.getPos());
		}
	}
}
void LeapDriver::leapTouchesEnded(cinder::app::TouchEvent event){
	//console()<<"TUIO touches ENDED"<<endl;
		
	if(mTouchManagerPntr){
		for(TouchEvent::Touch touch :event.getTouches() ){
			mTouchManagerPntr->touchesEnded(touch.getId(),touch.getPos());
		}
	}
}


//LEAP STUFF

/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/





const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };
const std::string stateNames[] = { "STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END" };

void LeapDriver::onInit(const Controller& controller) {
	console() << "Initialized" << std::endl;
}

void LeapDriver::onConnect(const Controller& controller) {
	console() << "Connected" << std::endl;
	//controller.enableGesture(Gesture::TYPE_CIRCLE);
	//controller.enableGesture(Gesture::TYPE_KEY_TAP);
	//controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	//controller.enableGesture(Gesture::TYPE_SWIPE);
}

void LeapDriver::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	console() << "Disconnected" << std::endl;
}

void LeapDriver::onExit(const Controller& controller) {
	console() << "Exited" << std::endl;
}

void LeapDriver::onFrame(const Controller& controller) {
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
	const InteractionBox iBox = frame.interactionBox();
	std::map<int, FingerObj> frameFingers;
	
	

	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;


		// Get fingers

		const FingerList fingers = hand.fingers();
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;
			if (finger.type() == 1){//Is it the index finger

				//Get the finger point in relation to the application window
				Vector normalPnt = iBox.normalizePoint(finger.stabilizedTipPosition());
				float x = normalPnt.x * getWindowWidth();
				float y = getWindowHeight() - normalPnt.y * getWindowHeight();

				//Check if we had the finger object in the old frame
				std::map<int, FingerObj>::iterator foundItr = mFingerMap.find(finger.id());


				FingerObj currentFinger;
				if (foundItr != mFingerMap.end()){ //WE are already tracking this finger
					currentFinger.id = (*foundItr).first;
					currentFinger.touchDown = (*foundItr).second.touchDown;
					currentFinger.position = Vec2f(x, y);

				}
				else{
					currentFinger.id = finger.id();
					currentFinger.position = Vec2f(x, y);

				}


				//If the touch was down on the last frame
				if (currentFinger.touchDown == true){

					if (finger.touchZone() != Leap::Pointable::Zone::ZONE_TOUCHING){
						currentFinger.touchDown = false;
						mTouchManagerPntr->touchesEnded(currentFinger.id, Vec2f(currentFinger.position.x, currentFinger.position.y));

					}
					else{//check if its still down, which means its just a move.

						mTouchManagerPntr->touchesMoved(currentFinger.id, Vec2f(currentFinger.position.x, currentFinger.position.y));

					}


				}
				else{//Finger was not down on the last frame or the finger was not being tracked

					//If the on the last frame this finger was down and its still down, the touch has only moved
					if (finger.touchZone() == Leap::Pointable::Zone::ZONE_TOUCHING && finger.isExtended()){
						currentFinger.touchDown = true;
						mTouchManagerPntr->touchesBegan(currentFinger.id, Vec2f(currentFinger.position.x, currentFinger.position.y));
					}
				}
				frameFingers[finger.id()] = currentFinger;
			}

		}
	}
	
	
	mFingerMapMutex.lock();
		mFingerMap.swap(frameFingers);
	mFingerMapMutex.unlock();

}

void LeapDriver::onFocusGained(const Controller& controller) {
	console() << "Focus Gained" << std::endl;
}

void LeapDriver::onFocusLost(const Controller& controller) {
	console() << "Focus Lost" << std::endl;
}

void LeapDriver::onDeviceChange(const Controller& controller) {
	console() << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		console() << "id: " << devices[i].toString() << std::endl;
		console() << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}
}

void LeapDriver::onServiceConnect(const Controller& controller) {
	console() << "Service Connected" << std::endl;
}

void LeapDriver::onServiceDisconnect(const Controller& controller) {
	console() << "Service Disconnected" << std::endl;
}

void LeapDriver::draw(){
	mFingerMapMutex.lock();
	for (std::pair<int, FingerObj> finger : mFingerMap){
		FingerObj fingerObj = finger.second;
			if (fingerObj.touchDown)gl::color(1, 0, 0);
		else					gl::color(0, 0, 1);

		gl::drawStrokedCircle(fingerObj.position, 40);
		gl::drawString(to_string(finger.first), fingerObj.position);


	}
	mFingerMapMutex.unlock();
}
