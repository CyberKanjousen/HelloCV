按照个人习惯，IDE为CLion，语言标准为C++26，操作系统为Ubuntu。

# 第一个实例

头文件需要包含：

```c++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
```

读取一张图片并在一个新建的窗口中显示：

```c++
int main() {
    // 将目标图片存储到一个Mat类型的数据中
    // 这里用cv::String代替std::string也是可以的
    std::string path = "../resources/image.jpg";
    cv::Mat img = cv::imread(path);
    // 新建一个名称为"Image"的窗口并在窗口中显示这张图片
    cv::imshow("Image", img);
    // 设置图片显示的持续时间（单位：毫秒），若delay为0，则表示无限等待，直到用户按下任意键（默认为0）
    cv::waitKey();
    // 关闭所有窗口
    cv::destroyAllWindows();
    return 0;
}
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761111760112-b6223608-95a8-4ff6-9c81-ab5de2f99ba0.png)

上面这个截图是用QQ截图工具获取的，因为使用Ubuntu自带截图工具需要按快捷键，而按键会立刻关闭程序。但是Linux上的QQ截图工具没有快捷键，用起来很麻烦。我从[此处](https://www.runoob.com/opencv/opencv-tutorial.html)得知，`cv::waitKey()`方法返回的是键盘输入的ASCII码（整型），所以我利用这一点，指定`Esc`键用来关闭程序：

```c++
// 将cv::waitKey()一行改成：
while (cv::waitKey() != 27) {}
```

这里`27`是`Esc`键的ASCII码。

如果要检查路径是否有效的话，可以加上这个：

```c++
if (img.empty()) {
    std::cerr << "Failed to load image: " << path << std::endl;
    return -1;
}
```

# 图像的基本操作

> 以下参考了OpenCV的注释和[此处](https://www.runoob.com/opencv/cpp-opencv-basic.html)。

## 读取图像

将`path`路径的图像读取到`Mat`类型的数据中：

```c++
cv::Mat image = cv::imread(path, flags);
```

形参：

- `path`: `cv::String`类型常量，图像文件的路径。
- `flags`: `int`类型变量，读取图像的方式，默认为`cv::IMREAD_COLOR`。

根据[此处](https://blog.csdn.net/youcans/article/details/121168935)的说法，读取图片的方式有：

- `cv::IMREAD_COLOR`: 始终将图像转换为3通道BGR彩色图像
- `cv::IMREAD_GRAYSCALE`: 始终将图像转换为单通道灰度图像
- `cv::IMREAD_UNCHANGED`: 按原样返回加载的图像（使用Alpha通道）
- `cv::IMREAD_ANYDEPTH`: 在输入具有相应深度时返回16位或32位图像，否则将其转换为8位
- `cv::IMREAD_ANYCOLOR`: 以任何可能的颜色格式读取图像

`Mat`是"Matrix"矩阵——图像的数据是以矩阵的形式存储的。

## 显示图像

新建一个窗口并在窗口中显示这张图片：

```c++
cv::imshow(winname, mat);
```

形参：

- `winname`: `cv::String`类型常量，窗口的名称。
- `mat`: `cv::InputArray`类型变量，要显示的图像。

根据[此处](https://docs.opencv.org/3.4/d4/d32/classcv_1_1__InputArray.html)的说法，`cv::InputArray`是一个接口（其中"Array"是数组的意思），可以从`cv::Mat` `std::vector<T>`等构造。

## 保存图像

```c++
cv::imwrite(filename, img, params)
```

形参：

- `filename`: `cv::String`类型常量，保存文件的名称。
- `img`: `cv::InputArray`类型变量，保存的图像。
- `params`: `std::vector<int>`类型常量，以成对的特定格式的参数（详见`cv::ImwriteFlags`），默认为`std::vector<int>()`。

## 获取尺寸

```c++
int height = image.rows; // 高度
int width = image.cols; // 宽度
```

## 获取通道数

```c++
int channels = image.channels();
```

根据[此处](https://en.wikipedia.org/wiki/Channel_(digital_image))的说法，通道是彩色图像的灰度图像，它仅由构成彩色图像的一种原色组成。比如在黑白图像中，只有一个名为“灰度”的通道；在BGR图像中，有蓝色、绿色、红色三个通道。如果一个BGR像素占24位，那么每个通道就是8位，一个通道的值的区间就是[0, 255]。

**OpenCV中的三通道图像一般是BGR而非RGB。** 也就是说，这类图像的三个通道先后为蓝色、绿色、红色而非红色、绿色、蓝色。

## 获取像素值

```c++
auto pixel = image.at<cv::Vec3b>(y, x); // 访问(x, y)处的像素值
```
根据OpenCV的源代码，`Vec3b`的完整写法应该是`Vec<unsigned char, 3>`。在我看来，`Vec`应指"Vector"向量，推测`Vec<unsigned char, 3>`相当于一个值类型为无符号字符的三维向量，其指定了像素的读取方式。

`auto`也可以换成`cv::Vec3b`，但根据编辑器的提示，使用`auto`优化性会更好一些。

在[上文](#第一个实例)那个例子中，如果加上：

```c++
auto pixel = img.at<cv::Vec3i>(1, 1);
std::cout << pixel[0] << " " << pixel[1] << " " << pixel[2] << std::endl;
```

则输出`121 131 149`，也就是这个像素的BGR值。

这里之所以用`cv::Vec3i`而不是`cv::Vec3b`，是因为：经过我测试，如果用`std::cout`对`pixel[i]`进行输出的话，必须将其强制转化为整型，否则输出乱码；用`cv::Vec3i`就不需要转化。

OpenCV提供了以下别名：

```c++
typedef Vec<uchar, 2> Vec2b;
typedef Vec<uchar, 3> Vec3b;
typedef Vec<uchar, 4> Vec4b;

typedef Vec<short, 2> Vec2s;
typedef Vec<short, 3> Vec3s;
typedef Vec<short, 4> Vec4s;

typedef Vec<ushort, 2> Vec2w;
typedef Vec<ushort, 3> Vec3w;
typedef Vec<ushort, 4> Vec4w;

typedef Vec<int, 2> Vec2i;
typedef Vec<int, 3> Vec3i;
typedef Vec<int, 4> Vec4i;
typedef Vec<int, 6> Vec6i;
typedef Vec<int, 8> Vec8i;

typedef Vec<float, 2> Vec2f;
typedef Vec<float, 3> Vec3f;
typedef Vec<float, 4> Vec4f;
typedef Vec<float, 6> Vec6f;

