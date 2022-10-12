//
//  Bootstrap.cpp
//  Final6883
//

#include<vector>
#include <algorithm>
#include <map>
#include <string>
#include "Stock.h"
#include "Bootstrap.h"
#include "SampleCalcul.h"
#include "OneCalcul.h"
#include <iostream>

using namespace std;

vector<vector<Stock*>> Bootstrap::Sampling(int size) {
    
    vector<vector<Stock*>> sampleresult((int)symbolList.size()); // size: 3*80  result of one sampling
    
    
    for (int i = 0; i < (int)symbolList.size(); i++) {
        vector<Stock*> sampleOfOneGroup(size);
        vector<int> samplelist;
        //get a vector of random int
        while ((int)samplelist.size() < size)
        {
            for (int j = 0; j < (int)(size - samplelist.size()); j++) {
                samplelist.push_back((int)(rand() % (symbolList[i].size())));
            }
            sort(samplelist.begin(), samplelist.end()); //samplelist is unordered, first we need to sort it
            vector<int>::iterator it;
            it = unique(samplelist.begin(), samplelist.end());
            samplelist.resize(distance(samplelist.begin(), it));
        }
        
        // int to stock pointer
        for (int j = 0; j < size; j++) {
            sampleOfOneGroup[j] = symbolList[i][samplelist[j]];
        }
        sampleresult[i] = sampleOfOneGroup;
    }
    
    return sampleresult;
    
}

vector<vector<Vector>> Bootstrap::calculateResultMatrix(int size, int times, const Benchmark& iwv) {
    
    vector<vector<Stock*>> sample(symbolList.size());  //use for store result of one sampling  size:3*80

    // 3*4*2N
    vector<vector<Vector>> Result(symbolList.size());
    
    vector<SampleCalcul> AARCalcul(symbolList.size()); //size:3
    vector<SampleCalcul> CAARCalcul(symbolList.size());  //size:3
    
    OneCalcul matOfOneGroup;
    
    for (int t = 0; t < times; t++) {
        //cout << t+1;
        sample = Sampling(size);  //Function Sampling generate sample for all 3 group each time
        //cout << " sampling "<<endl;
        for (int i = 0; i < (int)sample.size(); i++) { // 3 groups
            //for (int j = 0; j < (int)sample[i].size(); j++) {
                //cout <<j+1<<" " <<sample[i][j]->getSymbol() << "  ";
            //}
            //cout << endl;
            matOfOneGroup.calculate(sample[i], iwv);
            AARCalcul[i].addData(matOfOneGroup.getAAR());
            //cout << endl;
            //cout << AARCalcul[i].getData()[t];
            CAARCalcul[i].addData(matOfOneGroup.getCAAR());
            //cout << endl;
            //cout << CAARCalcul[i].getData()[t];
            matOfOneGroup.clearData();
        }
        sample.clear();
    }
    //cout << "sampling finished" << endl;
    // calculate Ave AAR, Ave CAAR, AAR std, CAAR std
    for (int i = 0; i < (int)Result.size(); i++) {  // 3 group
        Result[i].resize(4);
        AARCalcul[i].calculate();
        CAARCalcul[i].calculate();
        //Ave AAR
        Result[i][0] = AARCalcul[i].getMean();
        //Ave CAAR
        Result[i][1] = CAARCalcul[i].getMean();
        // AAR std
        Result[i][2] = AARCalcul[i].getStd();
        // CAAR std
        Result[i][3] = CAARCalcul[i].getStd();

        
    }

    return Result;
    
}
