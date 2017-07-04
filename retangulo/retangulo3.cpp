#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


int main()
{
    cv::Mat input = cv::imread("IMG_5213.JPG");

    // convert to grayscale (you could load as grayscale instead)
    cv::Mat gray;
    cv::cvtColor(input,gray, CV_BGR2GRAY);

    // compute mask (you could use a simple threshold if the image is always as good as the one you provided)
    cv::Mat mask;
    cv::threshold(gray, mask, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

    // find contours (if always so easy to segment as your image, you could just add the black/rect pixels to a vector)
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask,contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    /// Draw contours and find biggest contour (if there are other contours in the image, we assume the biggest one is the desired rect)
    // drawing here is only for demonstration!
    int biggestContourIdx = -1;
    float biggestContourArea = 0;
    cv::Mat drawing = cv::Mat::zeros( mask.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        cv::Scalar color = cv::Scalar(0, 100, 0);
        drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, cv::Point() );

        float ctArea= cv::contourArea(contours[i]);
        if(ctArea > biggestContourArea)
        {
            biggestContourArea = ctArea;
            biggestContourIdx = i;
        }
    }

    // if no contour found
    if(biggestContourIdx < 0)
    {
        std::cout << "no contour found" << std::endl;
        return 1;
    }

    // compute the rotated bounding rect of the biggest contour! (this is the part that does what you want/need)
    cv::RotatedRect boundingBox = cv::minAreaRect(contours[biggestContourIdx]);
    // one thing to remark: this will compute the OUTER boundary box, so maybe you have to erode/dilate if you want something between the ragged lines



    // draw the rotated rect
    cv::Point2f corners[4];
    boundingBox.points(corners);
    cv::line(drawing, corners[0], corners[1], cv::Scalar(255,255,255));
    cv::line(drawing, corners[1], corners[2], cv::Scalar(255,255,255));
    cv::line(drawing, corners[2], corners[3], cv::Scalar(255,255,255));
    cv::line(drawing, corners[3], corners[0], cv::Scalar(255,255,255));

    // display
    cv::imshow("input", input);
    cv::imshow("drawing", drawing);
    cv::waitKey(0);

    cv::imwrite("rotatedRect.png",drawing);

    return 0;
}
