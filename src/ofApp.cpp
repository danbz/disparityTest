#include "ofApp.h"


// generate disparity map from left and right colour image
// generate depth map and 3d point cloud form depth map
// dan buzzo 2017. http://www.buzzo.com
// http://www.github.com/danbz

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    
    //3264 × 2448
    
    leftImage.load( "left.png");
    rightImage.load( "right.png");
    colorImage.load( "left.png");
    
    leftImage.setImageType(OF_IMAGE_GRAYSCALE);
    rightImage.setImageType(OF_IMAGE_GRAYSCALE);
    
    disparity.allocate(384, 288, OF_IMAGE_GRAYSCALE);
    //disparity.allocate(1024, 436, OF_IMAGE_GRAYSCALE);

    imgMatLeft = toCv(leftImage);
    imgMatRight = toCv(rightImage);
    imitate(imgMatDisparity, imgMatLeft);
    
    
    paintMesh = true;
    renderStyle=1;
    drawSource=true;
    blur=false;
    erode = false;
    dilate = false;
    
    
    //disparity filter
    
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
    
    // filter depth image
    
    if (blur){
        ofxCv::GaussianBlur(depth, 5);
    }
    if (erode){
        ofxCv::erode(depth, depth, 2);
    }
    
    if (dilate) {
        ofxCv::dilate(depth, depth, 2);
    }
    
    
    // create mesh from derived depth image
    ofColor col;
    ofShortColor zGrey = 0;
    
    int step =1;
    int width = depth.getWidth();
    int height = depth.getHeight();
    int z = 0;
    ofVec3f v3;
    for(int y = 0; y < height; y += step) {
        for(int x = 0; x < width; x +=  step) {
            zGrey = depth.getPixels()[x+y*width];
            z = zGrey.r;
            v3.set(0,0,0);
            if (paintMesh) {
                col = (colorImage.getColor(x,y)); // getting RGB from ofShortImage
            } else {
                float h = ofMap(z, 0, 255, 0, 255, true);
                col.setHsb(h, 255, 255);
            }
           
            // v3.set((x - (width)*0.002)  ,(y -(height)*0.002) , 255-z*1.0 );
            v3.set(x   , y  , z*2.5     );
            mesh.addVertex(v3);
            mesh.addColor(col);
        }
    } // end create mesh
    
    //triangulate mesh
    int meshW =  width/step ;
    int meshH = height/step;
    for (int y = 0; y<height-step; y+= step){
        for (int x=0; x<width-step; x+= step){
            v3.set(0,0,0);
           
            mesh.addIndex(x+y*meshW);               // 0
            mesh.addIndex((x+1)+y*meshW);           // 1
            mesh.addIndex(x+(y+1)*meshW);           // 10
            
            mesh.addIndex((x+1)+y*meshW);           // 1
            mesh.addIndex((x+1)+(y+1)*meshW);       // 11
            mesh.addIndex(x+(y+1)*meshW);           // 10
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (drawSource){
        leftImage.draw(10, 10);
        rightImage.draw(400, 10);
        disparity.draw(10, 310);
        depth.draw(400,310);
    } else {
        //cout << ofGetFrameRate() << endl;\\\
        
        switch (renderStyle) { //set render style
            case 1:
                mesh.setMode(OF_PRIMITIVE_POINTS);
                break;
                
            case 2:
                mesh.setMode(OF_PRIMITIVE_TRIANGLES);
                break;
                
            case 3:
                mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
                break;
        }
        
        easyCam.begin();
        glPointSize(4);
        ofPushMatrix();
        ofScale(1, -1, 1);  // the projected points are 'upside down' and 'backwards'
        ofTranslate(-200, -200, -200); // center the points a bit
        glEnable(GL_DEPTH_TEST);
        
        //mesh.drawVertices();
        //mesh.drawFaces();
        ofSetColor( 255, 255, 255);  //set render colour for unpainted points, faces and lines
        mesh.draw();
        glDisable(GL_DEPTH_TEST);
        mesh.clear();
        ofPopMatrix();
        easyCam.end();
        
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
            
        case '1':
            renderStyle=1;
            break;
            
        case '2':
            renderStyle=2;
            break;
            
        case '3':
            renderStyle=3;
            break;
                    
            
        case 'p':
        case 'P':
            paintMesh=!paintMesh;
            break;
            
            
        case 'h':
        case 'H':
            easyCam.reset();//reset easycam settings to re-centre 3d view
            break;
            
        case 'f':
        case 'F':
            ofToggleFullscreen();
            break;
            
        case 'r':
        case 'R':
            drawSource=!drawSource;
            break;
            
        case 'b':
        case'B':
            blur=!blur;
            break;
            
        case 'e':
        case 'E':
            erode=!erode;
            break;
            
        case 'd':
        case 'D':
            dilate=!dilate;
            break;
            
            
            
    }

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
