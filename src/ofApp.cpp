#include "ofApp.h"


// generate disparity map from left and right colour image
// generate depth map and 3d point cloud form depth map
// dan buzzo 2017. http://www.buzzo.com
// http://www.github.com/danbz

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    
    leftImage.load( "left.png");
    rightImage.load( "right.png");
    colorImage.load( "left.png");
    
    leftImage.load( "left-landing.png");
    rightImage.load( "right-landing.png");
    colorImage.load( "left-landing.png");
    
//
//       leftImage.load( "cloud left sml.png");
//        rightImage.load( "cloud right sml.png");
//        colorImage.load( "cloud left sml.png");
//
//    leftImage.load( "ambush_5_left.jpg");
//    rightImage.load( "ambush_5_right.jpg");
//    colorImage.load( "ambush_5_left.jpg");
//
    leftImage.setImageType(OF_IMAGE_GRAYSCALE);
    rightImage.setImageType(OF_IMAGE_GRAYSCALE);
    //disparity.allocate(leftImage.getWidth(), leftImage.getHeight(), OF_IMAGE_GRAYSCALE);

//    imgMatLeft = toCv(leftImage);
//    imgMatRight = toCv(rightImage);
//    imitate(imgMatDisparity, imgMatLeft);
    
    
    paintMesh = true;
    renderStyle=1;
    drawSource=true;
    blur=false;
    s = true;
    erode = false;
    dilate = false;
    vidGrabber.listDevices();
    vidGrabber.setDeviceID(1);
    
    vidGrabber.setUseTexture(true);
    vidGrabber.setPixelFormat(OF_PIXELS_RGB);
    vidGrabber.setDesiredFrameRate(15);
    //vidGrabber.setup(2160, 720);
    vidGrabber.initGrabber(1440 , 576);
    
    
    // gui setup
    
    gui.setup(); // most of the time you don't need a name
    gui.add(minDisparity.setup("minDisparity", 0, 0, 20));
    gui.add(numberOfDisparities.setup("numberOfDisparities", 2, 0, 40));
    gui.add(SADWindowSize.setup("SADWindowSize", 3, 1, 9));
    gui.add(P1.setup("P1", 128, 0, 256));
    gui.add(P2.setup("P2", 256, 16, 266));
    gui.add(disp12MaxDiff.setup("disp12MaxDiff", 20, 1, 80));
    gui.add(preFilterCap.setup("preFilterCap", 4, 0, 10));
    gui.add(uniquenessRatio.setup("uniquenessRatio", 2, 0, 20));
      gui.add(speckleWindowSize.setup("speckleWindowSize", 100, 0, 800));
      gui.add(speckleRange.setup("speckleRange", 5, 1, 20));
    
  
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    vidGrabber.update();
    //vidGrabber.videoSettings();
    //vidGrabber.videoSettings();
//    if(vidGrabber.isFrameNew()){
//        grabbedImage = vidGrabber.getPixels();
//
//        leftImage.cropFrom(grabbedImage, 0, 0, grabbedImage.getWidth()/2, grabbedImage.getHeight());
//        rightImage.cropFrom(grabbedImage, grabbedImage.getWidth()/2 , 0, grabbedImage.getWidth()/2 , grabbedImage.getHeight());
//        colorImage.cropFrom(grabbedImage, 0, 0, grabbedImage.getWidth()/2, grabbedImage.getHeight());
//    }
    
    leftImage.setImageType(OF_IMAGE_GRAYSCALE);
    rightImage.setImageType(OF_IMAGE_GRAYSCALE);
    disparity.allocate(leftImage.getWidth(), leftImage.getHeight(), OF_IMAGE_GRAYSCALE);
    
   // cout << "left depth" << imgMatLeft.depth() << endl;
    imgMatLeft = toCv(leftImage);
    imgMatRight = toCv(rightImage);
    imitate(imgMatDisparity, imgMatLeft);
    
    
    //reprojectImageTo3D(imgMatDisparity, depthMat, <#InputArray Q#>);
  imgMatLeft.convertTo(imgMatLeft, CV_8UC1);
   imgMatRight.convertTo(imgMatRight, CV_8UC1);
   imgMatDisparity.convertTo(imgMatDisparity, CV_16UC1);

    //imgMatLeft.type()
    
    //- Call the constructor for StereoBM
    //int ndisparities = 16*5;   /**< Range of disparity */
    int SADWindowSize = 21; /**< Size of the block window. Must be odd */
    
    //stereoBM(ndisparities, SADWindowSize);
    StereoBM(1, 0 , 21);
    
    
