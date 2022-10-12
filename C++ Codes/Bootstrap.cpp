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

//һ��sampleing��80��stock in each group
//һ��vector��������vector��ÿ��vector����80��stock
//return:3*80 stock pointer

vector<vector<Stock*>> Bootstrap::Sampling(int size) {
    
    //vector<vector<Stock*>> symbolList; ��privete data
    vector<vector<Stock*>> sampleresult((int)symbolList.size()); // size: 3*80  result of one sampling //(int)symbolList.size()
    vector<Stock*> sampleOfOneGroup(size); //size��80
    vector<int> samplelist; //����������ÿ��ѭ���ͻ�������
    
    for (int i = 0; i < (int)symbolList.size(); i++) { //(int)symbolList.size()��3 ѭ��group
        //get a vector of random int
        //ÿ��groupȡ80
        while ((int)samplelist.size() < size) //(int)samplelist.size()��0 ����ʲô��û��
        {
            for (int j = 0; j < (int)(size - samplelist.size()); j++) {  //(int)(size - samplelist.size())��80
                samplelist.push_back((int)(rand() % (symbolList[i].size()))); 
                //rand()��������� 
                //ȡ�������ǰ������rescale�����group��index�������� ����ÿ��group�ж��ٹ�Ʊ Ȼ��������rescale��0��size-1 �Ϳ���ֱ�ӵ�vector of pointer��index �����϶�С��
                //symbolList[i]�ǵڼ���group   (symbolList[i].size())�����group�м�����Ʊ ���900
                //����samplelist���Ǵ洢80�������index
            }

            //����Ҫ���80�����ظ��������
            //������ôһֱһ��һ�����ɲ�����80�����ظ�
            //resizeһ�°Ѻ���յ�ȥ�� �ѿյĵط���Ų������
            //unique����ֻ��ɾ�����ڵ��ظ�
            vector<int>::iterator it;
            it = unique(samplelist.begin(), samplelist.end()); //order
            samplelist.resize(distance(samplelist.begin(), it));
            //һ��for loop�� (int)samplelist.size()���80 ����while loop
        }
        
        // int to stock pointer
        for (int j = 0; j < size; j++) {
            sampleOfOneGroup[j] = symbolList[i][samplelist[j]]; //symbolList[i]��group��900��stock //samplelist[j]�����index
        }
        sampleresult[i] = sampleOfOneGroup; //��i��group��������sampleOfOneGroup
    }
    
    return sampleresult;
    
}


//return: 3*4*2N
//һ��vector��������group vector��ÿ��vector�����ĸ������ÿ�������һ��Vector��Vector��vector<double>
//result = bootstrap.calculateResultMatrix(80, 40, iwv);
vector<vector<Vector>> Bootstrap::calculateResultMatrix(int size, int times, const Benchmark& iwv) {
    
    vector<vector<Stock*>> sample(symbolList.size());  //use for store result of one sampling  size:3*80

    // 3*4*2N
    vector<vector<Vector>> Result(symbolList.size());
    
    vector<SampleCalcul> AARCalcul(symbolList.size()); //size:3 
                                                       //AARCalcul[0] �� ���һ���AAR�� 
                                                       //AAR  vector<Vector> 40��AAR
                                                       //mean  vector 
                                                       //std   vector
    vector<SampleCalcul> CAARCalcul(symbolList.size());  //size:3 //CAARCalcul[0] �� ���һ���CAAR��mean��std
    
    OneCalcul matOfOneGroup;

    for (int t = 0; t < times; t++) { //40 times
        //cout << t+1;
        sample = Sampling(size);  //Function Sampling generate sample for all 3 group each time //size 80
        //cout << " sampling "<<endl;
        for (int i = 0; i < (int)sample.size(); i++) { // 3 groups�� sample.size()=3
            //for (int j = 0; j < (int)sample[i].size(); j++) {
                //cout <<j+1<<" " <<sample[i][j]->getSymbol() << "  ";
            //}
            //cout << endl;
            matOfOneGroup.calculate(sample[i], iwv); //sample[i]�� group 0 // return�� һ��sample��һ�����80��stockƽ����AAR
            AARCalcul[i].addData(matOfOneGroup.getAAR()); // AARCalcul[i]��group i��һ��sample��AAR��2N��ֵ
            //cout << endl;
            //cout << AARCalcul[i].getData()[t];
            CAARCalcul[i].addData(matOfOneGroup.getCAAR()); // CAARCalcul[i]��group i��һ��sample��CAAR��2N��ֵ
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
