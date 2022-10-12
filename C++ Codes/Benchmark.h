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
    map<int, PAIR> indexedTrades;
    int N;

public:
    Benchmark() : iSymbol(""), N(0) {}
    Benchmark(string iSymbol_) : iSymbol(iSymbol_), N(0) {}
    Benchmark(const Benchmark& benchmark) : iSymbol(benchmark.iSymbol), indexedTrades(benchmark.indexedTrades), N(benchmark.N) {}
    void addIndexedTrade(const pair<int, PAIR>& indexedTrade) { indexedTrades.insert(indexedTrade); }
    const map<int, PAIR>& getIndexedTrade() const { return indexedTrades; }
    string getSymbol() const { return iSymbol; }
    //static int N;  // the window size
    //static int GetN() { return N; }
    int GetN() const { return N; }
    void SetN(int n) { N = n; }
    friend ostream& operator<<(ostream& ostr, const Benchmark& benchmark) {
        ostr << "Symbol: " << benchmark.iSymbol << endl;
        for (auto itr = benchmark.indexedTrades.begin(); itr != benchmark.indexedTrades.end(); itr++) {
            ostr << itr->first << '\t' << itr->second.first << '\t' << itr->second.second << endl;
        }
        return ostr;
    }

    int DayFind(string date) const
    {
        // some firms release report on weekends (eg: CE, ON), so we will need to adjust the day0 to next trading day.
        
        for (auto iter = indexedTrades.begin(); iter != indexedTrades.end(); iter++)
        {
            if (iter->second.first >= date) return iter->first;
        }
        return (int)indexedTrades.size();
    }


    string NextNDays(string announceDate) const
    {
        int day_index = DayFind(announceDate);
        if (day_index + N > (int)indexedTrades.size()) {
            cout << "Benchmark data after the announcement date is not enough!" << endl;
            return indexedTrades.at((int)indexedTrades.size()).first;
        }
        else {
            return indexedTrades.at(day_index + N).first;

        }
    }

    string PrevNDays(string announceDate) const
    {
        int day_index = DayFind(announceDate);
        if (day_index - N < 1) {
            cout << "Benchmark data before the announcement date is not enough!" << endl;
            return indexedTrades.at(1).first;
        }
        else {
            return indexedTrades.at(day_index - N).first;

        }
    }

};
