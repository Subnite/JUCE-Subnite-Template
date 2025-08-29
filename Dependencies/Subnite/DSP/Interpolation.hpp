#pragma once
#include <functional>
#include <vector>

enum class InterpolationType{
    LANCZOS
};

class Interpolation{
private:
    inline static std::function<double(double x)> s_lanczosInterpolation = 
    [](double x)->double{
        const double pi = 3.14159265358979323846;
        const int a = 2; // the higher a, the closer to the sinc function

        auto sinc = [pi](double x)->double{
            return sin(pi*x)/(pi*x);
        };

        if (abs(x) >= a){
            return 0.0;
        } else if (x == 0.0){
            return 1.0; // otherwise it returns -nan whatever
        }

        return (sinc(x) * sinc(x/a));
    };
public:
    template<typename DataType>
    inline static std::vector<DataType> Interpolate(
        const std::vector<DataType>& inputData, 
        unsigned int fromIndex,
        unsigned int toIndex,
        double stepSize, 
        InterpolationType type)
        {
        std::function<double(double)> kernel;
        if (type == InterpolationType::LANCZOS){
            kernel = Interpolation::s_lanczosInterpolation;
        }

        std::vector<DataType> newData;
        int amountOfPoints = toIndex-fromIndex;
        newData.reserve(amountOfPoints/stepSize + 1);
        //newData.resize(inputData.size()/stepSize);

        for (double currentPos = 0; currentPos <= amountOfPoints; currentPos+=stepSize){
            DataType summedResult = 0;
            for (int i = 0; i < inputData.size(); i++){
                double kernelMultiplier = kernel(i-currentPos-fromIndex);
                summedResult += kernelMultiplier*inputData[i];
            }

            newData.push_back(summedResult);
        }

        return newData;
    }
};