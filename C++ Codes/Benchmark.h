//
//  Benchmark.h
//  Final6883
//


#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

typedef pair<string, double> PAIR; // pair of ticker and surprise%
class Benchmark
{
private:
    string iSymbol;
    // int -- index
    // pair<string, double> -- Date (string) and Close Price (double)
    map<int, PAIR> indexedTrades;  //store IWV data from librul
    int N;

public:
    Benchmark() : iSymbol(""), N(0) {}
    Benchmark(string iSymbol_) : iSymbol(iSymbol_), N(0) {}
    Benchmark(const Benchmark& benchmark) : iSymbol(benchmark.iSymbol), indexedTrades(benchmark.indexedTrades), N(benchmark.N) {}
    
    // add pair of <int(KEY), PAIR(value)>& indexedTrade 进入 map indexedTrades
    void addIndexedTrade(const pair<int, PAIR>& indexedTrade) { indexedTrades.insert(indexedTrade); }
    
    const map<int, PAIR>& getIndexedTrade() const { return indexedTrades; }
    string getSymbol() const { return iSymbol; }
    //static int N;  // the window size
    //static int GetN() { return N; }
    int GetN() const { return N; }
    void SetN(int n) { N = n; }

    // cout iwv data
    friend ostream& operator<<(ostream& ostr, const Benchmark& benchmark) {
        ostr << "Symbol: " << benchmark.iSymbol << endl;
        for (auto itr = benchmark.indexedTrades.begin(); itr != benchmark.indexedTrades.end(); itr++) {
            ostr << itr->first << '\t' << itr->second.first << '\t' << itr->second.second << endl;
        }
        return ostr;
    }

    //找announcement day
    int DayFind(string date) const
    {
        // indexedTrades已经有了 index <date, price>  - map<int, PAIR>
        for (auto iter = indexedTrades.begin(); iter != indexedTrades.end(); iter++) //indexedTrades.begin() 是 map的第一个
        {
            //数据从前往后筛选
            if (iter->second.first >= date) return iter->first; //iter->second.first是iwv的date //返回的就是day0的 iter->first是index 
            //通过大于：some firms release report on weekends (eg: CE, ON), so we will need to adjust the day0 to next trading day.
        }
        return (int)indexedTrades.size(); //iwv的长度
    }


    // 最后面的 第N天的date
    string NextNDays(string announceDate) const
    {
        int day_index = DayFind(announceDate);
        if (day_index + N > (int)indexedTrades.size()) {
            cout << "Benchmark data after the announcement date is not enough!" << endl;
            return indexedTrades.at((int)indexedTrades.size()).first; //iwv最后一个日期
        }
        else {
            return indexedTrades.at(day_index + N).first; //是 <date, price> 里的 date
            //map.at根据key找到value //Returns a reference to the mapped value of the element identified with key k.
            // key是day_index + N
            // indexedTrades.at(day_index + N) 是 value  即<date, price>
        }
    }

    // 最前面的 第-N天的date
    string PrevNDays(string announceDate) const
    {
        int day_index = DayFind(announceDate);
        if (day_index - N < 1) {
            cout << "Benchmark data before the announcement date is not enough!" << endl;
            return indexedTrades.at(1).first; //iwv第一个日期
        }
        else {
            return indexedTrades.at(day_index - N).first;

        }
    }

};
