#include <iostream>
#include <vector>
#include <getopt.h>
#include <fstream>

#include <opencv2/opencv.hpp>

#include "inference.h"

using namespace std;
using namespace cv;

#ifdef __cplusplus
extern "C" {          // 确保函数名称不会在导出时被修饰
#endif
Inference InitModel(const char* model_path, bool runOnGPU)
{
    // 判断model  文件是否存在
    std::ifstream file(model_path);
    if (file.good() == false)
    {
        std::cout << "Model file does not exist!" << std::endl;
    }
    Inference inf(model_path, cv::Size(640, 640), runOnGPU); // 初始化模型
    return inf;
}

int inference_header_det_model(Inference& inf, cv::Mat frame, std::vector<Detection>& output,bool enable_logger=false)
{
    // const char* image_path;
    // cv::Mat frame = cv::imread(image_path);

    // Inference starts here...
    try {
        output = inf.runInference(frame);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }
    if (enable_logger){
        int detections = output.size();
        std::cout << "Number of detections:" << detections << std::endl;
        for (int i = 0; i < detections; ++i)
        {
            Detection detection = output[i];
            // 获取类别id和类别名，并打印
            int class_id = detection.class_id;
            std::string className = detection.className;
            cv::Rect box = detection.box;
            cv::Scalar color = detection.color;

            // Detection box
            cv::rectangle(frame, box, color, 2);

            // Detection box text
            std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4);
            cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
            cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

            cv::rectangle(frame, textBox, color, cv::FILLED);
            cv::putText(frame, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);
        }

    const char* image_path = "result.jpg";
    cv::imwrite(image_path, frame);

    }
    return 0;
}
#ifdef __cplusplus
}
#endif
