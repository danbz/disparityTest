#pragma once

// generate disparity map from left and right colour image
// generate depth map and 3d point cloud form depth map
// dan buzzo 2017. http://www.buzzo.com
// http://www.github.com/danbz

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    ofImage leftImage;
    ofImage rightImage, colorImage;
    ofImage depth;
    ofShortImage disparity;
    
    cv::Mat imgMatLeft;
    cv::Mat imgMatRight;
    cv::Mat imgMatDisparity;
    cv::Mat depthMat;

    //cv::StereoBM stereo;
    cv::StereoSGBM stereo;
    
    cv::StereoVar stereoVar;
    
    ofEasyCam easyCam;
    ofMesh mesh;
    int renderStyle;
    bool paintMesh, drawSource, blur, erode, dilate;
};
