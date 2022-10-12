//
//  OneCalcul.h
//  Final6883
//


#pragma once
#include"Stock.h"
#include<vector>
#include"Vector.h"

class OneCalcul
{
private:
    // use OneCalcul to get AR of each stock in a group
    //vector<Vector> stockAR; // Vectors are AR of each stock
    Vector AAR;
    Vector CAAR;

public:
    
    OneCalcul(): AAR(Vector()), CAAR(Vector()){}
    
    ~OneCalcul() {}

    void calculate(vector<Stock*> stockList, const Benchmark& iwv) {
        vector<Vector> stockAR;
        for (auto itr = stockList.begin(); itr != stockList.end(); itr++)
        {
            stockAR.push_back((*itr)->calculate_AR(iwv));
        }

        //calculate AAR and CAAR of one group based on AR
        AAR.resize((int)stockAR[0].size());
        CAAR.resize((int)stockAR[0].size());
        AAR = stockAR[0];
        for (unsigned int i = 1; i < stockAR.size(); i++)
        {
            AAR = (AAR * i + stockAR[i]) / (i + 1.0);
        }
        CAAR = AAR.cumsum();
    }

    void clearData() {
        //stockAR.clear();
        AAR.clear();
        CAAR.clear();
    }

    Vector getAAR() { return AAR; }
    Vector getCAAR() { return CAAR; }

};


