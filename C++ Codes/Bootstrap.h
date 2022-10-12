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
    //1个vector里有三个vector
    //每个vector是有对应的group里那么多个Stock*的数据
    vector<vector<Stock*>> symbolList; //all the stock pointer that have enough data

public:
    //constructor
    Bootstrap(vector<map<string, Stock*>> mapList_)  { //vector<map<string, Stock*>> mapList_ 是3个group map
        
        srand((unsigned int)time(NULL)); //random时间

        vector<vector<Stock*>> symbolList_(mapList_.size()); //mapList_.size()是3
        for (unsigned int i = 0; i < mapList_.size(); i++) {
            
            map<string, Stock*> mapOfOneGroup = mapList_[i]; //提取第i个group
            map<string, Stock*>::iterator itr;
            for (itr= mapOfOneGroup.begin(); itr!= mapOfOneGroup.end(); itr++) {
                if (itr->second->getTrades().size() == 2 * itr->second->getN() + 1) { //if have enough data
                    symbolList_[i].push_back(itr->second); //itr->second是Stock*
                }
                
            }
        }
        symbolList = symbolList_; //每个group里所有stock的data
        
    }
    
    //一次sampleing有80个stock in each group
    //一个vector里有三个vector，每个vector里有80个stock
    vector<vector<Stock*>> Sampling(int size);  //return:3*80 stock pointer

    //一个vector里有三个group vector，每个vector里有四个结果，每个结果是一个Vector，Vector是vector<double>
    vector<vector<Vector>> calculateResultMatrix(int size, int times, const Benchmark& iwv); //return: 3*4*2N

};
