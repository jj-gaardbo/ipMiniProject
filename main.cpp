#include <iostream>
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

Mat image;
float scale = 2;

int * resizePixels(int temp[], int pixels[], int w1, int h1, int w2, int h2) {
    int x_scaleFactor = ((w1<<16)/w2) +1;
    int y_scaleFactor = ((h1<<16)/h2) +1;
    int sX, sY ;
    for (int i = 0; i < h2; i++) {
        for (int j = 0; j < w2; j++) {
            sX = j * x_scaleFactor >> 16;
            sY = i * y_scaleFactor >> 16;
            temp[(i*w2)+j] = pixels[(sY*w1)+sX] ;
        }
    }
    return temp ;
}

int * resizeImage(int temp[], int pixels[], int w, int h, int w2, int h2) {
    int A, B, C, D, sX, sY, index, gray ;

    //If we are scaling up. Than the scaling factor is smaller than 1.
    double x_scaleFactor = scale > 1 ? ((double)(w-1))/w2 : (double)w/w2;
    double y_scaleFactor = scale > 1 ? ((double)(h-1))/h2 : (double)h/h2;

    double delta_R, delta_C;

    for (int i = 0; i < h2; i++) {

        for (int j = 0; j < w2; j++) {

            sX = (int) (x_scaleFactor * j) ;
            sY = (int) (y_scaleFactor * i) ;

            delta_R = (x_scaleFactor * j) - sX;
            delta_C = (y_scaleFactor * i) - sY;

            index = (sY * w) + sX ;

            // range is 0 to 255 thus bitwise AND with 0xff
            A = pixels[index] & 0xff;
            B = pixels[index] & 0xff;
            C = pixels[index+w] & 0xff;
            D = pixels[index+w+1] & 0xff;

            gray = (int)(
                    A*(1-delta_R)*(1-delta_C) +
                    B*(delta_R)*(1-delta_C) +
                    C*(1-delta_R)*(delta_C) +
                    D*(delta_R*delta_C)
            ) ;

            temp[(i*w2)+j] = gray ;

        }

    }

    return temp;

}

int main() {
    String imageName( "resizeMe2.jpg" );
    image = imread( imageName, CV_LOAD_IMAGE_GRAYSCALE );
    if( image.empty() ){
        cout <<  "Could not open or find the image" << endl;
        return -1;
    } else {
        cout << imageName << " was loaded!" << endl;
    }
    imshow("original", image);

    int w = image.cols;
    int h = image.rows;

    int w2 = static_cast<int>(w * scale);
    int h2 = static_cast<int>(h * scale);

    int data[w2*h2];
    unsigned char* dataMat = image.data;

    resizePixels(data, reinterpret_cast<int *>(dataMat), w, h, w2, h2);
    Mat pixelResizedImage = Mat(h2, w2, CV_8U, data);
    imshow("pixelResizedImage", pixelResizedImage);

    resizeImage(data, reinterpret_cast<int *>(dataMat), w, h, w2, h2);
    Mat interpolationResizedImage = Mat(h2, w2, CV_8U, data);
    imshow("interpolationResizedImage", interpolationResizedImage);

    Size size(w2, h2);
    Mat openCVScale;
    resize(image, openCVScale, size);
    imshow("OpenCVScale", openCVScale);

    waitKey(0);
    return 0;
}
