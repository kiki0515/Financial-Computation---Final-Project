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
    // matOfOneGroup.calculate(sample[i], iwv); //sample[i]��80��Stock*
    // return�� һ��sample��һ�����80��stockƽ����AAR
    void calculate(vector<Stock*> stockList, const Benchmark& iwv) { //�������һ��group
        vector<Vector> stockAR; //  80*2N
        for (auto itr = stockList.begin(); itr != stockList.end(); itr++) //ѭ��һ��group���80��sample stock
        {
            stockAR.push_back((*itr)->calculate_AR(iwv)); //calculate_AR��stock.cpp //ÿ��stock����vector<double> 2N��AR
        }

        //calculate AAR and CAAR of one group based on AR
        // AAR/CAAR - 2N - vector<double>
        AAR.resize((int)stockAR[0].size()); //AAR private of OneCalcul matOfOneGroup  //(int)stockAR[0].size() = 2N
        CAAR.resize((int)stockAR[0].size()); //CAAR private of OneCalcul matOfOneGroup
        AAR = stockAR[0];
        for (unsigned int i = 1; i < stockAR.size(); i++) //stockAR.size()=80
        {
            AAR = (AAR * i + stockAR[i]) / (i + 1.0); //һ��sample��һ�����80��stockƽ����AAR  //vector overloading
        }
        CAAR = AAR.cumsum(); //cumsum()��vector.h
    }

    void clearData() {
        //stockAR.clear();
        AAR.clear();
        CAAR.clear();
    }

    Vector getAAR() { return AAR; }
    Vector getCAAR() { return CAAR; }

};


