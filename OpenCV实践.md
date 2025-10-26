# 设计思路

程序首先读取这个视频，然后逐帧处理：

1. 分成红、绿两个通道的图像并分别提取颜色；
2. 预处理，使其提取得到的轮廓更加准确；
3. 提取轮廓；
4. 按面积和形状筛选轮廓；
5. 通过筛选得到的轮廓来绘制矩形

最后，保存视频文件。

# 准备工作

在着手进行正式的代码编写之前，根据设计思路，我需要找到HSV提取范围：

```c++
cv::Mat imgHSV, mask;
int hMin = 0, hMax = 0, sMin = 0, sMax = 0, vMin = 0, vMax = 0;

int main() {

    cv::Mat img = cv::imread("../resources/R.png");
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

    cv::namedWindow("Trackbars", cv::WINDOW_NORMAL);
    cv::createTrackbar("Hue Min", "Trackbars", nullptr, 179);
    cv::createTrackbar("Hue Max", "Trackbars", nullptr, 179);
    cv::createTrackbar("Sat Min", "Trackbars", nullptr, 255);
    cv::createTrackbar("Sat Max", "Trackbars", nullptr, 255);
    cv::createTrackbar("Val Min", "Trackbars", nullptr, 255);
    cv::createTrackbar("Val Max", "Trackbars", nullptr, 255);

    cv::resize(img, img, cv::Size(img.cols / 2, img.rows / 2));
    cv::resize(imgHSV, imgHSV, cv::Size(imgHSV.cols / 2, imgHSV.rows / 2)); // 目标图像尺寸太大，电脑屏幕无法显示完整，于是调小一些

    do {
        hMin = cv::getTrackbarPos("Hue Min", "Trackbars");
        hMax = cv::getTrackbarPos("Hue Max", "Trackbars");
        sMin = cv::getTrackbarPos("Sat Min", "Trackbars");
        sMax = cv::getTrackbarPos("Sat Max", "Trackbars");
        vMin = cv::getTrackbarPos("Val Min", "Trackbars");
        vMax = cv::getTrackbarPos("Val Max", "Trackbars");
        
        cv::inRange(imgHSV, cv::Scalar(hMin, sMin, vMin), cv::Scalar(hMax, sMax, vMax), mask);

        cv::imshow("img", img);
        cv::imshow("imgHSV", imgHSV);
        cv::imshow("mask", mask);
    } while (cv::waitKey(1) != 27);

    return 0;
}
```

调整一下，尽量减少目标之外的颜色提取，尽量减少颜色提取范围：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761374396469-be4c8f22-6b66-43c4-9891-24f8d56a62e6.png)

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761374524008-89a30089-88f9-4a97-b7ba-09a3685c5ae0.png)

绿灯部分同理。最终得到：红灯识别的颜色提取范围：`hsv(0, 110, 172)`至`hsv(179, 218, 255)`；绿灯识别的颜色提取范围：`hsv(85, 196, 124)`至`hsv(95, 255, 196)`。

当然，实际上写在代码里的要比这里得到的颜色提取范围要稍微大一点（我实际上上下限分别放宽了`2`）。

# 代码编写

读取文件：

```c++
cv::VideoCapture cap("../resources/TrafficLight.mp4");
if (!cap.isOpened()) {
    std::cerr << "Failed to open video" << std::endl;
    return -1;
}
```

创建`VideoWriter`类：

```c++
double fps = cap.get(cv::CAP_PROP_FPS);
cv::Size2d frameSize(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT));
cv::VideoWriter writer("result.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frameSize);
```

对视频进行逐帧处理：将帧转化为HSV，并分成R通道和G通道，分别代表红灯和绿灯，然后分别进行有无圆形信号灯的判断。我在这里定义了一个`mask()`方法，可以识别圆形信号灯，并将方框和文字绘制在帧上。处理过后，将其写入到输出文件里。由于原图像尺寸过大，我的显示屏装不下，于是将其缩放50%，并显示在窗口中（写入到输出文件里的图像尺寸保持不变）。

