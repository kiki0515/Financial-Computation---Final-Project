//
//  SampleCalcul.h
//  Final6883
//

#pragma once
#include"Stock.h"
#include<vector>
#include"Vector.h"



class SampleCalcul
{
private:
    // store AARs/CAARs of 40 sampling for one group
    vector<Vector> sampleData;
    Vector mean;
    Vector stdev;

public:

    SampleCalcul(): sampleData(vector<Vector>()), mean(Vector()), stdev(Vector()) {}
    ~SampleCalcul() {}

    void addData(Vector v)
    {
        sampleData.push_back(v);
    }

    void clearData()
    {
        sampleData.clear();
        mean.clear();
        stdev.clear();
    }

    vector<Vector> getData() { return sampleData; }

    // remember to calculate after completing the data of the matrix
    void calculate()
    {
        mean.resize((int)sampleData[0].size());
        mean = sampleData[0];
        for (int i = 1; i < (int)sampleData.size(); i++)
        {
            mean = (mean * i + sampleData[i]) / (i + 1.0);
        }
        
        stdev.resize((int)sampleData[0].size());
        stdev = (sampleData[0] - mean)* (sampleData[0] - mean);
        for (int i = 1; i < (int)sampleData.size(); i++)
        {
            stdev = stdev + (sampleData[i] - mean)* (sampleData[i] - mean);
        }
        stdev = stdev / (double)(sampleData.size() - 1);
        stdev = stdev.sqrtV();
    }

    Vector getMean() { return mean; }
    Vector getStd() { return stdev; }
};
