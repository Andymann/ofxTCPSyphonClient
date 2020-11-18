//
//  ImageLoader.hpp
//  ofxTcpSyphonClient
//
//  Created by Andy Fischer on 17.11.20.
//

//#ifndef ImageLoader_hpp
//#define ImageLoader_hpp

#include <stdio.h>
//#include <ofThread.h>
#include <ofMain.h>

//#endif /* ImageLoader_hpp */
static int MESSAGESIZE = 200;

ofImage imgOut;
ofBuffer ofBuff;
static std::vector<unsigned char> bufImg;


bool bConnected = false;
bool bReceiveComplete = false;
bool bFoundPictureStart = false;
char buff[200];
int iImagesizeFromHeaderData=0;
int iStartOfImage = 0;


class ImageLoader : public ofThread{
    void setup(std::string imagePath){
        //this->path = imagePath;
    }

 void threadedFunction(){
        //ofLoadImage(image, path);
     ofLogNotice("ImageLoader.ThreadedFunction()");
    }
    
public:
    void processBuffer(){
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
                    if(((char)bufImg[i]=='\xff') && ((char)bufImg[i+1]=='\xd8')){
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
                    }
                }
            }
        }
    }

    
};
