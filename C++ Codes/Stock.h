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
    // map<string, Stock*> stockMap; ÿһ��string/ticker ����������������
    
    // excel �������
    string sSymbol;
    string announce_date;
    string period_end;
    double estimated;
    double reported;
    double surprise;
    double surprise_pct;

    // multithread������ж�set��
    string start_day;
    string end_day;
    
    string group;
    map<string, double> trades; // 2N+1���date, 2N+1���adj closed mapping //��multithread�һ����Ʊ����-n��n��������Ѵ��� ����StockMap��second pointerָ��λ�õ�����
    
    int N;
    
    //information for part B
    ////ÿ����Ʊ������
    //Vector������vector<double> v;

    Vector daily_return��
    Vector cumulative_return; //������Ʊ��cumulative_return
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



    void addTrade(const pair<string, double>& trade) { trades.insert(trade); } //multithread - ������
    string getSymbol() const { return sSymbol; }
    const map<string, double> & getTrades() const { return trades; } //multithread - �ж�2n+1

    // StockMap��� stock�Ѿ�set�� Get Stock Class��private data
    string getAnnounceDay() const { return announce_date; } 
    string getPeriodEnd() const { return period_end; }
    double getEstimated() const { return estimated; }
    
    // Get excel������
    double getReported() const { return reported; }
    double getSurprise() const { return surprise; }
    double getSurprisePct() const { return surprise_pct; }
    
    // multithread��ȷ�� -N��N���ڵ� ��ÿ����Ʊ
    void setStartDay(string start_day_) { start_day = start_day_; } //benchmark�������� //fetchtrading_multi
    void setEndDay(string end_day_) { end_day = end_day_; }
    
    string getStartDay() const { return start_day; } // one thread��ȡ�����õ� //fetchtrading
    string getEndDay() const{ return end_day; }
    
    // multithread
    void setN(int n) { N = n; } //fetchtrading_multi
    int getN() const { return N; } //fetchtrading


    bool checkdata() const; //check data�ǲ���2N+1
    string get_group(const vector<map<string, Stock*>> &StockMapVec); //1��2��or3
    
    
    void calculate_Return(); // calculate stock daily return and cumulative return
    Vector calculate_AR(const Benchmark& benchmark); //calculate stock abnormal return

    Vector getReturn() const { return daily_return; }
    Vector getCumulativeReturn() const { return cumulative_return; }
    Vector getAR() const { return AR; }
    

    friend ostream& operator<<(ostream& ostr, const Stock& stock) { //�������һ��Stock class��object
        // main��ó�����excel����
        ostr << "Symbol: " << stock.sSymbol << endl;
        ostr << "Earning Announcement Date: " << stock.announce_date << endl;
        ostr << "Earning Period Ending: " << stock.period_end << endl;
        ostr << "Earning Estimate: " << stock.estimated << endl;
        ostr << "Reported Earning Per Share: " << stock.reported << endl;
        ostr << "Earning Suprise: " << stock.surprise << endl;
        ostr << "Earning Suprise Percentage: " << stock.surprise_pct << endl;
        ostr << "Group: " << stock.group << endl; //main�����
        
        string check = "False";
        if (stock.checkdata()) check = "True";
        ostr << "No Enough Daily Price Data: " << check << endl << endl; //"No Enough Daily Price Data: True"

        ostr << "Stock Daily Price Dates: " << endl;
        for (auto iter = stock.getTrades().begin(); iter != stock.getTrades().end(); iter++) { //trades��multi��õ�
            ostr << setw(15) << iter->first; //getTrades()��first������date
        }
        ostr << endl << endl;

        ostr << "Stock Daily Price: " << endl;
        for (auto iter = stock.getTrades().begin(); iter != stock.getTrades().end(); iter++) {
            ostr << setw(15) << iter->second; //getTrades()��second������daily price
        }
        ostr << endl << endl;

        // calculate_Return();��calculate_AR��invoke��   Ȼ��calculate_Return()���Ѿ��������daily_return�� cummu return
        ostr << "Stock Daily Returns: " << endl;
        ostr << stock.daily_return << endl; //stock.daily_return��private data

        ostr << "Stock Cumulative Daily Returns: " << endl;
        ostr << stock.cumulative_return << endl; 

        ostr << "Stock Abnormal Daily Returns: " << endl;
        ostr << stock.AR << endl; //AR������main�� ��ʱ�Ѿ��洢����

        return ostr;
    }
};




