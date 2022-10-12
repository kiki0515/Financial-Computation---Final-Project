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
    vector<vector<Stock*>> symbolList; //all the stock pointer that have enough data

public:
    Bootstrap(vector<map<string, Stock*>> mapList_)  {
        
        srand((unsigned int)time(NULL));

        vector<vector<Stock*>> symbolList_(mapList_.size());
        for (unsigned int i = 0; i < mapList_.size(); i++) {
            
            map<string, Stock*> mapOfOneGroup = mapList_[i];
            map<string, Stock*>::iterator itr;
            for (itr= mapOfOneGroup.begin(); itr!= mapOfOneGroup.end(); itr++) {
                if ((int)itr->second->getTrades().size() == 2 * itr->second->getN() + 1) { //if have enough data
                    symbolList_[i].push_back(itr->second);
                }
                
            }
        }
        symbolList = symbolList_;
    }
    
    vector<vector<Stock*>> Sampling(int size);  //return:3*80 stock pointer

    vector<vector<Vector>> calculateResultMatrix(int size, int times, const Benchmark& iwv); //return: 3*4*2N

};
