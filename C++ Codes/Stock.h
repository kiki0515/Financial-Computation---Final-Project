//
//  Stock.h
//  Final6883
//

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "Benchmark.h"
#include "Vector.h"

using namespace std;

class Stock
{
private:
    // map<string, Stock*> stockMap; 每一个string/ticker 都有以下所有数据
    
    // excel 里的数据
    string sSymbol;
    string announce_date;
    string period_end;
    double estimated;
    double reported;
    double surprise;
    double surprise_pct;

    // multithread里给所有都set了
    string start_day;
    string end_day;
    
    string group;
    map<string, double> trades; // 2N+1天的date, 2N+1天的adj closed mapping //在multithread里，一个股票所有-n到n天的数据已存入 存在StockMap里second pointer指的位置的数据
    
    int N;
    
    //information for part B
    ////每个股票的数据
    //Vector里存的是vector<double> v;

    Vector daily_return；
    Vector cumulative_return; //单个股票的cumulative_return
    Vector AR; //abnormal return



public:
    Stock() :sSymbol(""), announce_date(""), period_end(""), estimated(0.0), reported(0.0), surprise(0.0), surprise_pct(0.0), start_day(""), end_day(""), N(0), group("") {}
    
    Stock(string sSymbol_, string announce_date, string period_end, double estimated, double reported, double surprise, double surprise_pct):
        sSymbol(sSymbol_), announce_date(announce_date), period_end(period_end), estimated(estimated), reported(reported), surprise(surprise),
        surprise_pct(surprise_pct), start_day(""), end_day(""),N(0), group("") {}
    
    Stock(string sSymbol_) : sSymbol(sSymbol_), announce_date(""), period_end(""), estimated(0.0), reported(0.0), surprise(0.0), 
        surprise_pct(0.0), start_day(""), end_day(""), N(0), group("") {}
    
    Stock(const Stock& stock) : sSymbol(stock.sSymbol), trades(stock.trades), announce_date(stock.announce_date), period_end(stock.period_end), 
        estimated(stock.estimated), reported(stock.estimated), surprise(stock.surprise), surprise_pct(stock.surprise_pct), start_day(stock.start_day), 
        end_day(stock.end_day), N(stock.N), daily_return(stock.daily_return), cumulative_return(stock.cumulative_return), AR(stock.AR), group("") {}



    void addTrade(const pair<string, double>& trade) { trades.insert(trade); } //multithread - 存数据
    string getSymbol() const { return sSymbol; }
    const map<string, double> & getTrades() const { return trades; } //multithread - 判断2n+1

    // StockMap里的 stock已经set过 Get Stock Class的private data
    string getAnnounceDay() const { return announce_date; } 
    string getPeriodEnd() const { return period_end; }
    double getEstimated() const { return estimated; }
    
    // Get excel里数据
    double getReported() const { return reported; }
    double getSurprise() const { return surprise; }
    double getSurprisePct() const { return surprise_pct; }
    
    // multithread里确认 -N和N日期的 给每个股票
    void setStartDay(string start_day_) { start_day = start_day_; } //benchmark里来操作 //fetchtrading_multi
    void setEndDay(string end_day_) { end_day = end_day_; }
    
    string getStartDay() const { return start_day; } // one thread里取数据用的 //fetchtrading
    string getEndDay() const{ return end_day; }
    
    // multithread
    void setN(int n) { N = n; } //fetchtrading_multi
    int getN() const { return N; } //fetchtrading


    bool checkdata() const; //check data是不是2N+1
    string get_group(const vector<map<string, Stock*>> &StockMapVec); //1，2，or3
    
    
    void calculate_Return(); // calculate stock daily return and cumulative return
    Vector calculate_AR(const Benchmark& benchmark); //calculate stock abnormal return

    Vector getReturn() const { return daily_return; }
    Vector getCumulativeReturn() const { return cumulative_return; }
    Vector getAR() const { return AR; }
    

    friend ostream& operator<<(ostream& ostr, const Stock& stock) { //输入的是一个Stock class的object
        // main里得出所有excel数据
        ostr << "Symbol: " << stock.sSymbol << endl;
        ostr << "Earning Announcement Date: " << stock.announce_date << endl;
        ostr << "Earning Period Ending: " << stock.period_end << endl;
        ostr << "Earning Estimate: " << stock.estimated << endl;
        ostr << "Reported Earning Per Share: " << stock.reported << endl;
        ostr << "Earning Suprise: " << stock.surprise << endl;
        ostr << "Earning Suprise Percentage: " << stock.surprise_pct << endl;
        ostr << "Group: " << stock.group << endl; //main里算的
        
        string check = "False";
        if (stock.checkdata()) check = "True";
        ostr << "No Enough Daily Price Data: " << check << endl << endl; //"No Enough Daily Price Data: True"

        ostr << "Stock Daily Price Dates: " << endl;
        for (auto iter = stock.getTrades().begin(); iter != stock.getTrades().end(); iter++) { //trades在multi里得到
            ostr << setw(15) << iter->first; //getTrades()的first是所有date
        }
        ostr << endl << endl;

        ostr << "Stock Daily Price: " << endl;
        for (auto iter = stock.getTrades().begin(); iter != stock.getTrades().end(); iter++) {
            ostr << setw(15) << iter->second; //getTrades()的second是所有daily price
        }
        ostr << endl << endl;

        // calculate_Return();在calculate_AR里invoke了   然后calculate_Return()里已经计算过了daily_return和 cummu return
        ostr << "Stock Daily Returns: " << endl;
        ostr << stock.daily_return << endl; //stock.daily_return是private data

        ostr << "Stock Cumulative Daily Returns: " << endl;
        ostr << stock.cumulative_return << endl; 

        ostr << "Stock Abnormal Daily Returns: " << endl;
        ostr << stock.AR << endl; //AR计算在main里 此时已经存储好了

        return ostr;
    }
};




