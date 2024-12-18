#include "../include/utils.hpp"

objectDetection::objectDetection(){};

objectDetection::~objectDetection(){};

void objectDetection::preprocess(cv::Mat input, cv::Mat &output)
{
    cv::Mat src = input.clone();
    cv::Size sizeDst(640,640);
    cv::Size sizeSrc = src.size();
    float ratioWidth = float(sizeDst.width)/float(sizeSrc.width);
    float ratioHeight = float(sizeDst.height)/float(sizeSrc.height);
    float r = ratioWidth < ratioHeight ? ratioWidth : ratioHeight;
    cv::Size sizeEnlarge(std::round(r*sizeSrc.width),std::round(r*sizeSrc.height));

    // Compute padding
    int dw = (sizeDst.width - sizeEnlarge.width)/2;
    int dh = (sizeDst.height - sizeEnlarge.height)/2;

    cv::Mat M = cv::Mat::zeros(2,3,CV_32F);
    M.at<float>(0,0) = r;
    M.at<float>(1,1) = r;
    M.at<float>(0,2) = dw;
    M.at<float>(1,2) = dh;
    std::cout << M << std::endl;
    cv::warpAffine(src,src,M,(cv::Size(640,640)),cv::INTER_LINEAR,cv::BORDER_CONSTANT,cv::Scalar(114,114,114));
    src.copyTo(output) ;

}

void objectDetection::HWC2NormalCHW(cv::Mat input, std::vector<float> &data)
{
    std::vector<cv::Mat> inputChannel(3);
    cv::Mat img;
    cv::cvtColor(input, img, cv::COLOR_BGR2RGB);
    img.convertTo(img, CV_32F, 1.0f/255.f);
    cv::split(img, inputChannel);
    std::vector<float>result;
    for(int i = 0; i < 3; i++)
    {
        std::vector<float> temp = std::vector<float>(inputChannel[i].reshape(1, 1));
        result.insert(result.end(),temp.begin(),temp.end());
    }
    data = result;
}

float objectDetection::areaBox(PredBox box)
{
    return (box.boxRight - box.boxLeft) * (box.boxDown - box.boxTop);
}

float objectDetection::iou(PredBox box1, PredBox box2)
{
    struct PredBox iouBox;
    iouBox.boxLeft = std::max(box1.boxLeft,box2.boxRight);
    iouBox.boxTop = std::max(box1.boxTop,box2.boxTop);
    iouBox.boxRight = std::max(box1.boxRight,box2.boxRight);
    iouBox.boxDown = std::max(box1.boxDown,box2.boxDown);

    float unionArea = std::max(iouBox.boxRight - iouBox.boxLeft,float(0.0)) * \
                      std::max(iouBox.boxDown - iouBox.boxTop,float(0.0));
    float crossArea = areaBox(box1) + areaBox(box2) - unionArea;
    if(crossArea == 0.0 || unionArea == 0.0)
    {
        return 0.0;
    }
    return unionArea/crossArea;
}

