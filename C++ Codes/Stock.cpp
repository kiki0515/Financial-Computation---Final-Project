//
//  Stock.cpp
//  Final6883
//


#include "Stock.h"

//No enough daily price data?
bool Stock::checkdata() const{
    //map<string, double> trades; ���������ں͹�Ʊ���������� һ�����ڶ�Ӧһ������
    //�����ĸ�stock��check�ĸ�stock
    if ((int)trades.size() < 2 * N + 1) return true;
    else return false;
}

//Which group this stock belong to?
//vector<map<string, Stock*>> һ��vector��������map��ÿ��map��һ��group��main��ģ�
string Stock::get_group(const vector<map<string, Stock*>> &StockMapVec){
    for (unsigned int i = 0; i < StockMapVec.size(); i++){ //StockMapVec.size()��3 //�ڼ���group
        for ( auto itr = StockMapVec[i].begin(); itr != StockMapVec[i].end(); itr++){ //StockMapVec[i]��map
            if (sSymbol == (itr->first)){  //sSymbol��main�︳ֵ //itr->first��group map���symbol
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
    Vector daily_price; //����Vector��daily_price //ֻ�����function
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
    // Find index of announce date ��������
    int day0_index = benchmark.DayFind(announce_date);

    // Get iwv prices of 2N+1
    Vector iwvPrice;
    map<int, PAIR> iwvPrices = benchmark.getIndexedTrade(); //iwvPrices�����е������benchmark������ //map<int, PAIR> indexedTrades;
    for (int i = day0_index - N; i <= day0_index + N; i++) //������2N+1��index
    {
        if (iwvPrices.find(i) != iwvPrices.end()) { //�ҵ���i����  //iwvPrices.find(i)��return *iter
            string iwv_date = iwvPrices[i].first; //i��key, iwvPrices[i]��pair,pair���һ��������
            double iwv_price = iwvPrices[i].second; //i��key, iwvPrices[i]��pair,pair��ڶ�����iwv��daily price
            
            if (trades.find(iwv_date) != trades.end()) { //��������check iwv�������
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
