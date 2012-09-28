// include all the needed header

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "PracticalSocket.h"  
#include <cstdlib>            
#include <iostream>
#include <stdio.h>
#include <pthread.h>


// all the namespace being used 
using namespace std;
using namespace cv;

// prototype declaration
void *ClientServer(void *arg);
void detectAndDisplay( Mat frame );
int display_caption( char* caption );
// global variables used
const int RCVBUFSIZE = 32;
String face_cascade_name = "/Users/bhavishagarwal/Downloads/haarcascade3.xml";
CascadeClassifier face_cascade;
string window_name = "Capture";
string window_name2 = "drawing image";
Mat bart_image;
Mat HSV_image;
Mat threshold_image;
RNG rng(12345);
bool isHandPresent = false;
bool leftsidepresent = false;
int main( int argc, const char** argv )
{
    CvCapture* capture;
    Mat frame;
    bart_image = cvLoadImage("/Users/bhavishagarwal/Desktop/GreyTheshold/GreyTheshold/bart.jpg");

    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    cout << "hello" << endl;
    //-- 2. Read the video stream
    capture = cvCaptureFromCAM(0);
    cout << "the capture is " << capture << endl;
    if( capture )
        {
        while( true )
            {
            frame = cvQueryFrame( capture );
            
            //-- 3. Apply the classifier to the frame
            if( !frame.empty() )
                { detectAndDisplay( frame ); 
                    imshow(window_name2, bart_image);
                }
            else
                { printf(" --(!) No captured frame -- Break!"); break; }
            
            int c = waitKey(10);
            if( (char)c == 'c' ) { break; }
            }
        }
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
    std::vector<Rect> faces;
    Mat frame_gray;
    
    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    
    equalizeHist( frame_gray, frame_gray );
                                            
                                             
                                            
    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces,3,3, 0|CV_HAAR_SCALE_IMAGE, Size(100, 100) );
    Rect rec;
    
    double rectangle = 0 ;
    if (faces.size()> 0){
        // classifier success , hence save the current state. 
        if (isHandPresent == false){
            putText(frame, "Hand is closed", Point(100,100), FONT_HERSHEY_SIMPLEX, 3.1, Scalar(255,255,0));
            isHandPresent = true;
            
            // upload the image here when this happens and go to sleep since we dont wont errors also to take place in the code.
            // the frame 
        }
        for( int i = 0; i < faces.size(); i++ )
            {
            if (rectangle < faces[i].area())
                {
                rec = Rect(faces[i]);
                }
            }
        cout << rec.x << "  " << rec.y << endl;
    }else {
        //  classifier failed to find the hand
        if (isHandPresent){    
            putText(frame, "Hand is opened", Point(100,100), FONT_HERSHEY_SIMPLEX, 3.1, Scalar(255,255,0));
            
            isHandPresent = false;
            system("curl http://www.bhavishh.com/downloadphotoaction");
            cout << "command done" << endl;
           
           
        }
    }
    //    Draw the rectange in the frame
    cv::rectangle(frame, rec, Scalar(250,0,250));
    //-- Show what you got
    imshow( window_name, frame );
}


