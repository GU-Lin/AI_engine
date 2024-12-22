# AI_deploy
Deploy AI model for different AI engine
# Build package
./make_build --opencv --tensorRT
# Model Convert
## Get onnx
clone yolov9 model 
export .pt to onnx
## Convert .onnx to .trt
export LD_LIBRARY_PATH=`pwd`/lib
./bin/trtexec --onnx=../../data/yolov9-c-converted.onnx --saveEngine=../../yolov9-c-converted.trt
# Make execute file
cmake CMakeList.txt
make
./main
