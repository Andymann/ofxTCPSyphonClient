#include "ofApp.h"

static float FRAMERATE = 25.;
static int MESSAGESIZE = 1000;

static bool COMPRESS = true;
static int COMPRESSION_TURBOJPEG = 0x04;
static int COMPRESSION_JPEG = 0x00;


bool bBlink = true;
string sTmp;
ofImage imgOutFromThread;

vector<ofBuffer> vecImg;

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetVerticalSync(true);
    ofSetFrameRate(FRAMERATE);
    //bConnected = tcpClient.setup("127.0.0.1", 9999);
//    ofLogNotice("connect:" + ofToString(bConnected));
    imgThread.setup(MESSAGESIZE);
    imgThread.start();
}

void ofApp::exit() {
    // stop the thread
    imgThread.stop();
    imgThread.waitForThread();
    
}

void ofApp::update(){
    
    if(vecImg.size()<2){
        vecImg.push_back(imgThread.getImageBuffer());
    }else{
        if(vecImg[0].size()>1){
            imgOutFromThread.load(vecImg[0]);
        }
        vecImg.erase(vecImg.begin());
    }
    
    
    
    //if(tmpImg){
    //    imgOutFromThread.load(tmpImg);
    //}
    
    if( bBlink ){
        ofSetBackgroundColor(0, 0, 0);
    }else{
        ofSetBackgroundColor(200, 0, 0);
    }
    
    //imgOut = imgThread.imgInThread;
    
    
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    if(imgOutFromThread.isAllocated()){
        imgOutFromThread.draw(10,10);
        //bReceiveComplete = false;   //restart receiving
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
