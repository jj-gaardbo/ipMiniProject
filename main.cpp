#include <iostream>
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

Mat image;
float scale = 5;

int * resizePixels(int temp[], const int pixels[],int w1,int h1,int w2,int h2) {
    int x_ratio = (int)((w1<<16)/w2) +1;
    int y_ratio = (int)((h1<<16)/h2) +1;
    int sX, sY ;
    for (int i = 0; i < h2; i++) {
        for (int j = 0; j < w2; j++) {
            sX = ((j*x_ratio)>>16) ;
            sY = ((i*y_ratio)>>16) ;
            temp[(i*w2)+j] = pixels[(sY*w1)+sX] ;
        }
    }
    return temp ;
}

int * resizeImage(int temp[], const int pixels[] , int w, int h, int w2, int h2) {
    int A, B, C, D, sX, sY, index, gray ;
    float x_ratio = ((float)(w-1))/w2 ;
    float y_ratio = ((float)(h-1))/h2 ;
    float x_diff, y_diff;
    int offset = 0 ;
    for (int i = 0; i < h2; i++) {
        for (int j=0;j<w2;j++) {
            sX = (int) (x_ratio * j) ;
            sY = (int) (y_ratio * i) ;
            x_diff = (x_ratio * j) - sX;
            y_diff = (y_ratio * i) - sY;
            index = sY * w + sX ;

            // range is 0 to 255 thus bitwise AND with 0xff
            A = pixels[index] & 0xff;
            B = pixels[index+1] & 0xff;
            C = pixels[index+w] & 0xff;
            D = pixels[index+w+1] & 0xff;

            // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
            gray = (int)(
                    A*(1-x_diff)*(1-y_diff) +  B*(x_diff)*(1-y_diff) +
                    C*(y_diff)*(1-x_diff)   +  D*(x_diff*y_diff)
            ) ;

            temp[offset++] = gray ;
        }
    }
    return temp;
}

int main() {
    String imageName( "resizeMe.jpg" );
    image = imread( imageName, CV_LOAD_IMAGE_GRAYSCALE );
    if( image.empty() ){
        cout <<  "Could not open or find the image" << endl;
        return -1;
    }
    imshow("original", image);

    int w = image.cols;
    int h = image.rows;

    int w2 = static_cast<int>((int)w * scale);
    int h2 = static_cast<int>((int)h * scale);

    int data[w2*h2];
    unsigned char* dataMat = image.data;
    resizeImage(data, reinterpret_cast<int *>(dataMat), w, h, w2, h2);

    Mat newImage = Mat(h2, w2, CV_8UC1, data);
    imshow("Resized", newImage);

    Size size(w2, h2);
    Mat openCVScale;
    resize(image, openCVScale, size);
    imshow("OpenCVScale", openCVScale);

    waitKey(0);
    return 0;
}
