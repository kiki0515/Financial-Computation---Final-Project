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
    string sSymbol;
    string group;
    
    string announce_date;
    string period_end;
    double estimated;
    double reported;
    double surprise;
    double surprise_pct;
    string start_day;
    string end_day;
    
    map<string, double> trades; // date, adj closed mapping
    
    int N;
    
    //information for part B
    Vector daily_return;
    Vector cumulative_return;
    Vector AR;



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

    void addTrade(const pair<string, double>& trade) { trades.insert(trade); }
    string getSymbol() const { return sSymbol; }
    const map<string, double> & getTrades() const { return trades; }

    string getAnnounceDay() const { return announce_date; }
    string getPeriodEnd() const { return period_end; }
    double getEstimated() const { return estimated; }
    
    double getReported() const { return reported; }
    double getSurprise() const { return surprise; }
    double getSurprisePct() const { return surprise_pct; }
    
    void setStartDay(string start_day_) { start_day = start_day_; }
    void setEndDay(string end_day_) { end_day = end_day_; }
    
    string getStartDay() const { return start_day; }
    string getEndDay() const{ return end_day; }
    
    void setN(int n) { N = n; }
    int getN() const { return N; }

    Vector getReturn() const { return daily_return; }
    Vector getCumulativeReturn() const { return cumulative_return; }
    Vector getAR() const { return AR; }
    

    bool checkdata() const;
    string get_group(const vector<map<string, Stock*>> &StockMapVec);
    
    
    void calculate_Return(); // calculate stock daily return and cumulative return
    Vector calculate_AR(const Benchmark& benchmark); //calculate stock abnormal return
    

    friend ostream& operator<<(ostream& ostr, const Stock& stock) {
        ostr << "Symbol: " << stock.sSymbol << endl;
        ostr << "Earning Announcement Date: " << stock.announce_date << endl;
        ostr << "Earning Period Ending: " << stock.period_end << endl;
        ostr << "Earning Estimate: " << stock.estimated << endl;
        ostr << "Reported Earning Per Share: " << stock.reported << endl;
        ostr << "Earning Suprise: " << stock.surprise << endl;
        ostr << "Earning Suprise Percentage: " << stock.surprise_pct << endl;
        ostr << "Group: " << stock.group << endl;
        string check = "False";
        if (stock.checkdata()) check = "True";
        ostr << "No Enough Daily Price Data: " << check << endl << endl;

        ostr << "Stock Daily Price Dates: " << endl;
        for (auto iter = stock.getTrades().begin(); iter != stock.getTrades().end(); iter++) {
            ostr << setw(15) << iter->first;
        }
        ostr << endl << endl;

        ostr << "Stock Daily Price: " << endl;
        for (auto iter = stock.getTrades().begin(); iter != stock.getTrades().end(); iter++) {
            ostr << setw(15) << iter->second;
        }
        ostr << endl << endl;

        ostr << "Stock Daily Returns: " << endl;
        ostr << stock.daily_return << endl;

        ostr << "Stock Cumulative Daily Returns: " << endl;
        ostr << stock.cumulative_return << endl;

        ostr << "Stock Abnormal Daily Returns: " << endl;
        ostr << stock.AR << endl;

        return ostr;
    }
};




