#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <random>

void mark(const cv::Mat& inputImage, cv::Mat drownImage, const cv::String & text);

int main() {

    cv::VideoCapture cap("../resources/TrafficLight.mp4");
    if (!cap.isOpened()) {
        std::cerr << "Failed to open video" << std::endl;
        return -1;
    }

    cv::Mat frame, frameHSV, frameShow, frameRedChannel, frameGreenChannel;

    double fps = cap.get(cv::CAP_PROP_FPS);
    cv::Size2d frameSize(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::VideoWriter writer("result.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frameSize);

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        if (cv::waitKey(30) == 27) {
            break;
        }
        cv::cvtColor(frame, frameHSV, cv::COLOR_BGR2HSV);

        cv::inRange(frameHSV, cv::Scalar(0, 108, 170), cv::Scalar(181, 220, 255), frameRedChannel);
        mark(frameRedChannel, frame, "Red");

        cv::inRange(frameHSV, cv::Scalar(83, 194, 122), cv::Scalar(97, 255, 198), frameGreenChannel);
        mark(frameGreenChannel, frame, "Green");

        cv::resize(frame, frameShow, cv::Size(frame.cols / 2, frame.rows / 2), 0.5, 0.5);

        writer.write(frame);

        cv::imshow("output", frameShow);
    }

    cap.release();
    writer.release();
    cv::destroyAllWindows();

    return 0;
}

void mark(const cv::Mat & inputImage, cv::Mat drownImage, const cv::String & text) {

    cv::Mat GaussianImage, CannyImage, dilateImage;
    bool ifText = false;

    cv::GaussianBlur(inputImage, GaussianImage, cv::Size(3, 3), 0);
    cv::Canny(GaussianImage, CannyImage, 25, 75);
    cv::dilate(CannyImage, dilateImage, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(dilateImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < contours.size(); i++) {

        std::vector<std::vector<cv::Point> > conPoly(contours.size());
        std::vector<cv::Rect> boundRect(contours.size());

        if (cv::contourArea(contours[i]) > 25000) {

            double peri = cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            boundRect[i] = cv::boundingRect(conPoly[i]);
            if (conPoly[i].size() >= 5) {
                cv::rectangle(drownImage, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(255, 0, 0), 10);

                ifText = true;
            }
        }
    }

    if (ifText) {
        cv::putText(drownImage, text, cv::Point(inputImage.cols / 2 - 300, 100), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0, 0));
    }
}
