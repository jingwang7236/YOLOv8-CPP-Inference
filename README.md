# YOLOv8Inference C++

This example demonstrates how to perform inference using YOLOv8 models in C++ with OpenCV's DNN API.

## Usage

```bash
# 代码参考官方代码: ultralytics/examples/YOLOv8-CPP-Inference

# Add a **yolov8\_.onnx** and/or **yolov5\_.onnx** model(s) to the ultralytics folder.
# Edit the **main.cpp** to change the **projectBasePath** to match your user.

# Note that by default the CMake file will try and import the CUDA library to be used with the OpenCVs dnn (cuDNN) GPU Inference.
# If your OpenCV build does not use CUDA/cuDNN you can remove that import call and run the example on CPU.

mkdir build
cd build
cmake ..
make
./Yolov8CPPInference
```

## Exporting YOLOv8 Models

To export YOLOv8 models:

```commandline
# conda env: ultralytics
yolo export model=yolov8n.pt imgsz=640,640 format=onnx opset=12
```


yolov8s.onnx:

![image](https://user-images.githubusercontent.com/40023722/217356132-a4cecf2e-2729-4acb-b80a-6559022d7707.png)


This repository utilizes OpenCV's DNN API to run ONNX exported models of YOLOv8. Note that the example networks are exported with rectangular (640x480) resolutions, but any exported resolution will work. You may want to use the letterbox approach for square images, depending on your use case.

The **main** branch version uses Qt as a GUI wrapper. The primary focus here is the **Inference** class file, which demonstrates how to transpose YOLOv8 models to work as YOLOv5 models.


## 转模型问题
测试头部检测模型det_header_yolov8n_640x640_250430_sim.onnx

由于重新训练的模型和yolov8n结构不太一样，导致出现如下错误：

terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(4.11.0) /home/gzjh/gzjh/docker/opencv-4.11.0/modules/dnn/src/net_impl_fuse.cpp:608: error: (-215:Assertion failed) biasLayerData->outputBlobsWrappers.size() == 1 in function 'fuseLayers'

分析原因：OpenCV的DNN模块在尝试优化网络时失败

解决方案：在加载模型时禁用层融合优化：

```cpp
//代码：inference.cpp # 155行
cv::dnn::Net net = cv::dnn::readNetFromONNX("model.onnx");
net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

```

## 动态库使用

参考 main.cpp

算法库：shared_library/libheader_detector.so
