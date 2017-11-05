#include <iostream>
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int scale = 2;

int w1 = 251;
int h1 = 183;

int w2 = w1*scale;
int h2 = h1*scale;

int main() {
    String imageName( "resizeMe.jpg" );
    Mat image = imread( imageName, CV_LOAD_IMAGE_GRAYSCALE );
    if( image.empty() ){
        cout <<  "Could not open or find the image" << endl;
        return -1;
    }
    imshow("original", image);

    //Initialize the variables
    int temp[w2*h2];
    int A, B, C, D, x, y, index, gray;
    float x_ratio = ((float)(w1-1))/w2;
    float y_ratio = ((float)(h1-1))/h2;
    float x_diff, y_diff, ya, yb;
    int offset = 0;

    //Begin the double loop
    for(int i = 0; i < h2; i++){
        for(int j = 0; j < w2; j++){
            x = (int)(x_ratio*j);
            y = (int)(y_ratio*i);
            x_diff = (x_ratio*j)-x;
            y_diff = (y_ratio*i)-y;
            index = y*w1+x;

            // range is 0 to 255 thus bitwise AND with 0xff
            A = image.at<uchar>(index) & 0xff ;
            B = image.at<uchar>(index+1) & 0xff ;
            C = image.at<uchar>(index+w1) & 0xff ;
            D = image.at<uchar>(index+w1+1) & 0xff ;

            // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
            gray = (int)(
                    A*(1-x_diff)*(1-y_diff) +  B*(x_diff)*(1-y_diff) +
                    C*(y_diff)*(1-x_diff)   +  D*(x_diff*y_diff)
            );
            temp[offset++] = gray;
        }
    }

    Mat newImage = Mat(h2, w2, CV_8UC1, (unsigned*)temp);
    imshow("Resized", newImage);
    waitKey(0);
    return 0;
}