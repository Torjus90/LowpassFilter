#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <string>
#include "include/LowPassFilter.h"
#include <fstream>

using namespace std;

// The test function.
// Could by all means be a member function of the LowPassFilter class
void testFilter(LowPassFilter &filter);

int main()
{
    double samplingFreq = 500.0;
    double cutoffFreq = 15;
    LowPassFilter filter(cutoffFreq, samplingFreq);

    testFilter(filter);

    return 0;
}

// There are many ways to check the correctness of a filter, e.g. DFT analysis using FFT, visual comparison between input and output sequence etc.
// However, implementing these tests would require third-party libraries.
// Instead, we will do the following tests
// 1: Run an input sequence of two sinusoids added together through the filter and save both input and output to a .csv to check it visually in e.g. Excel
// 2: Test sinusoids with increasing frequencies for a couple of different cut off frequencies and observe the magnitude response.
// 3: Check if an impulse response decays over time
void testFilter(LowPassFilter &filter)
{
    // ------------------------------------------------------------------------
    // First test. Two different, superimposed sinusoids.
    //
    double freq1 = 5.0;
    double freq2 = 200.0;
    
    double cutoffFreq = filter.GetCutoffFreq();
    double samplingFreq = filter.GetSamplingFreq();

    ofstream inputFile, outputFile;
    inputFile.open("input.csv");
    outputFile.open("output.csv");

    double in, out;

    for (int t = 0; t < samplingFreq * 2; t++) //Run for two seconds
    {
        // Generate an input signal of two sinusoids of different frequencies and amplitudes, emulating a signal with HF noise
        double in = sin(t / samplingFreq * 2 * M_PI * freq1) + 0.1 * sin(t / samplingFreq * 2 * M_PI * freq2);
        
        inputFile << in << ",";
        outputFile << filter.Update(in) << ",";
    }

    inputFile.close();
    outputFile.close();

    // ------------------------------------------------------------------------
    // Second test, varying frequencies and cutoffs. Note that for the last cutoff
    // frequency (90Hz), we go beyond samplingFreq/2, causing the filter to go haywire 
    //
    double testFreq;

    double magnitude;    

    // Testing with factorial cutoffFreq increase
    for (int i = 1; i <= 3; i++)
    {
        filter.Reconfigure(filter.GetCutoffFreq() * i, samplingFreq);
        cout << "Testing filter with cutoff freq " << filter.GetCutoffFreq() << endl;

        for (int j = -3; j < 4; j++)
        {
            testFreq = filter.GetCutoffFreq() * pow(2, j); // Test with different ratios of cutoff frequency
            magnitude = 0;

            for (int t = 0; t < samplingFreq * 2; t++)
            {
                in = sin(t / samplingFreq * 2 * M_PI * testFreq);

                out = filter.Update(in);
                if (t > samplingFreq && abs(out) > magnitude) magnitude = abs(out); // Only record the magnitude for the second half in case something needs to settle
            }
            cout << "Magnitude for sine freq " << testFreq << "Hz: " << magnitude << endl;
        }
        cout << endl;
    }

    // ------------------------------------------------------------------------
    // Second test. Check that the impulse response is decaying
    //

    cout << "Testing impulse response" << endl;
    vector<double> impulseInput(20, 0.0);
    impulseInput[0] = 10.0;

    for (const auto& in : impulseInput)
    {
        cout << filter.Update(in) << ", ";
    }

    cout << endl << endl;
    
}