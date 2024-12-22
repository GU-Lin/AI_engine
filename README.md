# AI_deploy
Deploy AI model for different AI engine base on yolov9
# Environment
* GPU : RTX3050 Laptop 4G
* opencv : 4.10
* tensorRT : release/10.6
* cuda : 12.6
* yolov9
# Build package
```
./make_build --opencv --tensorRT
```
# Model Convert
## Get onnx
clone yolov9 model and export .pt to onnx
## Convert .onnx to .trt
```
export LD_LIBRARY_PATH=`pwd`/lib
./bin/trtexec --onnx=../../data/yolov9-c-converted.onnx --saveEngine=../../yolov9-c-converted.trt
```
# Make execute file
```
cmake CMakeList.txt
make
./main
```