typedef Vec<double, 2> Vec2d;
typedef Vec<double, 3> Vec3d;
typedef Vec<double, 4> Vec4d;
typedef Vec<double, 6> Vec6d;
```

## 创建图像

```c++
cv::Mat newImage(640, 480, CV_8UC3, cv::Scalar(0, 0, 255)); // 创建一个640x480的红色图像（三通道8位无符号整型图像，通常为BGR图像）
```

`newImage`第三个参数是创建图像的深度。这里`8`代表一个像素的一个通道占8位，`UC`代表无符号整型，`3`代表一个像素有三个通道。

根据[此处](https://www.cnblogs.com/keye/p/18381834)的说法，常用的图像深度标识符有：

- `CV_8UC1`: 单通道8位无符号整型图像（灰度图像）。
- `CV_8UC3`: 三通道8位无符号整型图像（通常为BGR图像）。
- `CV_8UC4`: 四通道8位无符号整型图像（通常为BGRA图像，A表示Alpha通道，即透明度）。
- `CV_16UC1`: 单通道16位无符号整型图像。
- `CV_32FC1`: 单通道32位浮点型图像。

## 初始化图像

```c++
image.setTo(cv::Scalar(255, 255, 255)); // 将图像初始化为白色
```

形参：

- `value` - `cv::InputArray`类型变量，像素值
- `mask` - `cv::InputArray`类型变量，蒙板，将`image`中与`mask`中非零像素的坐标相同的像素的值都变为`value`，默认为一个所有像素都不为零的`mask`。

经测试，我发现：如果三通道`mask`中的一个像素为`bgr(1, 1, 0)`（这里借用了CSS的颜色表示方法），三通道`image`中同一坐标的像素为`bgr(0, 0, 0)`，然后通过`mask`将`image`初始化为`bgr(255, 255, 255)`，此时`image`的那个像素为`bgr(255, 255, 0)`（青色）而非`bgr(255, 255, 255)`（白色）。

由此我推断出：OpenCV是分通道来初始化图像的。

## 遍历像素

```c++
for (int y = 0; y < image.rows; y++) {
    for (int x = 0; x < image.cols; x++) {
        auto & pixel = image.at<cv::Vec3b>(y, x);
        // 对像素进行操作
    }
}
```

## 修改像素值

```c++
pixel[0] = 11; // 将蓝色通道设置为11
pixel[1] = 45; // 将绿色通道设置为45
pixel[2] = 14; // 将红色通道设置为14
```

结合[创建图像](#创建图像)、[初始化图像](#初始化图像)、[遍历像素](#遍历像素)部分，在[上文](#第一个实例)那个例子中，如果要通过`mask`给图片加一个宽度为`20px`的黑色边框，可以这样写：

```c++
cv::Mat mask(684, 1080, CV_8UC3, cv::Scalar(0, 0, 0));
for (int y = 0; y < mask.rows; y++) {
    for (int x = 0; x < mask.cols; x++) {
        if (y < 20 || y > 663 || x < 20 || x > 1059) {
            auto & pixel = mask.at<cv::Vec3b>(y, x);
            pixel[0] = 1;
            pixel[1] = 1;
            pixel[2] = 1;
            /*在这里也可以这样写：
            * mask.at<cv::Vec3b>(y, x)[0] = 1;
            * mask.at<cv::Vec3b>(y, x)[1] = 1;
            * mask.at<cv::Vec3b>(y, x)[2] = 1;
            * */
        }
    }
}
img.setTo(cv::Scalar(0, 0, 0), mask);
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761195488934-4536699e-fc40-4b33-9195-38649f505981.png)

经我测试，这里不能用`cv::Vec3i`替代`cv::Vec3b`，否则程序会报错，且显示的图片无法达成预期效果。

由于OpenCV分通道来初始化图像，所以这里`mask`的通道数必须也为3。

## 图像缩放

```c++
cv::Mat dst;
cv::resize(src, dst, dsize, fx, fy, interpolation);
```

形参：
- `src`: `cv::InputArray`类型变量，源图像
- `dst`: `cv::OutputArray`类型变量，输出图像
- `dsize`: `cv::Size`类型变量，输出图像的的尺寸。通常输入`cv::Size(width, height)`
- `fx`: `double`类型变量，沿x轴缩放的倍数。若值为`0`，则不缩放。默认为`0`
- `fy`: `double`类型变量，沿y轴缩放的倍数。若值为`0`，则不缩放。默认为`0`
- `interpolation`: `int`类型变量，插值方式（即图像缩放后对像素重新计算的方式），默认为`cv::INTER_LINEAR`

其中，根据[此处](https://blog.csdn.net/weixin_41466575/article/details/113058802)，OpenCV提供了以下插值方法：

- `cv::INTER_NEAREST`: 最邻近插值
- `cv::INTER_LINEAR`: 双线性插值
- `cv::INTER_CUBIC`: 4x4像素邻域内的双三次插值
- `cv::INTER_LANCZOS4`: 8x8像素邻域内的Lanczos插值

> 最近邻插值算法选择距离所求数据点最近点的值，并且根本不考虑其他相邻点的值，从而产生一个分段常数的内插值来作为所求数据点的值。
>
> 出自[Wikipedia](https://zh.wikipedia.org/zh-cn/%E6%9C%80%E8%BF%91%E9%82%BB%E6%8F%92%E5%80%BC)

> 在数学上，双线性插值是对线性插值在二维直角网格上的扩展，用于对双变量函数（例如 x 和 y）进行插值。其核心思想是在两个方向分别进行一次线性插值。
> 
> 出自[Wikipedia](https://zh.wikipedia.org/zh-cn/%E5%8F%8C%E7%BA%BF%E6%80%A7%E6%8F%92%E5%80%BC)

最近邻插值缩放会损失一定的空间对称性，因此常用相对复杂一些的双线性插值算法。但是在缩减一半或放大一倍的范围以外，“由于会丢掉太多的像素或者进行了过多的平滑处理，纹理看起来就会很差”（[Wikipedia](https://zh.wikipedia.org/wiki/%E5%8F%8C%E7%BA%BF%E6%80%A7%E8%BF%87%E6%BB%A4)）。在这种情况下，就要用到更加复杂的双三次插值。Lanczos插值比双三次插值还要复杂，而缩放效果会更好。

最近邻插值缩放10%的效果（原图像宽高4284*5712）：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761227852995-322fdfdd-685e-426d-867b-6d9c2f4a6af6.png)

双线性插值：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761227815360-96d370a7-2274-4049-9f32-11fd6754e0e6.png)

双三次插值：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761227780418-c79c25b9-ffaa-4a73-a9ac-30633704ba1d.png)

Lanczos插值：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761227741499-1ca3fa05-4318-425c-a2b5-7ae88945808a.png)

## 翻转图像

```c++
cv::Mat flippedImage;
cv::flip(image, flippedImage, 1); // 1表示水平翻转，0表示垂直翻转
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761223580845-6bef975b-9325-4720-ba01-4fef4b43615b.png)

## 裁剪图像

在图像里截取一个矩形图像。其中`cv::Rect`类型的属性见[绘制几何图形和文字](#绘制几何图形和文字)。

```c++
cv::Mat dst = src(cv::Rect(x, y, width, height));
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761450398104-264ded76-b888-4b9e-bda0-ec7c95265dec.png)

## 仿射变换

将图像绕中心旋转某个角度：

```c++
cv::Point2f center(image.cols / 2.0, image.rows / 2.0);
cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);
cv::Mat rotatedImage;
cv::warpAffine(image, rotatedImage, rotationMatrix, image.size());
```

`angle`值为90时：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761220644431-e9fe71da-c105-4159-bbe4-216f94c8fcc6.png)

将图像平移若干距离：

```c++
cv::Mat translationMatrix = (cv::Mat_<double>(2, 3) << 1, 0, tx, 0, 1, ty);
cv::Mat translatedImage;
cv::warpAffine(image, translatedImage, translationMatrix, image.size());
```

`tx`值为`114`，`ty`值为`514`时：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761221389547-ca982e7f-1b01-4992-9803-9b5bc3d522dc.png)

缩放图像：（缩放0.4倍数）

