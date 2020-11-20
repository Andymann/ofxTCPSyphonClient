//
//  imageThread.h
//  ofxTcpSyphonClient
//
//  Created by Andy Fischer on 19.11.20.
//
#pragma once

#include "ofMain.h"


class imageThread : public ofThread{
public:
    
    
    
    ~imageThread(){
        if (isThreadRunning()) stopThread();
    }
    
    void setup(int pMessageSize){
        //receiver.setup(port);
        MESSAGESIZE = pMessageSize;
        bConnected = tcpClient.setup("127.0.0.1", 9999);
        ofLogNotice("connect:" + ofToString(bConnected));
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
                if(bConnected){
                    if(!bReceiveComplete){
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
//                                    bBlink=!bBlink;
                                    bufImg.resize(0);
                                    bReceiveComplete=true;
                                    bFoundPictureStart=false;
                                    
                                }else{
                                    if(bufImg.size()>= iStartOfImage + iImagesizeFromHeaderData){
                                        //There it is...our received image;
                                        char tmpBuff[ iImagesizeFromHeaderData ];
                                        std::copy(&bufImg[iStartOfImage], &bufImg[iStartOfImage+iImagesizeFromHeaderData], tmpBuff);
                                        
                                         //----also Works, but might be slower
                                        //for(int i=0; i<iImagesizeFromHeaderData; i++){
                                        //    tmpBuff[i] = *&bufImg[i+iStartOfImage];
                                        //}
                                        bCopyingToBuffer=true;
                                        ofBuff.set((tmpBuff), iImagesizeFromHeaderData);
                                        bCopyingToBuffer=false;
                                        //imgInThread.load(ofBuff);
                                        ///ofBufferToFile("data.jpg", ofBuff);
                                        
                                        
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
                    sleep(1);
                }
            }
            unlock();
        }
    }
    
    ofBuffer getImageBuffer(){
        //if(bCopyingToBuffer==false){
            return ofBuff;
        //}
    }
    
    //ofEvent<ofxOscMessage> messageReceived;
    ofImage imgInThread;
    vector<unsigned char>bufImg;
    ofxTCPClient tcpClient;
    bool bCopyingToBuffer=false;
    
private:
    int MESSAGESIZE;
    bool bConnected = false;
    bool bReceiveComplete = false;
    bool bFoundPictureStart = false;
    char buff[1000];
    int iImagesizeFromHeaderData=0;
    int iStartOfImage = 0;
    
    //ofImage imgOut;
    ofBuffer ofBuff;

    
    
};
