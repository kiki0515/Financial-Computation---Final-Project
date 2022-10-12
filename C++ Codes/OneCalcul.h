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

    // in bootstrap.cpp
    // OneCalcul matOfOneGroup;
    // matOfOneGroup.calculate(sample[i], iwv); //sample[i]是80个Stock*
    // return： 一个sample的一个组的80个stock平均的AAR
    void calculate(vector<Stock*> stockList, const Benchmark& iwv) { //输入的是一个group
        vector<Vector> stockAR; //  80*2N
        for (auto itr = stockList.begin(); itr != stockList.end(); itr++) //循环一个group里的80个sample stock
        {
            stockAR.push_back((*itr)->calculate_AR(iwv)); //calculate_AR在stock.cpp //每个stock都有vector<double> 2N个AR
        }

        //calculate AAR and CAAR of one group based on AR
        // AAR/CAAR - 2N - vector<double>
        AAR.resize((int)stockAR[0].size()); //AAR private of OneCalcul matOfOneGroup  //(int)stockAR[0].size() = 2N
        CAAR.resize((int)stockAR[0].size()); //CAAR private of OneCalcul matOfOneGroup
        AAR = stockAR[0];
        for (unsigned int i = 1; i < stockAR.size(); i++) //stockAR.size()=80
        {
            AAR = (AAR * i + stockAR[i]) / (i + 1.0); //一个sample的一个组的80个stock平均的AAR  //vector overloading
        }
        CAAR = AAR.cumsum(); //cumsum()在vector.h
    }

    void clearData() {
        //stockAR.clear();
        AAR.clear();
        CAAR.clear();
    }

    Vector getAAR() { return AAR; }
    Vector getCAAR() { return CAAR; }

};