```c++
cv::Mat translationMatrix = (cv::Mat_<double>(2, 3) << 0.4, 0, 0, 0, 0.4, 0);
cv::Mat translatedImage;
cv::warpAffine(image, translatedImage, translationMatrix, image.size());
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761222891706-28804b8b-0e54-410d-b610-0e08603bd784.png)

根据[此处](https://blog.csdn.net/qq_22734027/article/details/134482760)的说法，`cv::warpAffine(src, dst, M, dsize, flags, borderMode, borderValue)`是二维仿射变换方法，其形参有：

- `src`: 输入图像，必须是单通道或三通道的8位或32位浮点型图像
- `dst`: 输出图像，其大小和类型与输入图像相同
- `M`: 2*3的变换矩阵
- `dsize`: 输出图像的大小
- `flags`: 用于指定插值的方法（见[图像缩放](#图像缩放)）
- `borderMode`: 用于指定如何处理输出图像边缘的像素，默认为常量填充模式。可用的选项有`BORDER_CONSTANT` `BORDER_REPLICATE` `BORDER_WRAP`等
- `borderValue`: 用于指定填充的边界值，默认为`0`。

其中，变换矩阵是用于表示图像变换的3x3矩阵——仿射变换一般省略第三行，即2*3矩阵。该矩阵的仿射变换的用法如下：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761223333987-d73abe6d-49ef-4f58-ae38-0eb02c414398.png)

OpenCV有一个`cv::getAffineTransform(src, dst)`方法可以通过映射输入图像和输出图像的坐标的方式来生成转换矩阵，其用法和`cv::getPerspectiveTransform(src, dst)`相似。`cv::getPerspectiveTransform(src, dst)`的用法见[透视变换](#透视变换)。


## 透视变换

```c++
cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(srcPoints, dstPoints);
cv::Mat perspectiveImage;
cv::warpPerspective(image, perspectiveImage, perspectiveMatrix, image.size());
```

根据[此处](https://zhuanlan.zhihu.com/p/641970591)，`cv::getPerspectiveTransform(src, dst)`的形参：

- `src`: `cv::InputArray`类型变量，输入图像坐标
- `dst`: `cv::InputArray`类型变量，输出图像坐标

这个函数将计算出透视变换的变换矩阵。

`cv::warpPerspective(src, dst, M, dsize, flags, borderMode, borderValue)`的形参，这和`cv::warpAffine()`很相似：

- `src`: `cv::InputArray`类型变量，输入图像
- `dst`: `cv::OutputArray`类型变量，输出图像，其大小和类型与输入图像相同
- `M`: `cv::InputArray`类型变量，3*3的变换矩阵
- `dsize`: `cv::Size`类型变量，输出图像的大小
- `flags`: `int`类型变量，用于指定插值的方法（见[图像缩放](#图像缩放)）
- `borderMode`: `int`类型变量，用于指定如何处理输出图像边缘的像素，默认为常量填充模式。可用的选项有`BORDER_CONSTANT` `BORDER_REPLICATE` `BORDER_WRAP`等
- `borderValue`: `cv::Scalar`类型常量，用于指定填充的边界值，默认为`0`。

实践一下，把这张银行卡“掰”正：（参照了[此处](https://zhuanlan.zhihu.com/p/641970591)的Python的思路）

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761232889653-c2dfcb86-c14b-40e7-b785-c867b2133e82.png)

首先，读取图片并把图片缩放到合适大小（原图像宽高4284*5712）：

```c++
cv::String path = "../resources/amiya_card.jpg";
cv::Mat img = cv::imread(path);
if (img.empty()) {
    std::cerr << "Failed to load image: " << path << std::endl;
    return -1;
}

cv::Mat dst;
cv::resize(img, dst, cv::Size(img.cols / 10, img.rows / 10), 0.1, 0.1, cv::INTER_LANCZOS4);
```

随后用鼠标指针在变换前的图片里确定银行卡四个角的坐标（按逆时针顺序排列）：

```c++
std::vector<float>
pt_A = {200, 126},
pt_B = {86, 349},
pt_C = {258, 422},
pt_D = {353, 165};
```

接下来计算银行卡的宽高：（初中就学过的两点间距离公式）

```c++
auto width_AD = static_cast<float>(sqrt(pow(pt_A[0] - pt_D[0], 2) + pow(pt_A[1] - pt_D[1], 2)));
auto width_BC = static_cast<float>(sqrt(pow(pt_B[0] - pt_C[0], 2) + pow(pt_B[1] - pt_C[1], 2)));
float maxWidth = std::ranges::max(width_AD, width_BC);

auto height_AB = static_cast<float>(sqrt(pow(pt_A[0] - pt_B[0], 2) + pow(pt_A[1] - pt_B[1], 2)));
auto height_CD = static_cast<float>(sqrt(pow(pt_C[0] - pt_D[0], 2) + pow(pt_C[1] - pt_D[1], 2)));
float maxHeight = std::ranges::max(height_AB, height_CD);
```

然后指定映射，计算转换矩阵`perspectiveMatrix`（注意点与点要相互对应）：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761230093222-17af76d1-be60-42df-b24f-b56a94ee3cbe.png)

```c++
cv::Point2f
srcPoints[] = {
    {pt_A[0], pt_A[1]},
    {pt_B[0], pt_B[1]},
    {pt_C[0], pt_C[1]},
    {pt_D[0], pt_D[1]}
},
dstPoints[] = {{0, 0},
    {0, maxHeight - 1},
    {maxWidth - 1, maxHeight - 1},
    {maxWidth - 1, 0}
};
cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(srcPoints, dstPoints);
```

输出图像到`perspectiveImage`中，最后显示窗口：

```c++
cv::Mat perspectiveImage;
cv::warpPerspective(dst, perspectiveImage, perspectiveMatrix, cv::Size(maxWidth, maxHeight));

cv::imshow("Image", perspectiveImage);
while (cv::waitKey() != 27) {}
cv::destroyAllWindows();
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761232828609-1cda974e-3cf7-44f9-8c73-93868f6f22f7.png)

## 颜色空间转换

RGB 转灰度：

```c++
cv::Mat grayImage;
cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761233557063-97c538b2-61f8-4258-80d2-0604eda1ff11.png)

RGB转HSV：

```c++
cv::Mat hsvImage;
cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
```

>HSV即色相、饱和度、明度（英语：Hue, Saturation, Value），又称HSB，其中B即英语：Brightness。
> 
>- 色相（Hue）是色彩的基本属性，就是平常所说的颜色名称，如红色、黄色等。
> 
>- 饱和度（Saturation）是指色彩的纯度，越高色彩越纯，低则逐渐变灰，取0-100%的数值。
> 
>- 明度（Value）、亮度（Lightness）、亮度（Brightness），取0-100%。
>
> ......
> 
> 艺术家有时偏好使用HSL或HSV而不选择三原色光模式（即RGB模型）或 印刷四分色模式（即CMYK模型），因为它类似于人类感觉颜色的方式，具有较强的感知度。RGB和CMYK分别是加法原色和减法原色模型，以原色组合的方式定义颜色，而HSV以人类更熟悉的方式封装了关于颜色的信息：“这是什么颜色？深浅如何？明暗如何？”。
> 
> 出自[Wikipedia](https://zh.wikipedia.org/wiki/HSL%E5%92%8CHSV%E8%89%B2%E5%BD%A9%E7%A9%BA%E9%97%B4)

## 通道分离与合并

通道分离：使用split函数将图像的通道分离。

```c++
std::vector<cv::Mat> channels;
cv::split(image, channels);
```

通道合并：使用merge函数将多个通道合并为一个图像。

```c++
cv::Mat mergedImage;
cv::merge(channels, mergedImage);
```

## 颜色提取

```c++
cv::Mat mask, dst;
cv::inRange(src, lowerb, upperb, mask);
cv::bitwise_and(src, src, dst, mask);
```

其中，`lowerb` `upperb`分别为颜色提取的最低值和最高值。通常情况下，`lowerb`应略小于目标颜色的实际最低值，`upperb`应略大于目标颜色的实际最高值。

比如，要提取在`bgr(125, 130, 100)`到`bgr(230, 230, 190)`之间的颜色：

```c++
cv::Mat mask, outputImage;
cv::inRange(image, cv::Scalar(125, 130, 100), cv::Scalar(230, 230, 190), mask);
cv::bitwise_and(image, image, outputImage, mask);
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761328081107-0c533c86-a834-44ee-9b05-ee31c5bf2947.png)

