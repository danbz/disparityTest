#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    
    leftImage.load( "left.png");
    rightImage.load( "right.png");

    leftImage.setImageType(OF_IMAGE_GRAYSCALE);
    rightImage.setImageType(OF_IMAGE_GRAYSCALE);
    
    disparity.allocate(384, 288, OF_IMAGE_GRAYSCALE);

    imgMatLeft = toCv(leftImage);
    imgMatRight = toCv(rightImage);
    imitate(imgMatDisparity, imgMatLeft);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //stereo.operator()(imgMatLeft, imgMatRight, imgMatDisparity, CV_16S);
    stereo.minDisparity=0;
    stereo.numberOfDisparities =32;
    stereo.SADWindowSize=3;
    stereo.P1 =128;
    stereo.P2 = 256;
    stereo.disp12MaxDiff = 20;
    stereo.preFilterCap=16;
    stereo.uniquenessRatio=1  ;
    stereo.speckleWindowSize=100;
    stereo.speckleRange = 20;
    stereo.fullDP=true;
    
    //stereo(0, 32, 3, 128, 256, 20, 16, 1, 100, 20, true);
    stereo.operator()(imgMatLeft, imgMatRight, imgMatDisparity);

    
    imitate(depthMat, imgMatLeft);
    toOf(imgMatDisparity, disparity);
    
    disparity.update();
    imgMatDisparity.convertTo(depthMat, depthMat.type());
    depth.allocate(384, 288, OF_IMAGE_GRAYSCALE);
    toOf(depthMat, depth);
    
    ofShortColor c;
    for(int y = 0; y < 288; y ++) {
        for(int x = 0; x < 384; x ++) {
            c =  disparity.getColor(x, y);
            depth.setColor(x, y, CLAMP(c.r, 0, 255));
        }
    }
    depth.update();    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    leftImage.draw(10, 10);
    rightImage.draw(400, 10);
    disparity.draw(10, 310);
    depth.draw(400,310);
    //cout << ofGetFrameRate() << endl;

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
