
//opencv
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//std
#include <iostream>
#include <cstdlib>

//main

void obtain_intensity(cv::Mat* img){
  // get intensity of the central pixel. Ordered as BGR
  cv::Vec3b pixel_intensity = img->at<cv::Vec3b>((img->rows)/2, (img->cols)/2);
  std::cout << "RGB: " 	<< (unsigned int)pixel_intensity[2] << ","
              << (unsigned int)pixel_intensity[1] << ","
              << (unsigned int)pixel_intensity[0] << std::endl;
}

cv::Mat draw_blue_point(cv::Mat image){
  cv::Mat img;
  image.copyTo(img);
  //ordered as BGR
  cv::Vec3b pixel_intensity = img.at<cv::Vec3b>(img.rows/2, img.cols/2);
  pixel_intensity[0] = 255;
  pixel_intensity[1] = 0;
  pixel_intensity[2] = 0;
  img.at<cv::Vec3b>(img.rows/2, img.cols/2) = pixel_intensity;

  return img;

}

cv::Mat green_rectangle(cv::Mat image,int width,int height){
  cv::Mat img;
  image.copyTo(img);
  int x = img.cols/2;
  int y = img.rows/2;
  cv::Point pt1(x - width/2, y - height/2);// and its bottom right corner.
  cv::Point pt2(x + width/2, y + height/2);// These two calls... //RGB
  cv::rectangle(img, pt1, pt2, cv::Scalar(0, 255, 0),-1); //cv::imshow("Output Window", img);

  return img;

}


cv::Mat convert_gray(cv::Mat image, int width, int height){
  cv::Mat img;
  image.copyTo(img);
  int x= img.cols/2 -width/2;
  int y= img.rows/2 - height/2;
  cv::Mat tmp;
  cv::Rect rect(x, y, width, height); //defining the rectangle where roi will be placed
  //cv::Mat roi(mask, rect); //defining region of interest
  cv::cvtColor(img,tmp, CV_BGR2GRAY);
  cv::cvtColor(tmp,tmp, CV_GRAY2BGR);
  cv::rectangle(tmp, rect, cv::Scalar(0, 255, 0));  //rectangle(imageORBmask, 10, 10, 255, int thickness=1, int lineType=8, int shift=0)¶ //drawing green rectangle
  cv::Rect roi(x,  y, width, height);
  tmp(roi).copyTo(img(roi));

  return img;
}


cv::Mat inversion (cv::Mat image,int width, int height){
  cv::Mat img;
  image.copyTo(img);
  int x= img.cols/2 -width/2;
  int y= img.rows/2 - height/2;
  cv::Mat roi = img(cv::Rect(x,y,width,height));
  roi =  cv::Scalar::all(255) - roi;
  //std::cout << "roi" << roi;
  return img;
}

cv::Mat blue_filtering(cv::Mat image,int width, int height){
  cv::Mat img;
  image.copyTo(img);
  cv::Mat img_output;
  int x= img.cols/2 -width/2;
  int y= img.rows/2 - height/2;
  cv::Scalar low_blue = cv::Scalar(180,84,0);
  cv::Scalar high_blue = cv::Scalar(255,181,175);
  cv::Mat maskBGR;
  cv::Mat roi = img(cv::Rect(x,y,width,height));
  cv::inRange(img, low_blue, high_blue, maskBGR);
  cv::bitwise_and(img,img,img_output, maskBGR);
  //img_output(roi).copyTo(img(roi)); //doesn't work!!
  return img_output;
}

cv::Mat red_filtering(cv::Mat image,int width, int height){
  cv::Mat img;
  image.copyTo(img);
  cv::Mat img_output;
  int x= img.cols/2 -width/2;
  int y= img.rows/2 - height/2;
  cv::Scalar low_red = cv::Scalar(58,39,114);
  cv::Scalar high_red = cv::Scalar(189,115,230);
  cv::Mat maskBGR;
  cv::Mat roi = img(cv::Rect(x,y,width,height));
  cv::inRange(img, low_red, high_red, maskBGR);
  cv::bitwise_and(img, img, img_output, maskBGR);
  return img_output;

}
cv::Mat green_filtering(cv::Mat image ,int width, int height){
  cv::Mat img;
  image.copyTo(img);
  cv::Mat img_output;
  int x= img.cols/2 -width/2;
  int y= img.rows/2 - height/2;
  cv::Scalar low_green = cv::Scalar(29,107,66);
  cv::Scalar high_green = cv::Scalar(163,236,210);
  cv::Mat maskBGR;
  cv::Mat roi = img(cv::Rect(x,y,width,height));
  cv::inRange(img, low_green, high_green, maskBGR);
  cv::bitwise_and(img, img, img_output, maskBGR);
  return img_output;

}

int main(int argc, char *argv[])
{
  cv::VideoCapture camera; //OpenCV video capture object
  cv::Mat image; //OpenCV image object
	int cam_id; //camera id . Associated to device number in /dev/videoX
	int user_key; //user pressed key to quit
	cv::Vec3b pixel_intensity; //pixel RGB intensity
	//check user args
	switch(argc)
	{
		case 1: //no argument provided, so try /dev/video0
			cam_id = 0;
			break;
		case 2: //an argument is provided. Get it and set cam_id
			cam_id = atoi(argv[1]);
			break;
		default:
			std::cout << "Invalid number of arguments. Call program as: webcam_capture [video_device_id]. " << std::endl;
			std::cout << "EXIT program." << std::endl;
			break;
	}

	//advertising to the user
	std::cout << "Opening video device " << cam_id << std::endl;

    //open the video stream and make sure it's opened
    if( !camera.open(cam_id) )
	{
        std::cout << "Error opening the camera. May be invalid device id. EXIT program." << std::endl;
        return -1;
    }

    //capture loop. Out of user press a key
    while(1)
	{
		//Read image and check it. Blocking call up to a new image arrives from camera.
        if(!camera.read(image))
		{
            std::cout << "No frame" << std::endl;
            cv::waitKey();
        }


//Change an area around the central pixel to produce some visual effect,
//such as color inversion, color filtering, or other effect you imagine.
int width = 200;
int height= 400;
obtain_intensity(&image);
cv::Mat blue_point = draw_blue_point(image);
cv::Mat green_rect = green_rectangle(image,width,height);
cv::Mat greyMat = convert_gray(image, width,height);
cv::Mat inv =inversion(image,width,height);



//Adding color filtering.
cv::Mat bluefiltering = blue_filtering(image,width,height);
cv::Mat redfiltering = red_filtering(image,width,height);
cv::Mat greenfiltering =green_filtering(image,width,height);


//***********Showing different images created *************//

cv::Mat Output1;
cv::Mat Output2;

cv::Mat matArray[3] = {image,greyMat,inv};
cv::hconcat(matArray,3,Output1);
cv::Mat matArray2[3] = {bluefiltering,redfiltering,greenfiltering};
cv::hconcat(matArray2,3,Output2);
cv::imshow("Rectangles: green, grey, inv", Output1);
cv::imshow("Filtering (BGR): blue, red, green", Output2);



		//Waits 30 millisecond to check if 'q' key has been pressed. If so, breaks the loop. Otherwise continues.
    	if( (unsigned char)(cv::waitKey(30) & 0xff) == 'q' ) break;
    }
}