在实际应用中，更经常使用HSV来提取颜色。但是，纯手动寻找HSV的范围并不容易。为了更方便地找到颜色提取的最低值和最高值，可以使用创建跟踪栏的方法。比如我要提取红绿灯红灯的部分：

```c++
cv::Mat imgHSV, mask;
int hMin = 0, hMax = 0, sMin = 0, sMax = 0, vMin = 0, vMax = 0;

int main() {

    cv::Mat img = cv::imread("../resources/image.png");
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

    cv::namedWindow("Trackbars", cv::WINDOW_NORMAL);
    cv::createTrackbar("Hue Min", "Trackbars", nullptr, 179);
    cv::createTrackbar("Hue Max", "Trackbars", nullptr, 179);
    cv::createTrackbar("Sat Min", "Trackbars", nullptr, 255);
    cv::createTrackbar("Sat Max", "Trackbars", nullptr, 255);
    cv::createTrackbar("Val Min", "Trackbars", nullptr, 255);
    cv::createTrackbar("Val Max", "Trackbars", nullptr, 255);

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

# 视频的基本操作

## 视频读取、显示、保存

视频读取：

> 在 OpenCV 中，`VideoCapture`类用于从视频文件或摄像头中读取视频帧。
>
> 要读取一个视频文件，首先需要创建一个`VideoCapture`对象，并指定视频文件的路径。
>
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-video.html)

```c++
using namespace cv;

int main() {
    // 创建 VideoCapture 对象并打开视频文件
    VideoCapture cap("example.mp4");

    // 检查视频是否成功打开
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video file." << std::endl;
        return -1;
    }

    // 视频读取与显示代码将在下面介绍
    return 0;
}
```

当`VideoCapture`的参数为`0`时，则打开默认摄像头。

视频显示：

> 读取视频文件后，可以通过循环逐帧读取视频，并使用`imshow()`函数显示每一帧。
> 
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-video.html)

```c++
Mat frame;
while (true) {
    // 读取下一帧
    cap >> frame;

    // 如果帧为空，说明视频已经结束
    if (frame.empty()) {
        break;
    }

    // 显示当前帧
    imshow("Video", frame);

    // 等待30毫秒，按下ESC键退出
    if (waitKey(30) == 27) {
        break;
    }
}

// 释放 VideoCapture 对象
cap.release();
// 关闭所有窗口
destroyAllWindows();
```

在我看来，这里的`>>`提取运算符可以这样理解：`cap`是视频所有帧的有序集合，`>>`将`cap`的帧按顺序提取出来赋给`frame`（提取出来的帧不放回`cap`）；当视频播放完毕，`cap`已经没有剩下的帧了，这时再进行提取的话，`frame`将会被赋成一个空值。

视频保存：

> 如果你想要将处理后的视频保存到文件中，可以使用`VideoWriter`类。首先需要指定输出文件的名称、编码格式、帧率和帧大小。
> 
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-video.html)

```c++
// 获取视频的帧率和帧大小
double fps = cap.get(CAP_PROP_FPS);
Size frameSize(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT));

// 创建 VideoWriter 对象
VideoWriter writer("output.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frameSize);

while (true) {
    cap >> frame;
    if (frame.empty()) {
        break;
    }

    // 将帧写入输出视频文件
    writer.write(frame);

    imshow("Video", frame);
    if (waitKey(30) == 27) {
        break;
    }
}

// 释放 VideoCapture 和 VideoWriter 对象
cap.release();
writer.release();
destroyAllWindows();
```

其中`cv::VideoWriter::fourcc('M', 'J', 'P', 'G')`指定了视频的编码格式为`MJPG`，其可指定的常用编码格式有：

> - `I420`: 一个未压缩的YUV编码，4:2:0色度子采样。这种编码广泛兼容，但会产生大文件。文件扩展名应为`.avi`
> - `PIM1`: 此选项为MPEG-1。文件扩展名应为`.avi`
> - `XVID`: 一个相对较旧的MPEG-4编码。如果要限制结果视频的大小，这是一个很好的选择。文件扩展名应为`.avi`
> - `mp4v`: 另一个相对较旧的MPEG-4编码。如果要限制结果视频的大小，这是一个很好的选择。文件扩展名应为`.mp4`
> - `X264`: 一个比较新的MPEG-4编码。如果要限制结果视频的大小，这可能是最好的选择。文件扩展名应为`.mp4`
> - `H264`: 传统的H264编码方式。如果要在限制结果视频的大小，这可能是很好的选择。文件扩展名应为`.mp4`
> - `THEO`: 即Ogg Vorbis。文件扩展名应为`.ogv`
> - `FLV1`: Flash视频。文件扩展名应为`.flv`
> - `MJPG`: motion-jpeg视频。文件扩展名应为`.avi`
> 
> 出自[此处](https://blog.csdn.net/m0_64660649/article/details/132798258)

实践一下：

用本地的默认摄像头录制一个`.mp4`格式的视频：

```c++
int main() {

    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cerr << "Failed to open camera" << std::endl;
        return -1;
    }

    cv::Mat frame;
    double fps = cap.get(cv::CAP_PROP_FPS);
    cv::Size2d frameSize(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    cv::VideoWriter writer("output.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, frameSize);

    while (true) {
        cap >> frame;

        writer.write(frame);

        imshow("Video", frame);
        if (cv::waitKey(30) == 27) {
            break;
        }
    }

    cap.release();
    writer.release();
    cv::destroyAllWindows();

    return 0;
}
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761356705100-c609f00c-608a-4893-b950-d72641fdaff5.png)

打开输出的`output.mp4`，可以正常播放视频。

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761356846265-7dd0bad3-1fec-47e9-b5cf-58b6bf756bb4.png)

经我测试，以X264等某些编码格式输出摄像头输入的视频时，OpenCV会输出FFMPEG不支持用X264生成该视频的报错，但并不影响程序运行，因为FFMPEG在这种情况会使用默认编码格式mp4v进行处理。

## 逐帧处理

> 在视频处理中，通常需要对每一帧进行特定的处理操作。例如，可以对每一帧进行灰度化、边缘检测等操作。
> 
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-video.html)

```c++
while (true) {
    cap >> frame;
    if (frame.empty()) {
        break;
    }

    // 将帧转换为灰度图像
    Mat grayFrame;
    cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

    // 显示灰度帧
    imshow("Gray Video", grayFrame);

    if (waitKey(30) == 27) {
        break;
    }
}
```

实践一下：

将帧转换为灰度图像，并添加文字：

```c++
cv::Mat grayFrame;
cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
cv::putText(grayFrame, "EW", cv::Point(25, 100), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0), 2);
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761357872492-6717edc9-4a2c-4212-b9ea-2bcf7c661b3d.png)

## 实时处理

> 实时处理视频帧时，通常需要在每一帧上应用一些实时处理算法。例如，实时检测视频中的运动物体。
> 
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-video.html)

```c++
Mat prevFrame, nextFrame, diffFrame;
cap >> prevFrame;
cvtColor(prevFrame, prevFrame, COLOR_BGR2GRAY);