```c++
while (true) {
    cap >> frame;
    if (frame.empty()) {
        break;
    }
    if (cv::waitKey(30) == 27) {
        break;
    }
    cv::cvtColor(frame, frameHSV, cv::COLOR_BGR2HSV);

    cv::inRange(frameHSV, cv::Scalar(0, 108, 170), cv::Scalar(179, 220, 255), frameRedChannel);
    mark(frameRedChannel, frame, "Red");

    cv::inRange(frameHSV, cv::Scalar(83, 194, 122), cv::Scalar(97, 255, 198), frameGreenChannel);
    mark(frameGreenChannel, frame, "Green");

    cv::resize(frame, frameShow, cv::Size(frame.cols / 2, frame.rows / 2), 0.5, 0.5);

    writer.write(frame);

    cv::imshow("output", frameShow);
}
```

循环中出现的`cv::Mat`类型变量已经在循环外面定义好了。循环之后，释放`cap` `writer`，并关闭窗口：

```c++
cap.release();
writer.release();
cv::destroyAllWindows();
```

其实这三行代码不写也不影响效果，因为在程序结束后，`cap` `writer`自动释放，窗口也自动关闭。但是为规范起见，尽量还是写一下。

另外，`writer`释放以后，`result.avi`才能正常打开。

接下来是`mask()`方法的实现。

`mask()`方法定义：

```c++
void mark(const cv::Mat& inputImage, cv::Mat drownImage, const cv::String & text);
```

定义需要放在`main()`之前，否则无法在主函数内引用这个方法。`cv::String`是OpenCV自己定义的字符串类型，在这里可以换成`std::string`。不过为了“随乡入俗”（OpenCV的方法形参的字符串类型一般都用`cv::String`），我选用OpenCV定义的字符串类型。

首先，对输入的图像进行预处理。从先到后分别是高斯滤波（或者说高斯模糊），Canny边缘检测，膨胀：
```c++
cv::Mat GaussianImage, CannyImage, dilateImage;

cv::GaussianBlur(inputImage, GaussianImage, cv::Size(3, 3), 0);
cv::Canny(GaussianImage, CannyImage, 25, 75);
cv::dilate(CannyImage, dilateImage, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
```

然后，提取边缘，获得点坐标和向量的集合。实际上向量集合在后面并不会用到，但`hierarchy`形参是必须的，所以就定义了一个~~用来凑数~~。

```c++
std::vector<std::vector<cv::Point> > contours;
std::vector<cv::Vec4i> hierarchy;

cv::findContours(dilateImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
```

之后，对`contours`的元素进行遍历，然后处理：

```c++
for (int i = 0; i < contours.size(); i++) {

    std::vector<std::vector<cv::Point> > conPoly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());

    if (cv::contourArea(contours[i]) > 25000) {

        double peri = cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
        boundRect[i] = cv::boundingRect(conPoly[i]);
        if (conPoly[i].size() >= 5) {
            cv::rectangle(drownImage, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(255, 0, 0), 10);
        }
    }
}
```

在最初写`for`循环的时候，本来写的是用`auto`关键字遍历contours内的元素（按照编辑器语法检查的说法，`auto`关键字遍历元素比传统的用序号遍历元素的效率更高）：

```c++
for (auto & contour : contours) {
    // ...
}
```

但由于后面还有`conPoly[i]`，需要获取当前遍历的`contours`的元素的序号`i`，因此就改成了序号遍历元素的方式。

在这里加了一个对轮廓包围面积的判断，以过滤噪点和红绿灯数字的轮廓，将圆形信号灯的轮廓筛选出来。里面又套了一个对折线化后的轮廓的顶点数量的判断，以保证边框框住的信号灯是圆形的（实际上不加这个判断也可以，因为视频中并没有出现需要过滤掉的其他形状的信号灯）。

至于文字的绘制，首先在图像预处理那个部分中定义一个布尔类型对象`ifText`，代表需要绘制文字的真假，初始值为`false`。然后，在顶点数量的判断里，将其赋值成`true`。也就是说，如果边框成功绘制，则文字绘制的需要为真。在`mask()`方法末尾，判断需要文字绘制的真假，若为真，则：

```c++
cv::putText(drownImage, text, cv::Point(inputImage.cols / 2 - 300, 100), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0, 0));
```

`mask()`方法实现完成。

# 运行程序

