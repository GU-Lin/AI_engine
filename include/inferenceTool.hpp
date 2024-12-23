#include<iostream>
#include<string>
#include<opencv2/opencv.hpp>
#include <NvInfer.h>
#include <NvInferRuntime.h>
#include <NvInferRuntimeCommon.h>
#include <cuda_runtime_api.h>
#include <common/buffers.h>
#include <fstream>
#include <memory>
#include <fstream>

using namespace nvinfer1;
class Logger : public ILogger {
public:
    void log(ILogger::Severity severity, const char* msg) noexcept override {
        // Only print messages for errors
        if (severity <= ILogger::Severity::kERROR) {
            std::cerr << msg << std::endl;
        }
    }
};

class inferenceTool
{
    public:
        inferenceTool() = default;
        explicit inferenceTool(std::string path){};
        ~inferenceTool(){};
        virtual void run(std::vector<float> &input, cv::Mat &output){};
        std::string m_modelPath;
};

class TRTInferenceTool : public inferenceTool
{
    public:
        TRTInferenceTool(std::string path);
        ~TRTInferenceTool();
        void run(std::vector<float> &input, cv::Mat &output) override;
    private:
        int getIOSize(char const *name);

        std::unique_ptr<IExecutionContext> m_context;
        std::shared_ptr<ICudaEngine> m_engine;
        std::unique_ptr<IRuntime> m_runtime;
        int m_inputSize = 0;
        int m_outputSize = 0;
        int m_outputBoxNum = 8400;
        int m_outputClass = 84; // w,y,w,h and 80 class-> 80+4
        Logger gLogger;
        char const* inputName;
        char const* outputName;
        void* buffers[2];
        float* hostData;

};


