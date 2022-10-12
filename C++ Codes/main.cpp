//
//  main.cpp
//  Final6883

#include "Stock.h"
#include "Util.h"
#include "Benchmark.h"
#include "MultiThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <ostream>
#include <locale>
#include <chrono>
#include "curl/curl.h"
#include <ctime>
#include "SampleCalcul.h"
#include "Bootstrap.h"
#include "GnuPlot_PC.h"

using namespace std;


int main() {
    //read stock information
    // fin is name of this file
    ifstream fin("Russell3000EarningsAnnouncements.csv");
    string line;

    //containers:
    // string is ticker, Stock is data
    // Stock* - Stock is a class - address
    // 全部从小到大
    map<string, Stock*> stockMap; 
    map<string, Stock*> beatMap;
    map<string, Stock*> missMap;
    map<string, Stock*> meetMap;

    //stockSurp：pair of ticker and surprise%
    vector<PAIR> stockSurp;

    vector<vector<Vector>> result;
    Benchmark iwv("IWV");  //Benchmark(string iSymbol_) : iSymbol(iSymbol_), N(0) {}

    bool bCompleted = false;
    char selection;

    getline(fin, line); // ignore the first line (titles)
    while (getline(fin, line))
    {
        //std::istringstream sin is a string class object which is used to stream the string into different variables and similarly files can be stream into strings. 
        //Objects of this class use a string buffer that contains a sequence of characters. This sequence of characters can be accessed as a string object.
        istringstream sin(line);
        string symbol, announce_date, period_end, estimated, reported, surprise, surprise_pct;
        //istream& getline (istream& is, string& str, char delim); delim分隔符
        getline(sin, symbol, ','); // before ， is symbol
        getline(sin, announce_date, ',');
        getline(sin, period_end, ',');
        getline(sin, estimated, ',');
        getline(sin, reported, ',');
        getline(sin, surprise, ',');
        getline(sin, surprise_pct, ',');

        // stock is an address
        // stod: string to double

        /// 1. 为什么用dynamic？
        // 不然 for循环结束后，这个会delete
 
        Stock* stock = new Stock(symbol, announce_date, period_end, stod(estimated), stod(reported), stod(surprise), stod(surprise_pct));
        // map<string, Stock*> stockMap;
        // symbol is ticker, access data of a ticker through symbol
        stockMap[symbol] = stock; // insert the stock pointer into stockMap ////excel里所有的数据
        // vector<PAIR> stockSurp;
        stockSurp.emplace_back(PAIR(symbol, stod(surprise_pct))); //insert pair of ticker and surprise% into stockSurp

    }

    //sort the tickers in ascending order according to surprise%
    //bool cmp_by_value(const PAIR& lhs, const PAIR& rhs) { return lhs.second < rhs.second;}
    //void sort (RandomAccessIterator first, RandomAccessIterator last, Compare comp);
    //comp:
    //Binary function that accepts two elements in the range as arguments, and returns a value convertible to bool. 
    //The value returned indicates whether the element passed as first argument is considered to go before the second in the specific strict weak ordering it defines.
    sort(stockSurp.begin(), stockSurp.end(), cmp_by_value); // bool cmp_by_value
    // after  ascending order sorting - 从小到大
    int miss_size = (int)stockSurp.size() / 3;
    int meet_size = miss_size;
    int beat_size = (int)stockSurp.size() - miss_size - meet_size;
    //cout<<stockSurp.size()<<endl;
    for ( int i = 0; i < int(stockSurp.size()); i++) {
        if (i < miss_size) {
            // i - vector stockSurp ith pair
            // stockSurp[i].first - symbol is ticker - key value
            // map<string, Stock*> missMap; symbol/ticker is the string
            // 把stockMap[stockSurp[i].first]的value/address赋给了missMap
            missMap[stockSurp[i].first] = stockMap[stockSurp[i].first];
        }
        else if (i < miss_size + meet_size) {
            meetMap[stockSurp[i].first] = stockMap[stockSurp[i].first];
        }
        else {
            beatMap[stockSurp[i].first] = stockMap[stockSurp[i].first];
        }
    }


    //parameters:
    int num_thread = 16;
    string url_common = "https://eodhistoricaldata.com/api/eod/";
    string api_token = "61f9f0dea8ca28.37045932";  


    //menu
    //bool bCompleted = false;
    while (!bCompleted)  //!bCompleted = TRUE
    {
        cout << endl;
        cout << "Menu" << endl;
        cout << "========" << endl;
        cout << "A - Retrieve Historical Price Data for All Stocks" << endl;
        cout << "B - Pull Information for One Stock" << endl;
        cout << "C - Show AAR, AAR-STD, CAAR and CAAR-STD" << endl;
        cout << "D - Show the Gnuplot Graph with CAAR" << endl;
        cout << "X - Exit" << endl << endl;

        cout << "Enter selection: ";

        /// 2. 没有用 输入1也是 "Invalid Input“
        if (!(cin >> selection)) //char selection;  //!(cin >> selection) 意思是 不是char
        {
            cout << "Invalid Input, Please Reenter" << endl;
            // if an error occurs when taking the input from cin. 
            // If an error occurs then an error flag is set and future attempts to get input will fail. 
            // That's why you need cin.clear(); to get rid of the error flag.
            cin.clear(); 

            //？？？？和!(cin >> selection)有啥区别

            // the input which failed will be sitting in what I assume is some sort of buffer. 
            // When you try to get input again, it will read the same input in the buffer and it will fail again. 
            // That's why you need cin.ignore(10000, '\n');
            // It takes out 10000 characters from the buffer but stops if it encounters a newline(\n).The 10000 is just a generic large value.
            // 一直读 直到读到'\n' 跳到新的一行 再跳出while loop
            while (cin.get() != '\n') //'\n'is a character constant.
                // cin.get() is used for accessing character array. It includes white space characters. Generally, cin with an extraction operator (>>) terminates when whitespace is found. 
                // However, cin.get() reads a string with the whitespace.
                continue;
            // continue the while loop in the beginning
            continue;
        }
        cout << endl << endl;


        switch (selection)
        {
        case'a':
        case'A':
                
        {
            int n;
            cout << "Please enter the number of days retriving backwards and forwards (60 - 90): " << endl;
            if (!(cin >> n) || n < 60 || n > 90) //!(cin >> n) 意思是 不是数字
            {
                cout << "Invalid Input, Please enter a positive integer >= 60 and <= 90" << endl;
                cin.clear();
                while (cin.get() != '\n')
                    // cin.get() is used for accessing character array. It includes white space characters. Generally, cin with an extraction operator (>>) terminates when whitespace is found. However, cin.get() reads a string with the whitespace.
                    continue;
                // continue the while loop in the beginning
                
                
                //4.？？？？？？
                continue; //!bCompleted = TRUE //重新开始menu
            }
            iwv.SetN(n); //Benchmark iwv("IWV");
            
            clock_t start, end; //计时
            start = clock();

            // declaration of an object CURL
            CURL* handle;

            CURLcode result;

            // set up the program environment that libcurl needs
            curl_global_init(CURL_GLOBAL_ALL);

            // curl_easy_init() returns a CURL easy handle
            handle = curl_easy_init();

            //populate benchmark IWV first!!!!
            if (handle)
            {
                string read_buffer;
                //populate benchmark IWV first!!!!
                cout << "Start downloading IWV price data..." << endl << endl;
                //结合string url_common，去写entire URL
                //"https://eodhistoricaldata.com/api/eod/aple.us?from=2020-06-15&to=2022-05-10&api_token=61f9f0dea8ca28.37045932&period=d"
                string start_date = "2020-06-15";
                string end_date = "2022-05-10";

                //网址
                string url_request = url_common + "IWV" + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d&fmt=json";
                // cout << url_request << endl;

                curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());//change c++ string to c char array to handle
                curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0); // no SSL
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0); // no authentification
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, &read_buffer); //call WriteCallback function write to the location of read_buffer

                // perform, then store the expected code in result
                result = curl_easy_perform(handle); //CURLcode result;


                if (result != CURLE_OK)
                {
                    // if errors have occured, tell us what is wrong with result
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                    return -1;
                }

                //json parsing
                Json::CharReaderBuilder builder;
                Json::CharReader* reader = builder.newCharReader();
                Json::Value root;   // will contains the root value after parsing.
                string errors;

                bool parsingSuccessful = reader->parse(read_buffer.c_str(), read_buffer.c_str() + read_buffer.size(), &root, &errors);
                if (not parsingSuccessful)
                {
                    // Report failures and their locations in the document.
                    cout << "Failed to parse JSON" << endl << read_buffer << errors << endl;
                    return -1;
                }
                else
                {
                    // cout << "\nSuccess parsing json\n" << root << endl;
                    // cout << "Success parsing json for " << stock.getSymbol() << endl;
                    string  sDate;
                    double dValue;
                    int count = 0;
                    for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); itr++)
                    {
                        count++;
                        sDate = (*itr)["date"].asString();
                        dValue = (*itr)["adjusted_close"].asDouble();
                        
                        // add pair of <int, PAIR>& indexedTrade 进入 map indexedTrades
                        //Benchmark iwv("IWV");
                        //map<int, PAIR> indexedTrades;  
                        //void addIndexedTrade(const pair<int, PAIR>& indexedTrade) { indexedTrades.insert(indexedTrade); }
                        iwv.addIndexedTrade(make_pair(count, make_pair(sDate, dValue))); //pair<int, PAIR>  //从1开始
                    }
                }

                cout << "Successfully downloaded!" << endl << endl;
                cout << iwv << endl;

            }
            else
            {
                fprintf(stderr, "Curl init failed!\n");
                return -1;
            }

            // cleanup since you've used curl_easy_init
            curl_easy_cleanup(handle);

            // release resources acquired by curl_global_init()
            curl_global_cleanup();


            //populate all stocks data
            cout << "Start downloading stock price data..." << endl << endl;
            Multi_threading mul(num_thread); //Multi_threading class
            mul.fetchtrading_multi(stockMap, iwv, url_common, api_token);
            //void fetchtrading_multi(map<string, Stock*>& stockMap, const Benchmark& benchmark, const string url_common,  const string api_token)



            cout << "Successfully downloaded!" << endl;
            end = clock();
            cout << "time taken : " << (double)(end - start) / CLOCKS_PER_SEC << " s" << endl << endl;

            break;
        }

            case'b':
            case'B':
            {
                //Create Vector for different stock group map
                vector<map< string, Stock* >> StockMapVec(3); //3个vector
                StockMapVec[0] = beatMap; 
                StockMapVec[1] = missMap;
                StockMapVec[2] = meetMap;
                
                string Symbol;
                cout << "Enter Symbol for Detailed Information: ";
                cin >> Symbol;   //User enter symbol

                transform(Symbol.begin(), Symbol.end(), Symbol.begin(), ::toupper); //Make sure Symbol are all UPPERCASE
                map<string, Stock*>::iterator itr = stockMap.find(Symbol); //stockMap里找Symbol itr是iterator指向stockMap里那一行
                if (itr != stockMap.end()){ //代表找到了
                    itr->second->get_group(StockMapVec); //itr->second是pointer //vector<map< string, Stock* >> StockMapVec
                    itr->second->calculate_AR(iwv);
                    //itr->second是pointer，*(itr->second)是dereference后的普通的object
                    cout << *(itr->second) << endl; //overload cout // *(itr->second)是 stock的object，dereference一下
                } 
                else{
                    cout << "This is not a valid Stock symbol." << endl; // Cannot find this symbol
                }

                break;
            }

                case'c':
                case'C':
                {
                    vector<map< string, Stock* >> mapvector(3);
                    mapvector[0] = beatMap;
                    mapvector[1] = missMap;
                    mapvector[2] = meetMap;
                    //cout << "123" << endl;
                    Bootstrap bootstrap(mapvector); //输入mapvector
                    //cout << "456" << endl;
                    mapvector.clear();
                    cout << "Start Bootstrapping... " << endl;
                    result = bootstrap.calculateResultMatrix(80, 40, iwv); //3*4*2N 2N-Vector 
                    cout << "Finished!" << endl << endl;

                    bool cCompleted = false;
                    int group;

                    while (!cCompleted)
                    {
                        cout << "Please select a group:  0 - Beat, 1 - Miss, 2 - Meet, 3 - Exit" << endl;
                        if (!(cin >> group) || group > 3 || group < 0)
                        {
                            cout << "Invalid Input, Please Reenter" << endl;
                            cin.clear();
                            while (cin.get() != '\n')
                                // cin.get() is used for accessing character array. It includes white space characters. Generally, cin with an extraction operator (>>) terminates when whitespace is found. However, cin.get() reads a string with the whitespace.
                                continue;
                            // continue the while loop in the beginning
                            continue;
                        }
                        cout << endl << endl;

                        switch (group)
                        {
                        case 2:
                        case 1:
                        case 0:
                        {
                            string g;
                            if (group == 2) g = "Meet";
                            else if (group == 1) g = "Miss";
                            else g = "Beat";

                            cout << "---------------------------------------------------------------------------------------------------" << endl;
                            cout << "Group:" << '\t' << g << endl;
                            cout << setw(15) << "Ave AAR" << setw(15) << "Ave CAAR" << setw(15) << "AAR std" << setw(15) << "CAAR std" << endl;
                            cout << "---------------------------------------------------------------------------------------------------" << endl;
                            cout << result[group] << endl;
                            break;
                        }

                        case 3: 
                        {
                            cCompleted = true;
                            cout << "Exit the loop." << endl;
                            break;
                        }

                        }

                    }

                    break;
                }

                case'd':
                case'D':
                {
                    vector<double> xData, CAAR_1, CAAR_2, CAAR_3;

                    //算x
                    for (int i = -iwv.GetN() + 1; i <= iwv.GetN(); i++) { // -59 - 60
                        xData.push_back(i); 
                    }

                    CAAR_1 = result[0][1].Getv(); //beat  //result[0]是第一个group，result[0][1] CAAR - Vector
                    CAAR_2 = result[1][1].Getv(); //miss  
                    CAAR_3 = result[2][1].Getv(); //meet
                    plot_CAAR(xData, CAAR_1, CAAR_2, CAAR_3);
                    
                    break;
                }

                case'x':
                case'X':
                {
                    bCompleted = true;
                    // Free memory
                    for (auto itr = stockMap.begin(); itr != stockMap.end(); itr++) {
                        delete itr->second;
                        itr->second = nullptr;
                    }
                    cout << "Exit the loop." << endl;
                    break;
                }

                default:
                {
                    cout << "Invalid Input" << endl;
                    break;
                }
                }
            }
            

            return 0;

        }