while (true) {
    cap >> nextFrame;
    if (nextFrame.empty()) {
        break;
    }

    cvtColor(nextFrame, nextFrame, COLOR_BGR2GRAY);

    // 计算帧间差异
    absdiff(prevFrame, nextFrame, diffFrame);

    // 显示差异帧
    imshow("Motion Detection", diffFrame);

    // 更新前一帧
    prevFrame = nextFrame.clone();

    if (waitKey(30) == 27) {
        break;
    }
}
```

按照上述的代码，实践一下：（我的手机在运动）

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761358220977-00ead993-110f-4ff4-8eef-43166c73f39c.png)

在我看来，逐帧播放是在当前将要播放的帧进行处理，而实时处理是对下一个（或者下若干个）将要播放的帧进行处理。

# 图像处理

## 图像滤波

均值滤波：

> 均值滤波是一种简单的线性滤波方法，它将图像中每个像素的值替换为其邻域内所有像素值的平均值。这种方法可以有效去除噪声，但也会使图像变得模糊。
> 
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-image-operator.html)

```c++
cv::blur(src, dst, ksize, anchor, borderType);
```

形参：

- `src`: `cv::InputArray`类型变量，输入图像；图像通道数不限，图像深度应为`CV_8U` `CV_16U` `CV_16S` `CV_32F` `CV_64F`
- `dst`: `cv::OutputArray`类型变量，目标图像，需要和源图片有一样的尺寸和类型
- `ksize`: `cv::Size`类型变量，卷积核的大小
- `anchor`: `cv::Point`类型变量，锚点；默认值为`cv::Point(-1, -1)`，这意味着锚点与卷积核中心重合
- `borderType`: `int`类型变量，用于推断图像外部像素的某种边界模式。默认值为`BORDER_DEFAULT`

> 卷积核（kernel）：也被叫做滤波器（filter），假设卷积核的高和宽分别为`kh`和`kw` ，则将称为`kh*kw`卷积，比如`3*5`卷积，就是指卷积核的高为3, 宽为5。卷积核中数值为对图像中与卷积核同样大小的子块像素点进行卷积计算时所采用的权重。
> 
> 出自[paddlepedia.readthedocs.io](https://paddlepedia.readthedocs.io/en/latest/tutorials/CNN/convolution_operator/Convolution.html)

高斯滤波：

> 高斯滤波是一种非线性滤波方法，它使用高斯函数来计算邻域内像素的权重，从而对图像进行平滑处理。高斯滤波在去除噪声的同时，能够更好地保留图像的边缘信息。
>
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-image-operator.html)

> 高斯滤波器仅是空间的函数，即在滤波时考虑附近的像素。它没有考虑像素是否具有几乎相同的强度。它不考虑像素是否是边缘像素。所以它也模糊了边缘。
> 
> 出自[opencv-python-tutorials.readthedocs.io](https://opencv-python-tutorials.readthedocs.io/zh/latest/4.%20OpenCV%E4%B8%AD%E7%9A%84%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86/4.4.%20%E5%9B%BE%E5%83%8F%E6%BB%A4%E6%B3%A2/)

```c++
cv::GaussianBlur(src, dst, ksize, sigmaX, sigmaY, borderType);
```

形参：

- `src`: `cv::InputArray`类型变量，输入图像；图像通道数不限，深度应为`CV_8U` `CV_16U` `CV_16S` `CV_32F` `CV_64F`
- `dst`: `cv::OutputArray`类型变量，目标图像，需要和源图片有一样的尺寸和类型
- `ksize`: `cv::Size`类型变量，高斯卷积核的大小
- `sigmaX`: `double`类型变量，X方向的标准偏差
- `sigmaY`: `double`类型变量，Y方向的标准偏差；默认值为`0`，此时Y方向的标准偏差等于`sigmaX`；若`sigmaY`和`sigmaX`都为`0`，则根据高斯卷积核大小计算这两个方向的标准偏差
- `borderType`: `int`类型变量，用于推断图像外部像素的某种边界模式。默认值为`BORDER_DEFAULT`

> 如果需要，可以使用函数`cv.getGaussianKernel()`创建高斯卷积核。
> 
> 上面的代码可以修改为高斯模糊：

```python
blur = cv.GaussianBlur(img,(5,5),0)
```

> 出自[opencv-python-tutorials.readthedocs.io](https://opencv-python-tutorials.readthedocs.io/zh/latest/4.%20OpenCV%E4%B8%AD%E7%9A%84%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86/4.4.%20%E5%9B%BE%E5%83%8F%E6%BB%A4%E6%B3%A2/)
> 
> *`cv.getGaussianKernel()`是Python的写法，C++写法为`cv::getGaussianKernel()`，这是个`cv::Mat`类型的方法。*

高斯滤波可以用于形成高斯模糊，通过这一点可以在CSS中实现毛玻璃效果。按照上述引用的代码，我用C++实现了一下：

```c++
cv::GaussianBlur(image, GaussianBlurImage,cv::Size(25,25),0) // 经测试，高斯卷积核越大，模糊效果越明显
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761289079879-ddd78389-5be5-4492-80dc-c632e4c4f655.png)

和CSS的`blur()`不同，高斯卷积核宽高的值必须为偶数，否则运行会报错：

```
terminate called after throwing an instance of 'cv::Exception'
what():  OpenCV(4.6.0) ./modules/imgproc/src/smooth.dispatch.cpp:293: error: (-215:Assertion failed) ksize.width > 0 && ksize.width % 2 == 1 && ksize.height > 0 && ksize.height % 2 == 1 in function 'createGaussianKernels'
```

中值滤波：

> 中值滤波是一种非线性滤波方法，它将图像中每个像素的值替换为其邻域内所有像素值的中值。这种方法在去除椒盐噪声时效果非常好。
> 
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-image-operator.html)

> 椒盐噪声也称为脉冲噪声，是图像常见的一种噪声，为随机出现的白点或者黑点，可能是亮的区域有黑色像素或是在暗的区域有白色像素（或是两者皆有）。椒盐噪声的成因可能是影像信号受到突如其来的强烈干扰而产生、模数转换器或比特传输错误等。例如失效的感应器导致像素值为最小值，饱和的感应器导致像素值为最大值。
> 
> 出自[Wikipedia](https://zh.wikipedia.org/zh-cn/%E6%A4%92%E7%9B%90%E5%99%AA%E5%A3%B0)

```c++
cv::medianBlur(src, dst, ksize);
```

形参：

- `src`: `cv::InputArray`类型变量，输入图像；支持1、3、4通道图像；当卷积核大小为`3`或`5`时， 图像深度应为`CV_8U` `CV_16U` `CV_32F`，而对于更大的卷积核，其只能为`CV_8U`
- `dst`: `cv::OutputArray`类型变量，目标图像，需要和源图片有一样的尺寸和类型
- `ksize`: `int`类型变量，卷积核大小，必须为大于`1`的奇数

实践一下：

先给图像添加椒盐噪声：

```c++
cv::Mat noisyImage = image.clone();
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution dist_rows(0, image.rows - 1);
std::uniform_int_distribution dist_cols(0, image.cols - 1);

for (int k = 0; k < 2048; k++) {
    int y = dist_rows(gen);
    int x = dist_cols(gen);

    noisyImage.at<cv::Vec3b>(y, x)[0] = 255;
    noisyImage.at<cv::Vec3b>(y, x)[1] = 255;
    noisyImage.at<cv::Vec3b>(y, x)[2] = 255;
}
for (int k = 0; k < 2048; k++) {
    int y = dist_rows(gen);
    int x = dist_cols(gen);

    noisyImage.at<cv::Vec3b>(y, x)[0] = 0;
    noisyImage.at<cv::Vec3b>(y, x)[1] = 0;
    noisyImage.at<cv::Vec3b>(y, x)[2] = 0;
    }
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761292575448-90d25866-e7fc-45f5-9957-872b70eacce6.png)

然后进行中值滤波：

```c++
cv::Mat medianBlurImage;
cv::medianBlur(noisyImage, medianBlurImage, 3);
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761292764978-41ef7a42-afb6-439a-b1d0-d6b3743e412a.png)

