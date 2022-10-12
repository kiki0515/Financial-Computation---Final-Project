//
//  Bootstrap.h
//  Final6883
//


#pragma once
#include<vector>
#include <algorithm>
#include <map>
#include <string>
#include "Stock.h"
#include <cstdlib>
#include <ctime>
#include "SampleCalcul.h"
#include "Benchmark.h"

using namespace std;

class Bootstrap {
private:
    //1��vector��������vector
    //ÿ��vector���ж�Ӧ��group����ô���Stock*������
    vector<vector<Stock*>> symbolList; //all the stock pointer that have enough data

public:
    //constructor
    Bootstrap(vector<map<string, Stock*>> mapList_)  { //vector<map<string, Stock*>> mapList_ ��3��group map
        
        srand((unsigned int)time(NULL)); //randomʱ��

        vector<vector<Stock*>> symbolList_(mapList_.size()); //mapList_.size()��3
        for (unsigned int i = 0; i < mapList_.size(); i++) {
            
            map<string, Stock*> mapOfOneGroup = mapList_[i]; //��ȡ��i��group
            map<string, Stock*>::iterator itr;
            for (itr= mapOfOneGroup.begin(); itr!= mapOfOneGroup.end(); itr++) {
                if (itr->second->getTrades().size() == 2 * itr->second->getN() + 1) { //if have enough data
                    symbolList_[i].push_back(itr->second); //itr->second��Stock*
                }
                
            }
        }
        symbolList = symbolList_; //ÿ��group������stock��data
        
    }
    
    //һ��sampleing��80��stock in each group
    //һ��vector��������vector��ÿ��vector����80��stock
    vector<vector<Stock*>> Sampling(int size);  //return:3*80 stock pointer

    //һ��vector��������group vector��ÿ��vector�����ĸ������ÿ�������һ��Vector��Vector��vector<double>
    vector<vector<Vector>> calculateResultMatrix(int size, int times, const Benchmark& iwv); //return: 3*4*2N

};
