//
//  imageThread.h
//  ofxTcpSyphonClient
//
//  Created by Andy Fischer on 19.11.20.
//
#pragma once

#include "ofMain.h"
//#include "ofxOsc.h"

class imageThread : public ofThread{
public:
    
    ~imageThread(){
        if (isThreadRunning()) stopThread();
    }
    
    void setup(){
        //receiver.setup(port);
    }
    
    void start(){
        //startThread(true, false);
        startThread();
    }
    
    void stop(){
        stopThread();
    }
    
    void threadedFunction(){
        while (isThreadRunning()){
            if (lock()){
                //while (receiver.hasWaitingMessages()){
                    //if (receiver.getNextMessage(&msg)){
                        //ofxOscMessage m = msg;
                        //ofNotifyEvent(messageReceived, m, this);
                    //}
                //}
            }
            unlock();
        }
    }
    
    //ofEvent<ofxOscMessage> messageReceived;
    
private:
    
    //ofxOscReceiver receiver;
    //ofxOscMessage msg;
    
};
