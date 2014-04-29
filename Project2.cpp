// Project2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <cxcore.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

///////////////////////////////////////////////////
using namespace std;






int main(int argc, char* argv[])
{
   CvSize size640x480 = cvSize(640, 480); // 640 x 480 is the window size
                                          // Need to set webcam to 640 x 480 as well

   CvCapture* p_capwebcam;         //Webcam video stream will be assigned to this pointer object

   IplImage* p_imgOriginal;        // Pointer to an image structure, this will be the input image from the webcam
   IplImage* p_imgProcessed;        // Pointer to an image structure, this will be the processed image

   CvMemStorage* p_strStorage;    //storage variable to pass into cvHoughCircles()

   CvSeq* p_seqCircles;         //Pointer to an OpenCv sequence,  will be returned by cvHoughCircles()and will contain all circles. 
                                // It stores a series of circles

   float* p_fltXYRadius;        //Pointer to a 3 element array of float
                               //[0] => x, [1] => y, [2] => radius

   int i;                   // loop counter

   char charCheckForEscKey = 'A'; //Escape Key
     

   p_capwebcam = cvCaptureFromCAM(0); // 0 referes to 1st webcam, use 1 for 2nd webcam, 2 for 3rd webcam and so on...

   if (p_capwebcam == NULL)   //Capture is not successfull 
   {
	   cout<<"Capture is Null"<<endl;
       getchar();
	   return(-1);
   }

 cvNamedWindow("Original", CV_WINDOW_AUTOSIZE); //Original image from webcam
 cvNamedWindow("Processed", CV_WINDOW_AUTOSIZE); //Processed image

 p_imgProcessed = cvCreateImage(size640x480,   // Variable declared earlier
 								IPL_DEPTH_8U, //8 bit colour depth 
                                1);            // 1 channel (grey scale) , use 3 for colour image

   while(1)                               
   
   {
             p_imgOriginal = cvQueryFrame(p_capwebcam);   //get frame from webcam
			 if (p_imgOriginal == NULL) 
			 {cout<<"Error: Frame is Null"<<endl;   //if frame did not get captured successfully 
			                             getchar();
                                         break;
										}
                                  
         cvInRangeS(p_imgOriginal,  //function input
				CV_RGB(175, 0, 0),   //min filtering value
				CV_RGB(256, 100, 100), // max filtering value
				p_imgProcessed );       //output

		 p_strStorage = cvCreateMemStorage(0); //allocate necessary memory storage variable to pass into cvHoughCircles()
		 
		 cvSmooth(p_imgProcessed, p_imgProcessed, CV_GAUSSIAN, 9, 9 ); //Smooth's the processed image. Makes it easier to pick out circles
         

          //Returns all circles in the processed image 
		 p_seqCircles = cvHoughCircles (p_imgProcessed,  //input image 
										p_strStorage,    //memory storage
										CV_HOUGH_GRADIENT, //2- pass algorithm
										 2, // size of image
										p_imgProcessed->height / 4, //min. distance between pixels
										100,                     //High Threshold
										50,                     //Low Threshold
										10,                      // min raidus
										400);                  // max radius

		 for (i = 0; i <  p_seqCircles->total; i++)    //For each element in sequential circles
		 {  p_fltXYRadius = (float*)cvGetSeqElem(p_seqCircles, i);
		 
		   cout<<"X-Coordinate = "<<p_fltXYRadius[0]<<"   Y-Coordinate = " <<p_fltXYRadius[1] <<"   Radius = "<<p_fltXYRadius[2]<<endl;
		  
		   //Draw a small green circle at the radius of the detected object
		   cvCircle(p_imgOriginal, 
					cvPoint(cvRound(p_fltXYRadius[0]), cvRound(p_fltXYRadius[1])),
                    3,
					CV_RGB(0, 255, 0),
					CV_FILLED);
					
             //Draw a red circle around the detected object
		   cvCircle(p_imgOriginal, 
					cvPoint(cvRound(p_fltXYRadius[0]), cvRound(p_fltXYRadius[1])),
                    cvRound(p_fltXYRadius[2]),
					CV_RGB(255, 0, 0),
					3);
					
		 
		 }
      cvShowImage("Original", p_imgOriginal);
      cvShowImage("Processed", p_imgProcessed);

     
    if (char (charCheckForEscKey) == 27) break;
    }
 
  cvReleaseMemStorage(&p_strStorage);
  cvReleaseCapture(&p_capwebcam);

  cvDestroyWindow("Original");
  cvDestroyWindow("Processed");

  return(0);

}
