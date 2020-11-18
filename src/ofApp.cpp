#include "ofApp.h"

static float FRAMERATE = 25.;
static int MESSAGESIZE = 1000;

static Boolean COMPRESS = true;
static int COMPRESSION_TURBOJPEG = 0x04;
static int COMPRESSION_JPEG = 0x00;

ofImage imgOut;
ofBuffer ofBuff;
static std::vector<unsigned char> bufImg;


bool bConnected = false;
bool bReceiveComplete = false;
bool bFoundPictureStart = false;
char buff[1000];
int iImagesizeFromHeaderData=0;
int iStartOfImage = 0;

bool bBlink = false;

string sTmp;

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetVerticalSync(true);
    ofSetFrameRate(FRAMERATE);
    bConnected = tcpClient.setup("127.0.0.1", 9999);
    ofLogNotice("connect:" + ofToString(bConnected));
}

void ofApp::exit() {
    // stop the thread
}

void ofApp::update(){
    bBlink=!bBlink;
    /*
    if( bBlink ){
        ofSetBackgroundColor(0, 0, 0);
    }else{
        ofSetBackgroundColor(200, 0, 0);
    }
    */
    
    if(bConnected){
        while(!bReceiveComplete){
            tcpClient.receiveRawBytes( (char*) &buff[0], MESSAGESIZE);
            
            // store receive buffer into vector.
            for(int i=0; i<MESSAGESIZE; i++){
                bufImg.push_back(buff[i]);
            }
            //ofLogNotice(ofToString(char('\xd8')));   // \330
            //ofLogNotice(ofToString(char('\xff')));   // \377
            
            if(bufImg.size()>=MESSAGESIZE){
                
                if(!bFoundPictureStart){
                    //repeatedly check only the last 200 bytes
                    for(int i=bufImg.size()-MESSAGESIZE; i<bufImg.size(); i++){
                        //if(((char)bufImg[i]=='\xff') && ((char)bufImg[i+1]=='\xd8')){
                        if((bufImg[i]==255) && (bufImg[i+1]==216)){
                            bFoundPictureStart=true;
                            iStartOfImage=i;
                            iImagesizeFromHeaderData=(bufImg[i-3]<<8) + bufImg[i-4]-16;
                            //ofLogNotice("ImageStart at " + ofToString(i) + " Size:" + ofToString(iImagesizeFromHeaderData));
                            break;
                        }else{
                            //maybe later... for resource mgmt or such
                            //vecBuff.erase(vecBuff.begin()+1);
                        }
                    }
                }else{
                    //---We found the beginnging of an image in bufImg and
                    //---know its size: iImagesizeFromHeaderData
                    if(iImagesizeFromHeaderData<=0){
                        ofLogNotice("HALT; FALSCH; AUS; ENDE");
                    }else{
                        if(bufImg.size()>= iStartOfImage + iImagesizeFromHeaderData){
                            //There it is...our received image;
                            char tmpBuff[ iImagesizeFromHeaderData ];
                            std::copy(&bufImg[iStartOfImage], &bufImg[iStartOfImage+iImagesizeFromHeaderData], tmpBuff);
                            /*
                             //----also Works, but might be slower
                            for(int i=0; i<iImagesizeFromHeaderData; i++){
                                tmpBuff[i] = *&bufImg[i+iStartOfImage];
                            }
                            */
                            ofBuff.set((tmpBuff), iImagesizeFromHeaderData);
                            imgOut.load(ofBuff);
                            
                            //----
                            iImagesizeFromHeaderData = 0;
                            bReceiveComplete = true;
                            bFoundPictureStart = false;
                            bufImg.resize(0);
                        }else{
                            // keep on recieving
                            //ofSleepMillis(1);
                        }
                    }
                }
            }
        }//while
        bReceiveComplete = false;   //restart receiving
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    if(imgOut.isAllocated()){
        imgOut.draw(10,10);
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
