#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

int x_offset = 215;
int y_offset = 100;
int board_size = 220;

Scalar orange_lower = Scalar(100, 125, 180);
Scalar orange_upper = Scalar(255, 255, 255);

VideoCapture cap(0);

void initialize_camera() {

    namedWindow("Display window");

    if (!cap.isOpened())
    {
        cout << "cannot open camera";
    }
}

//Converts to range [-1, 1]
double normalize(int value, int size) {
    return 2 * ((double)value / size) - 1;
}

//Returns Vec2d containing the normalized x and y coordinates of the center of the ball
//Also shows live feed
Vec2d image_process() {

    Mat image, image_cropped, frame_orange;

    cap >> image;

    Rect r = Rect(x_offset, y_offset, board_size, board_size);
    rectangle(image, r, Scalar(0, 0, 255), 2, 8, 0);

    image_cropped = image(Range(y_offset, y_offset + board_size), Range(x_offset, x_offset + board_size));
    inRange(image_cropped, orange_lower, orange_upper, frame_orange);

    int x_sum = 0;
    int y_sum = 0;
    int pts = 0;

    for (int i = 0; i < frame_orange.rows; i++)
    {
        for (int j = 0; j < frame_orange.cols; j++)
        {
            uchar bgrPixel = frame_orange.at<uchar>(i, j);

            if ((int)bgrPixel == 255)
            {
                x_sum += i;
                y_sum += j;
                pts++;
            }
        }
    }

    int x_avg, y_avg;

    if(pts < 200) {
        x_avg = 0;
        y_avg = 0;
    } 
    else {
        x_avg = x_sum / pts;
        y_avg = y_sum / pts;
    }

    line(image, Point(x_offset + y_avg, y_offset), Point(x_offset + y_avg, y_offset + board_size), Scalar(0, 0, 255), 2, 8, 0);
    line(image, Point(x_offset, y_offset + x_avg), Point(x_offset + board_size, y_offset + x_avg), Scalar(0, 0, 255), 2, 8, 0);

    imshow("Display window", image);

    waitKey(25);

    return Vec2d(normalize(x_avg, image_cropped.rows), normalize(y_avg, image_cropped.cols));

}

int main()
{

initialize_camera();

    while (true) {

        

        Vec2d pos = image_process();
        

    }

    return 0;
}