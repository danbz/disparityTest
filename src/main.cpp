#include "ofMain.h"
#include "ofApp.h"

// generate disparity map from left and right colour image
// generate depth map and 3d point cloud form depth map
// dan buzzo 2017. http://www.buzzo.com
// http://www.github.com/danbz


//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
