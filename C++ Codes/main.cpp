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
    ifstream fin("Russell3000EarningsAnnouncements.csv");
    string line;

    //containers:
    map<string, Stock*> stockMap;
    map<string, Stock*> beatMap;
    map<string, Stock*> missMap;
    map<string, Stock*> meetMap;
    vector<PAIR> stockSurp;
    vector<vector<Vector>> result;
    Benchmark iwv("IWV");

    bool bCompleted = false;
    char selection;

    getline(fin, line); // ignore the first line (titles)
    while (getline(fin, line))
    {
        istringstream sin(line);
        string symbol, announce_date, period_end, estimated, reported, surprise, surprise_pct;
        getline(sin, symbol, ',');
        getline(sin, announce_date, ',');
        getline(sin, period_end, ',');
        getline(sin, estimated, ',');
        getline(sin, reported, ',');
        getline(sin, surprise, ',');
        getline(sin, surprise_pct, ',');

        Stock* stock = new Stock(symbol, announce_date, period_end, stod(estimated), stod(reported), stod(surprise), stod(surprise_pct));
        stockMap[symbol] = stock; // insert the stock pointer into stockMap
        stockSurp.emplace_back(PAIR(symbol, stod(surprise_pct))); //insert pair of ticker and surprise% into stockSurp

    }


    sort(stockSurp.begin(), stockSurp.end(), cmp_by_value);  //sort the tickers in ascending order according to surprise%
    int miss_size = (int)stockSurp.size() / 3;
    int meet_size = miss_size;
    int beat_size = (int)stockSurp.size() - miss_size - meet_size;
    //cout<<stockSurp.size()<<endl;
    for ( int i = 0; i < int(stockSurp.size()); i++) {
        if (i < miss_size) {
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
    while (!bCompleted)
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
        if (!(cin >> selection))
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

        switch (selection)
        {
        case'a':
        case'A':
                
        {
            int n;
            cout << "Please enter the number of days retriving backwards and forwards (60 - 90): " << endl;
            if (!(cin >> n) || n < 60 || n > 90)
            {
                cout << "Invalid Input, Please enter a positive integer >= 60 and <= 90" << endl;
                cin.clear();
                while (cin.get() != '\n')
                    // cin.get() is used for accessing character array. It includes white space characters. Generally, cin with an extraction operator (>>) terminates when whitespace is found. However, cin.get() reads a string with the whitespace.
                    continue;
                // continue the while loop in the beginning
                continue;
            }
            iwv.SetN(n);
            
            clock_t start, end;
            start = clock();

            // declaration of an object CURL
            CURL* handle;

            CURLcode result;

            // set up the program environment that libcurl needs
            curl_global_init(CURL_GLOBAL_ALL);

            // curl_easy_init() returns a CURL easy handle
            handle = curl_easy_init();

            if (handle)
            {
                string read_buffer;
                //populate benchmark IWV first!!!!
                cout << "Start downloading IWV price data..." << endl << endl;
                string start_date = "2020-06-15";
                string end_date = "2022-05-10";

                string url_request = url_common + "IWV" + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d&fmt=json";
                // cout << url_request << endl;

                curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
                curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, &read_buffer);

                // perform, then store the expected code in result
                result = curl_easy_perform(handle);


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
                        iwv.addIndexedTrade(make_pair(count, make_pair(sDate, dValue)));
                    }
                }

                cout << "Successfully downloaded!" << endl << endl;
                //cout << iwv << endl;

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


            //populate all stocks
            cout << "Start downloading stock price data..." << endl << endl;
            Multi_threading mul(num_thread);
            mul.fetchtrading_multi(stockMap, iwv, url_common, api_token);



            cout << "Successfully downloaded!" << endl;
            end = clock();
            cout << "time taken : " << (double)(end - start) / CLOCKS_PER_SEC << " s" << endl << endl;

            break;
        }

            case'b':
            case'B':
            {
                //Create Vector for different stock group map
                vector<map< string, Stock* >> StockMapVec(3);
                StockMapVec[0] = beatMap;
                StockMapVec[1] = missMap;
                StockMapVec[2] = meetMap;
                
                string Symbol;
                cout << "Enter Symbol for Detailed Information: ";
                cin >> Symbol;   //User enter symbol

                transform(Symbol.begin(), Symbol.end(), Symbol.begin(), ::toupper); //Make sure Symbol are all UPPERCASE
                map<string, Stock*>::iterator itr = stockMap.find(Symbol);
                if (itr != stockMap.end()){
                    itr->second->get_group(StockMapVec);
                    itr->second->calculate_AR(iwv);
                    cout << *(itr->second) << endl;
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
                    Bootstrap bootstrap(mapvector);
                    //cout << "456" << endl;
                    mapvector.clear();
                    cout << "Start Bootstrapping... " << endl;
                    result = bootstrap.calculateResultMatrix(80, 40, iwv);
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

                    for (int i = -iwv.GetN() + 1; i <= iwv.GetN(); i++) {
                        xData.push_back(i);
                    }

                    CAAR_1 = result[0][1].Getv(); //beat
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