椒盐噪声几乎全部清除，但是边缘变得不清晰了。经测试，`ksize`值越大，边缘模糊越严重。

双边滤波：

> 双边滤波在降低噪音方面非常有效，同时保持边缘清晰。但与其他过滤器相比，操作速度较慢。
> 
> 双边滤波器在空间中也采用高斯滤波器，但是还有一个高斯滤波器是像素差的函数。空间的高斯函数确保仅考虑附近的像素用于模糊，而强度差的高斯函数确保仅考虑具有与中心像素相似的强度的像素用于模糊。因此它保留了边缘，因为边缘处的像素将具有较大的强度变化。
> 
> 出自[opencv-python-tutorials.readthedocs.io](https://opencv-python-tutorials.readthedocs.io/zh/latest/4.%20OpenCV%E4%B8%AD%E7%9A%84%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86/4.4.%20%E5%9B%BE%E5%83%8F%E6%BB%A4%E6%B3%A2/)

```c++
cv::bilateralFilter(src, dst, d, sigmaColor, sigmaSpace, borderType);
```

形参：

- `src`: `cv::InputArray`类型变量，输入图像，需要为8位或者浮点型1、3通道的图像
- `dst`: `cv::OutputArray`类型变量，即目标图像，需要和源图片有一样的尺寸和类型
- `d`: `int`类型变量，表示在过滤过程中每个像素邻域的直径。如果这个值我们设其为非正数，那么OpenCV会从形参`sigmaSpace`来计算出`d`。
- `sigmaColor`: `double`类型变量，颜色空间滤波器的`sigma`值。这个参数的值越大，就表明该像素邻域内有更宽广的颜色会被混合到一起，产生较大的半相等颜色区域
- `sigmaSpace`: `double`类型变量，坐标空间中滤波器的`sigma`值，坐标空间的标注方差。数值越大，意味着越远的像素会相互影响，从而使更大的区域足够相似的颜色获取相同的颜色。当`d > 0`，`d`指定了邻域大小且与`sigmaSpace`无关；否则，`d`正比于`sigmaSpace`
- `borderType`: `int`类型变量，用于推断图像外部像素的某种边界模式。默认值为`BORDER_DEFAULT`

在中值滤波的实践中，如果换用双边滤波：

```c++
cv::Mat bilateralFilterImage;
cv::bilateralFilter(image, bilateralFilterImage, 3, 75, 75);
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761293150454-5ca06df5-7b2d-4d94-b50d-6638f5901d31.png)

椒盐噪声几乎全部清除，边缘仍然清晰。

如果`d`设为`0`或者一个很大的数（比如`114`），输出的图像会“褪色”。当`d`等于`0`时：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761293398204-fb3a951b-a1aa-4a1b-a2a2-addf9ce37241.png)

## 图像边缘检测（图形提取）

> 边缘检测是图像处理中的一个重要任务，用于识别图像中物体的边界。常用的边缘检测方法包括Sobel算子和Canny边缘检测。
>
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-image-operator.html)

Sobel算子：

> Sobel算子是一种基于梯度的边缘检测方法，它可以检测图像中的水平和垂直边缘。
>
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-image-operator.html)

```c++
cv::Mat grad_x, grad_y;
cv::Sobel(src, grad_x, CV_16S, 1, 0); // 水平方向
cv::Sobel(src, grad_y, CV_16S, 0, 1); // 垂直方向
cv::convertScaleAbs(grad_x, grad_x); // 增强图像
cv::convertScaleAbs(grad_y, grad_y);
cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, dst); // 合并结果
```

将输出图像的“反色铅笔线稿”：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761294308776-35b24b23-4407-40e9-9119-17ca08ccd64a.png)

Canny 边缘检测：

> Canny 边缘检测是一种多阶段的边缘检测算法，它能够有效地检测出图像中的边缘，并且对噪声具有较强的鲁棒性。
>
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-image-operator.html)

```c++
cv::Canny(images, edges, 100, 200);  // 阈值1（最小值） = 100，阈值2（最大值） = 200。以这两个参数进行明显的边缘检测
```

将输出图像的“反色黑白线稿”：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761294478110-eec1788b-5783-47d2-acc5-7a1f89d55906.png)

## 图像形态学操作

> 形态学操作是基于图像形状的一系列操作，常用于图像的前景和背景分离、噪声去除等任务。常见的形态学操作包括腐蚀、膨胀、开运算、闭运算和形态学梯度。
> 
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-image-operator.html)

腐蚀：

> 腐蚀操作可以消除图像中的小物体或细节，使得前景物体变小。
> 
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-image-operator.html)

```c++
cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
cv::erode(src, dst, kernel);
```

输出图像边缘变粗，看起来更像水彩画了：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761294976114-9907c196-1350-4b73-a45b-ce7f80645f4b.png)

膨胀：

> 膨胀操作可以扩大图像中的前景物体，常用于填补前景物体中的空洞。
> 
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-image-operator.html)

```c++
cv::dilate(src, dst, kernel);
```

输出图像边缘弱化，整体变得模糊（处理后的凯尔希的脸有点难绷）：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761296574137-985187bb-ccd8-4a4f-b1a9-7d2296652aa5.png)

开运算：

> 开运算是先腐蚀后膨胀的操作，常用于去除小物体或噪声。
> 
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-image-operator.html)

```c++
cv::morphologyEx(src, dst, cv::MORPH_OPEN, kernel);
```

输出图像看起来像是先经过了模糊化处理，然后重新勾勒了一遍边缘：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761296772929-136db41c-f897-4b34-ab1e-d44b845f2d6e.png)

闭运算：

> 闭运算是先膨胀后腐蚀的操作，常用于填补前景物体中的小孔。
> 
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-image-operator.html)

```c++
cv::morphologyEx(src, dst, cv::MORPH_CLOSE, kernel);
```

输出图像看起来像水墨画（膨胀之后再作侵蚀处理使得凯尔希的脸正常多了）：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761296852220-716c426c-945b-477a-80e4-cc8293a63e5c.png)

形态学梯度：

> 形态学梯度是膨胀和腐蚀的差值，可以用于提取物体的边缘。
> 
> 出自[runoob.com](https://www.runoob.com/opencv/cpp-opencv-image-operator.html)

```c++
cv::morphologyEx(src, dst, cv::MORPH_GRADIENT, kernel);
```

输出图像和Sobel算子检测边缘的输出图像相似，前者和后者相比，整体更加模糊，细枝末节的线条更少：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761297028340-cf62ca46-562d-4e5f-8884-9ffaf0f91065.png)

## 边缘提取

```c++
cv::findContours(image, contours, hierarchy, mode, method, offset)
```

形参：

- `image`: `cv:InputOutputArray`类型变量，需要提取边缘的图像
- `contours`: `cv::OutputArrayOfArrays`类型变量，提取出的点的集合将输出到这个对象中（类型可以是`std::vector<std::vector<cv::Point> >`）
- `hierarchy`: `cv::OutputArray`类型变量，提取出的向量的集合将输出这个对象中（类型可以是`std::vector<cv::Vec4i>`）
- `mode`: `int`类型变量，
- `method`: `int`类型变量，
- `offset`: `cv::Point`类型变量，填充轮廓上所有点的坐标的偏移量，默认值为`cv::Point()`，也就是不偏移

在我看来，`contours`的结构可以这样理解：`{{封闭图形A的边缘上的点}, {封闭图形B的边缘上的点}, ...}`

实践部分见[绘制几何图形和文字](#绘制几何图形和文字)的绘制边缘部分。

# 图形绘制和文字绘制

## 点和向量

OpenCV中用`cv::Point`类表示二维平面上的一个点，其包含两个属性`x` `y`。

比如：

```c++
cv::Point pt = cv::Point(114, 514); // 默认为整型。按这个方法定义时可改用auto的定义方法，下同。
// OpenCV还提供了以下类用来表示二维平面上的一个点（类似于cv::Size2d等）：
// cv::Point2d 双精度浮点型
// cv::Point2f 浮点型
// cv::Point2i 整型
// cv::Point2l 长整型

