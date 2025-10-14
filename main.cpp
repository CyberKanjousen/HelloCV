#include <opencv2/opencv.hpp>

int main() {
    cv::Mat src = cv::imread(R"(C:\Users\TT\Pictures\EB91C0DCC7EDF6558CDC51AAB0051AB3.jpg)");
    cv::imshow("src", src);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}