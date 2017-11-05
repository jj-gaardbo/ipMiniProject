#include <iostream>
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main() {
    String imageName( "resizeMe.jpg" );
    Mat image = imread( imageName, CV_LOAD_IMAGE_GRAYSCALE );
    if( image.empty() ){
        cout <<  "Could not open or find the image" << endl;
        return -1;
    }
    imshow("original", image);





    waitKey(0);
    return 0;
}