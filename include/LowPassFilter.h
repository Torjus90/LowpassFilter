// This class implements a second order digital Butterworth filter.
// A possible extension would be to add an order variable and
// implement higher order Butterworth filters using cascading biquads.
// Also, you could make a Filter class and use it as a base class
// and expand it to high, low, bandpass, notch etc.
#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <algorithm>
#include <vector>

class LowPassFilter
{
public:
    LowPassFilter(double cutoffFreq, double sampingFreq);

public:
    void Reconfigure(double cutoffFreq, double samplingFreq);
    double Update(double input);

    double GetCutoffFreq();
    double GetSamplingFreq();
private:
    double m_cutoffFreq; // The desired cutoff frequency [hz]
    double m_samplingFreq; // The sampling freq [hz]

    std::vector<double> a, b; // Vectors for the a and b coefficients 
    std::vector<double> prevInput, prevOutput; // Vectors for storing previous values

private:
    void configure();
};