// 若要表示三维空间内一个点，则用Point3_类，使用例如下：
cv::Point3_ pt_3 = cv::Point3_(19, 19, 810); //若类模板实参类型无法推导，则需要加上<typename>
// OpenCV还提供了以下类用来表示三维空间内的一个点：
// cv::Point3d 双精度浮点型
// cv::Point3f 浮点型
// cv::Point3i 整型
```

访问并输出`pt`中的属性：

```c++
std::cout << "pt is " << pt.x << ' ' << pt.y << std::endl;
std::cout << "pt_3 is " << pt.x << ' ' << pt.y << ' ' << pt.z << std::endl;
```

向量则用`cv::Scalar`表示，定义方法和`cv::Point`类似。最高可定义4个维度。

```c++
auto s_1 = cv::Scalar(947);
auto s_2 = cv::Scalar(9, 47);
auto s_3 = cv::Scalar(9, 4, 7);
auto s_4 = cv::Scalar(0, 7, 2, 1);
```

访问数据则使用`[]`（在我看来，本质上是一个以长度为4的数组为元素的结构体）：

```c++
    std::cout << "s_4 is " << s_4[0] << '' << s_4[1] << '' << s_4[2] << '' << s_4[3] << std::endl;
```

## 绘制几何图形和文字

绘制线条：

```c++
cv::line(img, pt1, pt2, color, thickness, lineType, shift)
```

形参：

- `img`: `cv:InputOutputArray`类型变量，需要绘制线的图像
- `pt1` `pt2`: `cv::Point`类型变量，线段的两端点坐标
- `color`: `cv::Scalar`类型常量，线的颜色
- `thickness`: `int`类型变量，线的粗细，单位为px，默认值为`1`
- `lineType`: `int`类型变量，线的类型，默认值为`LINE_8`
- `shift`: `int`类型变量，点坐标中的小数位数，默认值为`0`

比如，普瑞赛斯要在凯尔希的脖子上画一条线：

```c++
cv::line(image, cv::Point(88, 340), cv::Point(209, 319), cv::Scalar(0, 0, 0), 10);
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761319938172-69d95b65-8448-42c2-923d-9e6634a42aa9.png)

绘制矩形：

```c++
cv::rectangle(img, rec, color, thickness, lineType, shift) // 以cv::Rect类绘制
cv::rectangle(img, pt1, pt2, color, thickness, lineType, shift) // 以对角线两端点坐标绘制
```

形参：

- `img`: `cv::InputOutputArray`类型变量，需要绘制矩形的图像
- `rec`: `cv::Rect`类型变量，矩形
- `pt1` `pt2`: `cv::Point`类型变量，对角线两端点坐标
- `color`: `cv::Scalar`类型常量，边框颜色
- `thickness`: `int`类型变量，边框粗细，单位为px，默认值为`1`
- `lineType`: `int`类型变量，边框线条类型，默认值为`LINE_8`
- `shift`: `int`类型变量，点坐标中的小数位数，默认值为`0`

其中，`cv::Rect`是一个表示矩形的类，有如下属性：

- `_x` `_y`: 矩形的左上角坐标
- `_width` `_height`: 矩形的宽度和高度

比如，普瑞赛斯要画一个红框来框住凯尔希：

```c++
cv::rectangle(image, cv::Rect(76, 223, 123, 123), cv::Scalar(0, 0, 255), 5); // 以cv::Rect类绘制
cv::rectangle(image, cv::Point(76, 223), cv::Point(199, 346), cv::Scalar(0, 0, 255), 5); // 以对角线两端点坐标绘制
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761320196109-8df4359a-4b35-4206-9cf8-189b576f825c.png)

绘制椭圆：

```c++
cv::ellipse(img, center, axes, angle, startAngle, endAngle, color, thickness, lineType, shift);
```

形参：

- `img`: `cv::InputOutputArray`类型变量，需要绘制椭圆的图像
- `center`: `cv::Point`类型变量，椭圆中心坐标
- `axes`: `cv::Size`类型变量，椭圆横、纵向长度
- `angle`: `double`类型变量，顺时针旋转角度
- `startAngle` `endAngle`: `double`类型变量，从左上角逆时针,绘制多少度的椭圆
- `color`: `cv::Scalar`类型常量，边框颜色
- `thickness`: `int`类型变量，边框粗细，单位为px，默认值为`1`
- `lineType`: `int`类型变量，边框线条类型，默认值为`LINE_8`
- `shift`: `int`类型变量，点坐标中的小数位数，默认值为`0`

比如，普瑞赛斯要画一个红色椭圆来圈住凯尔希：

```c++
cv::ellipse(image, cv::Point(146, 294), cv::Size(90, 70), 0, 0, 360, cv::Scalar(0, 0, 255), 5);
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761321028514-a1c04345-a63a-4ace-8ce6-34c6140fb62e.png)

绘制圆：

```c++
cv::circle(img, center, radius, color, thickness, lineType, shift);
```

形参：

- `img`: `cv::InputOutputArray`类型变量，需要绘制圆的图像
- `center`: `cv::Point`类型变量，圆心坐标
- `radius`: `int`类型变量，半径大小
- `color`: `cv::Scalar`类型常量，边框颜色
- `thickness`: `int`类型变量，边框粗细，单位为px，默认值为`1`
- `lineType`: `int`类型变量，边框线条类型，默认值为`LINE_8`
- `shift`: `int`类型变量，点坐标中的小数位数，默认值为`0`

比如，普瑞赛斯要画一个红色圆来圈住凯尔希：

```c++
cv::circle(image, cv::Point(146, 294), 90, cv::Scalar(0, 0, 255), 5);
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761321355852-59c36968-ee22-4aa3-afc9-0a1d41be5d28.png)

绘制多边形：

```c++
cv::polylines(img, pts, isClosed, color, thickness, lineType, shift);
```

形参：

- `img`: `cv::InputOutputArray`类型变量，需要绘制多边形的图像
- `pts`: `cv::InputArrayOfArrays`类型变量，多边形的顶点集合
- `isClosed`: `bool`类型变量，多边形是否封闭
- `color`: `cv::Scalar`类型常量，边框颜色
- `thickness`: `int`类型变量，边框粗细，单位为px，默认值为`1`
- `lineType`: `int`类型变量，边框线条类型，默认值为`LINE_8`
- `shift`: `int`类型变量，点坐标中的小数位数，默认值为`0`

比如，普瑞赛斯要画一个红色的猫的形状来圈住凯尔希：

```c++
std::vector pts = {
        cv::Point(77, 217),
        cv::Point(114, 235),
        cv::Point(148, 230),
        cv::Point(177, 200),
        cv::Point(177, 253),
        cv::Point(191, 280),
        cv::Point(175, 318),
        cv::Point(127, 328),
        cv::Point(95, 301),
        cv::Point(91, 265)
    };
    cv::polylines(image, pts, true, cv::Scalar(0, 0, 255), 5);
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761325312831-3c09c2b0-5fc3-47d6-8ffb-385a6f31d3da.png)

