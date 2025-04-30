#include <iostream>
#include <vector>
#include <getopt.h>
#include <dlfcn.h>
#include <opencv2/opencv.hpp>

#include "inference.h"

using namespace std;
using namespace cv;


typedef Inference (*InitModelFunc)(const char* model_path, bool runOnGPU);
typedef int (*InferenceHeaderDetModelFunc)(Inference& inf, cv::Mat frame, std::vector<Detection>& output,bool enable_logger);


// 加载动态so库
static int loadSo(const char* soPath, void*& handle) {
	handle = dlopen(soPath, RTLD_LAZY);
	if (!handle) {
		std::cerr << "Cannot open library: " << dlerror() << std::endl;
		return 1;
	}
	dlerror();  // 清除之前的错误
	return 0;
}

int main(int argc, char **argv)
{
    if (argc != 4) {
		printf("%s <model_path> <image_path> <enable_logger>\n", argv[0]);
		return -1;
	}
	int ret = 0;
	const char* model_path = argv[1];
	const char* image_path = argv[2];  // single image path
    bool enable_logger = argv[3]; // 是否开启日志和可视化图片推理结果
    // 读取图片
    cv::Mat image = cv::imread(image_path);

    // 打开动态库
	dlerror();
	void* handle = NULL;
	const char* so_path = "libheader_detector.so";
	ret = loadSo(so_path, handle);
    // 初始化模型
    bool runOnGPU = true; // 是否使用GPU推理
    InitModelFunc init_model = (InitModelFunc)dlsym(handle, "InitModel");
    Inference inf = init_model(model_path, runOnGPU);
    // 推理模型
    std::vector<Detection> output;
    InferenceHeaderDetModelFunc det_model = (InferenceHeaderDetModelFunc)dlsym(handle, "inference_header_det_model");
    ret = det_model(inf, image, output, enable_logger);
    std::cout << "output size: " << output.size() << std::endl;
    // 关闭动态库
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << dlsym_error << std::endl;
        dlclose(handle);
        return -1;
    }
    
	if (handle != nullptr) {
		dlclose(handle);
	}
	return 0;
}