//    stereo.minDisparity=0;
//    stereo.numberOfDisparities =32;
//    stereo.SADWindowSize=3;
//    stereo.P1 =128;
//    stereo.P2 = 256;
//    stereo.disp12MaxDiff = 20;
//    stereo.preFilterCap=16;
//    stereo.uniquenessRatio=1  ;
//    stereo.speckleWindowSize=100;
//    stereo.speckleRange = 20;
//    stereo.fullDP=true;
//    
//    stereo.minDisparity=0;
//    stereo.numberOfDisparities =32;
//    stereo.SADWindowSize=3;
//    stereo.P1 =32;
//    stereo.P2 = 256;
//    stereo.disp12MaxDiff = 20;
//    stereo.preFilterCap=4;
//    stereo.uniquenessRatio=2  ;
//    stereo.speckleWindowSize=100;
//    stereo.speckleRange = 5;
//    stereo.fullDP=true;
    
    stereo.minDisparity= minDisparity*16;
    stereo.numberOfDisparities =numberOfDisparities*16;
    stereo.SADWindowSize=SADWindowSize;
    stereo.P1 =P1;
    stereo.P2 = P2;
    stereo.disp12MaxDiff = disp12MaxDiff;
    stereo.preFilterCap=preFilterCap;
    stereo.uniquenessRatio=uniquenessRatio  ;
    stereo.speckleWindowSize=speckleWindowSize;
    stereo.speckleRange = speckleRange;
    stereo.fullDP=true;


    //StereoVar(USE_AUTO_PARAMS=true);
   // StereoVar();
  //  StereoVar(<#int levels#>, <#double pyrScale#>, <#int nIt#>, <#int minDisp#>, <#int maxDisp#>, <#int poly_n#>, <#double poly_sigma#>, <#float fi#>, <#float lambda#>, <#int penalization#>, <#int cycle#>, <#int flags#>);
    
    //StereoVar(1, 0.5, 1, 0, 32, 5, 1.1, 1.0, 1.0, 1, 0, 4);
    
    //stereo(0, 32, 3, 128, 256, 20, 16, 1, 100, 20, true); example SGBM settings
    //stereoVar.operator()( imgMatLeft, imgMatRight, imgMatDisparity);
    if (s){
        stereoBM.operator()(imgMatLeft, imgMatRight, imgMatDisparity, CV_16S);
    } else {
        stereo.operator()(imgMatLeft, imgMatRight, imgMatDisparity);
    }

    imitate(depthMat, imgMatLeft);
    toOf(imgMatDisparity, disparity);
    
    disparity.update();
    imgMatDisparity.convertTo(depthMat, depthMat.type());
    depth.allocate(leftImage.getWidth(), leftImage.getHeight(), OF_IMAGE_GRAYSCALE);
    toOf(depthMat, depth);
    
    
    ofShortColor c;
  
    for(int y = 0; y < leftImage.getHeight(); y ++) {
        for(int x = 0; x < leftImage.getWidth(); x ++) {
            c =  depth.getColor(x, y * 0.00390625);
            //cout << c << endl;
           //depth.setColor(x, y, CLAMP(c.r * 0.00390625 , 0, 255));
           // depth.setColor(x, y, CLAMP(c.r , 0, 255));
        }
    }
    
    depth.update();
    
    // filter depth image
    
    if (blur){
        //ofxCv::GaussianBlur(depth, 10);
        ofxCv::medianBlur(depth, depth, 10);

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
            v3.set(x   , y  , z*1.5     );
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
        rightImage.draw(20+(leftImage.getWidth()), 10);
        
        disparity.draw(10, 20 + (leftImage.getHeight()));
        depth.draw(20+(leftImage.getWidth()), 20 + (leftImage.getHeight()));
       // grabbedImage.draw(50,50);
        
    } else {
        
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
    
    if(showGui){
        gui.draw();
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
            
        case 'g':
        case 'G':
            showGui=!showGui;
            break;
            
        case 's':
        case 'S':
            s=!s;
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
