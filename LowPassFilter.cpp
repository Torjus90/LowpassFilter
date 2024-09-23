#include "include/LowPassFilter.h"

LowPassFilter::LowPassFilter(double cutoffFreq, double samplingFreq)
    : m_cutoffFreq(cutoffFreq)
    , m_samplingFreq(samplingFreq)
    , prevInput(2, 0)
    , prevOutput(2, 0)
{
    configure();
}

void LowPassFilter::Reconfigure(double cutoffFreq, double samplingFreq)
{
    this->m_cutoffFreq = cutoffFreq;
    this->m_samplingFreq = samplingFreq;
    configure();
}

double LowPassFilter::Update(double input)
{
    // Direct form 1 implementation of the difference equation, https://en.wikipedia.org/wiki/Digital_biquad_filter
    double output = b[0] * input + b[1] * prevInput[0] + b[2] * prevInput[1] - a[0] * prevOutput[0] - a[1] * prevOutput[1];

    // Update the previous values
    std::rotate(prevInput.rbegin(), prevInput.rbegin() + 1, prevInput.rend());
    prevInput[0] = input;
    std::rotate(prevOutput.rbegin(), prevOutput.rbegin() + 1, prevOutput.rend());
    prevOutput[0] = output;

    return output;
}

double LowPassFilter::GetCutoffFreq()
{
    return this->m_cutoffFreq;
}

double LowPassFilter::GetSamplingFreq()
{
    return this->m_samplingFreq;
}

void LowPassFilter::configure()
{
    // Make sure we start with a clean slate
    a.clear();
    b.clear();

    // This is the coefficients for a second order discretized butterworth filter with pre-warping baked into the coefficients
    // Taken from the excellent https://thewolfsound.com/bilinear-transform/

    double T = 1 / m_samplingFreq;
    double W = tan(m_cutoffFreq * 2 * M_PI * T / 2);
    double sqrt2 = sqrt(2);
    double W2 = W * W;

    // Normalize the coefficients here, so we don't have to do it every update
    double normFactor = 1 + W * sqrt2 + W2;

    a.push_back(2 * (W2 - 1) / normFactor);
    a.push_back((W2 - W * sqrt2 + 1) / normFactor);

    b.push_back(W2 / normFactor);
    b.push_back(2 * W2 / normFactor);
    b.push_back(W2 / normFactor);

}