多边形填充：

cv::fillPoly(img, pts, color, lineType, shift, offset);

形参：

- `img`: `cv::InputOutputArray`类型变量，需要绘制多边形的图像
- `pts`: `cv::InputArrayOfArrays`类型变量，多边形的顶点集合
- `color`: `cv::Scalar`类型常量，边框颜色
- `lineType`: `int`类型变量，边框线条类型，默认值为`LINE_8`
- `shift`: `int`类型变量，点坐标中的小数位数，默认值为`0`
- `offset`: `cv::Point`类型变量，填充轮廓上所有点的坐标的偏移量，默认值为`cv::Point()`，也就是不偏移

比如，在用红色的猫的形状来圈住凯尔希之后，普瑞赛斯又想把这个形状用红色填充起来：

```c++
cv::fillPoly(image, pts, cv::Scalar(0, 0, 255));
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761325582088-065eb7f3-b9ae-435e-87d0-510ec3767865.png)

绘制文字：

```c++
cv::putText(img, text, org, fontFace, fontScale, color, thickness, lineType, bottomLeftOrigin)
```

形参：

- `img`: `cv::InputOutputArray`类型变量，需要绘制文字的图像
- `text`: `cv::String`类型常量，添加的文字
- `org`: `cv::Point`类型变量，文字左下角坐标位置
- `fontFace`: `int`类型变量，字体类型
- `fontScale`: `double`类型变量，文字缩放尺度
- `color`: `cv::Scalar`类型常量，边框颜色
- `thickness`: `int`类型变量，边框粗细，单位为px，默认值为`1`
- `lineType`: `int`类型变量，边框线条类型，默认值为`LINE_8`
- `shift`: `int`类型变量，点坐标中的小数位数，默认值为`0`

比如，在用红框来框住凯尔希之后，普瑞赛斯又想在红框上面绘制凯尔希的名字：

```c++
cv::putText(image, "Kal'tsit", cv::Point(76, 215), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255), 2);
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761326547569-fc48c861-fbee-4e3f-b8ba-52e857eca398.png)

绘制边缘：

```c++
cv::drawContours(image, contours, contourIdx, color, thickness, lineType, hierarchy, maxLevel, offset);
```

形参：

- `img`: `cv::InputOutputArray`类型变量，需要绘制文字的图像
- `contours`: `cv::OutputArrayOfArrays`类型变量，边缘的点的集合
- `contourIdx`: `int`类型变量，需要绘制的边缘的点的范围，若为负数，则将`contours`中的点全部画出
- `color`: `cv::Scalar`类型常量，边缘颜色
- `thickness`: `int`类型变量，边缘线条粗细，单位为px，默认值为`1`
- `lineType`: `int`类型变量，边缘线条类型，默认值为`LINE_8`
- `hierarchy`: `cv::OutputArray`类型变量，边缘的向量的集合；如果需要只画出`contours`的一部分点，那么这个形参才是需要的，否则不需要；默认值为`noArray()`
- `maxLevel`: `int`类型变量，绘制的点的最大等级，默认值为`2147483647`
- `offset`: `cv::Point`类型变量，填充轮廓上所有点的坐标的偏移量，默认值为`cv::Point()`，也就是不偏移

比如，普瑞赛斯想要让凯尔希的边缘“发出红光”：

```c++
int main() {
    
    cv::Mat
    image = cv::imread("../resources/Kal'tsit.png"),
    GaussianImage, CannyImage, dilateImage;
    
    // 图像预处理
    cv::GaussianBlur(image, GaussianImage, cv::Size(3, 3), 0);
    cv::Canny(GaussianImage, CannyImage, 25, 75);
    cv::dilate(CannyImage, dilateImage, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    
    // 提取边缘
    std::vector<std::vector<cv::Point> > contours, contoursKaltist;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(dilateImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    // 只保留包围面积最大的边缘点集合
    std::map<int, std::vector<cv::Point> > contourArea;
    for (const auto & contour : contours) {
        contourArea.emplace(cv::contourArea(contour), contour);
    }
    auto iter = contourArea.end();
    --iter;
    contoursKaltist.push_back(iter->second);
    
    // 绘制边缘
    cv::drawContours(image, contoursKaltist, -1, cv::Scalar(0, 0, 255), 3);
    
    cv::imshow("image", image);
    while (cv::waitKey(0) != 27) {}
    cv::destroyAllWindows();
    
    return 0;
}
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761385737292-a7f485fa-5d7c-42f2-86e2-35d053ab5730.png)

利用边缘提取来框住目标：

普瑞赛斯希望“现代化”一点，要利用边缘提取来生成一个框住凯尔希的矩形，于是在上个实例的基础上，定义了一个`mark()`方法，并将提取边缘的部分转移到这个方法内：

```c++
void mark(const cv::Mat & inputImage, cv::Mat drownImage) {

    cv::Mat GaussianImage, CannyImage, dilateImage;

    std::vector<std::vector<cv::Point> > contours, contoursKaltist;
    std::vector<cv::Vec4i> hierarchy;

    // 提取边缘
    cv::findContours(inputImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    std::map<int, std::vector<cv::Point> > contourArea;
    for (const auto & contour : contours) {
        contourArea.emplace(cv::contourArea(contour), contour);
    }
    auto iter = contourArea.end();
    --iter;
    contoursKaltist.push_back(iter->second);
    
    // 绘制边缘
    cv::drawContours(drownImage, contoursKaltist, -1, cv::Scalar(0, 0, 255), 3);

    std::vector<cv::Point> conPoly;
    cv::Rect boundRect;

    double peri = cv::arcLength(contoursKaltist[0], true); // 计算轮廓周长；形参closed值为true时，集合内的点强制围成封闭图形
    cv::approxPolyDP(contoursKaltist[0], conPoly, 0.02 * peri, true); // 将连续光滑曲线折线化，减少用于构成轮廓的点
    boundRect = cv::boundingRect(conPoly); // 返回能够包围轮廓的最小矩形
    cv::rectangle(drownImage, boundRect.tl(), boundRect.br(), cv::Scalar(0, 0, 255), 10); // 绘制矩形
    
}
```

相应的，`main()`方法则修改成：

```c++
int main() {

    cv::Mat
    image = cv::imread("../resources/Kal'tsit.png"),
    GaussianImage, CannyImage, dilateImage;

    // 图像预处理
    cv::GaussianBlur(image, GaussianImage, cv::Size(3, 3), 0);
    cv::Canny(GaussianImage, CannyImage, 25, 75);
    cv::dilate(CannyImage, dilateImage, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

    mark(dilateImage, image);

    cv::imshow("image", image);
    while (cv::waitKey(0) != 27) {}
    cv::destroyAllWindows();

    return 0;
}
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761392145970-7d80bca3-9c61-412e-b5dc-072072bd6d70.png)

[语雀笔记页面](https://www.yuque.com/u29112212/ucgrla/ykahds8z3ctgzxym)