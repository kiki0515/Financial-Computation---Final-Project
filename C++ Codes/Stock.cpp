//
//  Stock.cpp
//  Final6883
//


#include "Stock.h"

//No enough daily price data?
bool Stock::checkdata() const{
    //map<string, double> trades; 所有天日期和股票所有天数据 一天日期对应一个数据
    //输入哪个stock，check哪个stock
    if ((int)trades.size() < 2 * N + 1) return true;
    else return false;
}

//Which group this stock belong to?
//vector<map<string, Stock*>> 一个vector里有三个map。每个map是一个group（main里的）
string Stock::get_group(const vector<map<string, Stock*>> &StockMapVec){
    for (unsigned int i = 0; i < StockMapVec.size(); i++){ //StockMapVec.size()是3 //第几个group
        for ( auto itr = StockMapVec[i].begin(); itr != StockMapVec[i].end(); itr++){ //StockMapVec[i]是map
            if (sSymbol == (itr->first)){  //sSymbol在main里赋值 //itr->first是group map里的symbol
                if (i == 0) group = "Beat";
                else if (i == 1) group = "Miss";
                else group = "Meet";
                return group;
            }
        }
    }
    group = "Other stock";
    return group;
}


//Get Stock Daily Returns for one stock
void Stock::calculate_Return(){
    Vector daily_price; //创建Vector叫daily_price //只在这个function
    for (auto itr = trades.begin(); itr != trades.end(); itr++)
    {
        daily_price.pushback(itr->second);

    }
    daily_return = daily_price.pct_change();
    cumulative_return = daily_return.cumsum();
}



//Get Stock Abnormal Daily Returns = daily stock return - daily benchmark
Vector Stock::calculate_AR(const Benchmark& benchmark)
{
    // Find index of announce date 不是日期
    int day0_index = benchmark.DayFind(announce_date);

    // Get iwv prices of 2N+1
    Vector iwvPrice;
    map<int, PAIR> iwvPrices = benchmark.getIndexedTrade(); //iwvPrices是所有的两年的benchmark的数据 //map<int, PAIR> indexedTrades;
    for (int i = day0_index - N; i <= day0_index + N; i++) //挑出来2N+1的index
    {
        if (iwvPrices.find(i) != iwvPrices.end()) { //找到了i那行  //iwvPrices.find(i)是return *iter
            string iwv_date = iwvPrices[i].first; //i是key, iwvPrices[i]是pair,pair里第一个是日期
            double iwv_price = iwvPrices[i].second; //i是key, iwvPrices[i]是pair,pair里第二个是iwv的daily price
            
            if (trades.find(iwv_date) != trades.end()) { //？？？？check iwv里的日期
                iwvPrice.pushback(iwv_price);
            }
        }
    }
    Vector iwcReturn = iwvPrice.pct_change();

    // calculate stock daily return and cummulative raterun
    calculate_Return();

    //Vector AR;
    AR = daily_return - iwcReturn;
    return AR;
}
