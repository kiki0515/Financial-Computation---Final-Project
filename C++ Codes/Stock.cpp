//
//  Stock.cpp
//  Final6883
//


#include "Stock.h"

//No enough daily price data?
bool Stock::checkdata() const{
    if ((int)trades.size() < 2 * N + 1) return true;
    else return false;
}

//Which group this stock belong to?
string Stock::get_group(const vector<map<string, Stock*>> &StockMapVec){
    for (unsigned int i = 0; i < StockMapVec.size(); i++){
        for ( auto itr = StockMapVec[i].begin(); itr != StockMapVec[i].end(); itr++){
            if (sSymbol == (itr->first)){
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


//Get Stock Daily Returns
void Stock::calculate_Return(){
    Vector daily_price;
    for (auto itr = trades.begin(); itr != trades.end(); itr++)
    {
        daily_price.pushback(itr->second);

    }
    daily_return = daily_price.pct_change();
    cumulative_return = daily_return.cumsum();
}



//Get Stock Abnormal Daily Returns
Vector Stock::calculate_AR(const Benchmark& benchmark)
{
    // Find index of announce date
    int day0_index = benchmark.DayFind(announce_date);
    // Get iwv prices of surrounding 2N+1
    Vector iwvPrice;
    map<int, PAIR> iwvPrices = benchmark.getIndexedTrade();
    for (int i = day0_index - N; i <= day0_index + N; i++)
    {
        if (iwvPrices.find(i) != iwvPrices.end()) {
            string iwv_date = iwvPrices[i].first;
            double iwv_price = iwvPrices[i].second;
            if (trades.find(iwv_date) != trades.end()) {
                iwvPrice.pushback(iwv_price);
            }
        }
    }
    Vector iwvReturn = iwvPrice.pct_change();

    // calculate stock daily return
    calculate_Return();
//    Vector AR;
    AR = daily_return - iwvReturn;
    return AR;
}
