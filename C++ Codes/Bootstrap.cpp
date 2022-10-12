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

//一次sampleing有80个stock in each group
//一个vector里有三个vector，每个vector里有80个stock
//return:3*80 stock pointer

vector<vector<Stock*>> Bootstrap::Sampling(int size) {
    
    //vector<vector<Stock*>> symbolList; 是privete data
    vector<vector<Stock*>> sampleresult((int)symbolList.size()); // size: 3*80  result of one sampling //(int)symbolList.size()
    vector<Stock*> sampleOfOneGroup(size); //size是80
    vector<int> samplelist; //变量声明，每次循环就会重置了
    
    for (int i = 0; i < (int)symbolList.size(); i++) { //(int)symbolList.size()是3 循环group
        //get a vector of random int
        //每个group取80
        while ((int)samplelist.size() < size) //(int)samplelist.size()是0 里面什么都没有
        {
            for (int j = 0; j < (int)(size - samplelist.size()); j++) {  //(int)(size - samplelist.size())是80
                samplelist.push_back((int)(rand() % (symbolList[i].size()))); 
                //rand()生成随机数 
                //取余数就是把随机数rescale到这个group的index的区间上 就是每个group有多少股票 然后把随机数rescale到0到size-1 就可以直接当vector of pointer的index 余数肯定小于
                //symbolList[i]是第几个group   (symbolList[i].size())是这个group有几个股票 大概900
                //所以samplelist就是存储80个随机的index
            }

            //我们要搞出80个不重复的随机数
            //但是这么一直一个一个生成不可能80个不重复
            //resize一下把后面空的去掉 把空的地方都挪到后面
            //unique好像只会删除相邻的重复
            vector<int>::iterator it;
            it = unique(samplelist.begin(), samplelist.end()); //order
            samplelist.resize(distance(samplelist.begin(), it));
            //一次for loop后 (int)samplelist.size()变成80 跳出while loop
        }
        
        // int to stock pointer
        for (int j = 0; j < size; j++) {
            sampleOfOneGroup[j] = symbolList[i][samplelist[j]]; //symbolList[i]是group里900个stock //samplelist[j]是随机index
        }
        sampleresult[i] = sampleOfOneGroup; //第i个group的数据是sampleOfOneGroup
    }
    
    return sampleresult;
    
}


//return: 3*4*2N
//一个vector里有三个group vector，每个vector里有四个结果，每个结果是一个Vector，Vector是vector<double>
//result = bootstrap.calculateResultMatrix(80, 40, iwv);
vector<vector<Vector>> Bootstrap::calculateResultMatrix(int size, int times, const Benchmark& iwv) {
    
    vector<vector<Stock*>> sample(symbolList.size());  //use for store result of one sampling  size:3*80

    // 3*4*2N
    vector<vector<Vector>> Result(symbolList.size());
    
    vector<SampleCalcul> AARCalcul(symbolList.size()); //size:3 
                                                       //AARCalcul[0] 是 存第一组的AAR的 
                                                       //AAR  vector<Vector> 40个AAR
                                                       //mean  vector 
                                                       //std   vector
    vector<SampleCalcul> CAARCalcul(symbolList.size());  //size:3 //CAARCalcul[0] 是 存第一组的CAAR的mean和std
    
    OneCalcul matOfOneGroup;

    for (int t = 0; t < times; t++) { //40 times
        //cout << t+1;
        sample = Sampling(size);  //Function Sampling generate sample for all 3 group each time //size 80
        //cout << " sampling "<<endl;
        for (int i = 0; i < (int)sample.size(); i++) { // 3 groups， sample.size()=3
            //for (int j = 0; j < (int)sample[i].size(); j++) {
                //cout <<j+1<<" " <<sample[i][j]->getSymbol() << "  ";
            //}
            //cout << endl;
            matOfOneGroup.calculate(sample[i], iwv); //sample[i]： group 0 // return： 一个sample的一个组的80个stock平均的AAR
            AARCalcul[i].addData(matOfOneGroup.getAAR()); // AARCalcul[i]是group i的一次sample的AAR的2N个值
            //cout << endl;
            //cout << AARCalcul[i].getData()[t];
            CAARCalcul[i].addData(matOfOneGroup.getCAAR()); // CAARCalcul[i]是group i的一次sample的CAAR的2N个值
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