运行程序，弹出一个播放视频的窗口，窗口中绘制了红绿灯的识别结果：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761446759021-615e1852-8966-4245-9c15-02295457a524.png)

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761446799123-afb01fff-3856-4e31-8fdf-1bd3d606e2aa.png)

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761446810828-c53e6c76-43f4-47f3-9c3d-3f80c85fe1a9.png)

视频播放完毕以后，窗口自动关闭，程序结束运行。随后打开程序生成的`result.avi`：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761446979976-cf90694b-8c96-41d9-9ab1-70f043fbe27b.png)

可以正常播放，程序运行成功。

# 遇到的问题

## OpenCV警告使用非空指针不安全

在[准备工作](#准备工作)代码的将跟踪栏的值赋给变量的部分，我最初是这样写的：

```c++
cv::createTrackbar("Hue Min", "Trackbars", &hMin, 179);
cv::createTrackbar("Hue Max", "Trackbars", &hMax, 179);
cv::createTrackbar("Sat Min", "Trackbars", &sMin, 255);
cv::createTrackbar("Sat Max", "Trackbars", &sMax, 255);
cv::createTrackbar("Val Min", "Trackbars", &vMin, 255);
cv::createTrackbar("Val Max", "Trackbars", &vMax, 255);
```

但OpenCV却警告：

> Using ‘value’ pointer is unsafe and deprecated. Use NULL as value pointer. To fetch trackbar value setup callback.

根据[此处](https://forum.opencv.org/t/createtrackbar-warns-that-value-pointer-is-deprecated-but-is-there-any-other-way-to-set-trackbars-initial-position/7675)，改成：（其他同理）

```c++
// 循环外
cv::createTrackbar("Hue Min", "Trackbars", nullptr, 179);
// 循环内
hMin = cv::getTrackbarPos("Hue Min", "Trackbars");
```

## 方框绘制在了红绿灯的数字上

在调试时，红绿灯的数字上闪现了几帧方框，特别是在黄灯亮起时，黄灯上叠加的数字周围一直绘有方框。

于是我添加了对轮廓包围面积的判定。经过反复试错，将面积阈值设置在25000，刚好能在不影响圆形信号灯的判定的前提下，避免在数字周围绘制方框。

```c++
if (cv::contourArea(contours[i]) > 25000) {
    // ...
}
```

当然，在我看来，也可以遍历并输出某一帧中所有轮廓的包围面积来确定这个阈值。把这一行写在循环里即可：

```c++
std::cout << cv::contourArea(contours[i] << std::endl;
```

不过，由于数字的面积和圆形信号灯的面积差距比较大，阈值很好估计，所以我就没有这样写。如果阈值不好凭肉眼确定的话，我觉得可以用这个方法来确定阈值。如果图像中包含的轮廓很多，很难把输出到终端的面积与图像中的轮廓对应，可以将轮廓绘制出来，并将`i`的值绘制在轮廓上。

```c++
// 循环内
// 这里可以先设定一个比较小的面积阈值，用于过滤噪点的轮廓
cv::drawContours(drownImage, contours, i, cv::Scalar(255, 0, 0), 10);
cv::putText(drownImage, to_string(i), contours[i][0], cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0, 0))
std::cout << i << cv::contourArea(contours[i] << std::endl;
```

## 有几帧的圆形信号灯未能成功识别

经检查，问题出现在：在生成蒙板图像`frameRedChannel` `frameGreenChannel`时，设定的颜色提取范围和先前得到的颜色范围刚好相等。但是，源视频是对实景的拍摄，获得的信号灯颜色的HSV值会有一定浮动。在某些帧中，HSV值刚好浮动到了颜色提取范围之外，因而没能在蒙板图像中完整呈现圆形信号灯的形状，最终导致无法识别信号灯。

```c++
cv::inRange(frameHSV, cv::Scalar(0, 110, 172), cv::Scalar(179, 218, 255), frameRedChannel);

cv::inRange(frameHSV, cv::Scalar(85, 196, 124), cv::Scalar(95, 255, 196), frameGreenChannel);
```

我的解决方法也很简单，稍微放宽一些颜色提取范围即可：

```c++
cv::inRange(frameHSV, cv::Scalar(0, 108, 170), cv::Scalar(179, 220, 255), frameRedChannel);

cv::inRange(frameHSV, cv::Scalar(83, 194, 122), cv::Scalar(97, 255, 198), frameGreenChannel);
```