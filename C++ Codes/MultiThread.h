//
//  MultiThread.h
//  Final6883
//


#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <thread>
#include "curl/curl.h"
#include "json/json.h"
#include "Benchmark.h"
#include "Stock.h"
#include "Util.h"
//using namespace std;

int fetchtrading(map<string, Stock*>& stockMap, string url_common, string api_token);

// class Multi_threading is used for multithreading fetching trading data for all stocks
class Multi_threading {
private:
    int no_of_threads;

public:
    Multi_threading() : no_of_threads(10) {}
    Multi_threading(int no_of_threads_) : no_of_threads(no_of_threads_) {}
    Multi_threading(const Multi_threading& mul) : no_of_threads(mul.no_of_threads) {}

    //mul.fetchtrading_multi(stockMap, iwv, url_common, api_token);
    //map<string, Stock*> stockMap; 
    void fetchtrading_multi(map<string, Stock*>& stockMap, const Benchmark& benchmark, const string url_common,  const string api_token)
    {
        //batch_size 每个线程分配到的股票数量
        int batch_size = (int)stockMap.size() / no_of_threads; //batch size越少跑越快 //how many data for one thread to deal with
        std::cout << "The number of thread is " << no_of_threads << std::endl;
        std::cout << "The batch size is " << batch_size << std::endl << std::endl; 
        std::cout << "Warning: here are stocks with less than 2N+1 price data: " << std::endl;

        //确定日期 -N N
        vector<thread> threads;
        vector<map<string, Stock*>> sub_stockMap_list;
        vector<string> stockTickers; //record the stock symbols in the same order as stockMap
        for (auto iter = stockMap.begin(); iter != stockMap.end(); iter++) {
            //map<string, Stock*> stockMap;
            // 给所有股票都set完了-N和N
            string annouceday = iter->second->getAnnounceDay(); // iter->second 是Stock* Stock是class 往里存数据 //string getAnnounceDay() const { return announce_date; }
            iter->second->setStartDay(benchmark.PrevNDays(annouceday)); //benchmark是iwv //benchmark.PrevNDays(annouceday) 是第-N天日期
            iter->second->setEndDay(benchmark.NextNDays(annouceday)); //benchmark.NextNDays(annouceday)是第N天日期
            iter->second->setN(benchmark.GetN()); // *Stock指向的数据 已经是有 N 
            stockTickers.emplace_back(iter->first); //iter->first 是 stockMap里的symbol/ticker
        }

        //????找分界行
        for (int i = 0; i < no_of_threads; i++)
        {
            int start = i * batch_size;  // 0
            auto start_ = stockMap.find(stockTickers[start]); //stockTickers存所有symbol //stockTickers[start]是股票名称

            //mymap['a']=50;
            //std::cout << "a => " << mymap.find('a')->second << '\n';
            //a => 50

            int end = (i + 1) * batch_size; // 10
            auto end_ = stockMap.find(stockTickers[end]); //stockMap.find(stockTickers[start])是一个iterator，指到那一行
            if (i == no_of_threads - 1) {
                end_ = stockMap.end();
            }

            //因为start_和end_已经是指向tockMap的pointer 所以直接把start到end这一段给了stockMap_sliced
            map<string, Stock*> stockMap_sliced(start_, end_); //start_是<string, Stock*> 
            sub_stockMap_list.emplace_back(stockMap_sliced); //vector<map<string, Stock*>> sub_stockMap_list;
            //sub_stockMap_list是n组分解行的start+end
        }

        // fetch trading data in one thread    
        for (int i = 0; i < no_of_threads; i++)
        {
            //vector<thread> threads;
            //fetchtrading是fetch一次： 从start到end
            //ref(sub_stockMap_list[i]), url_common, api_token)是fetchtrading的参数
            threads.emplace_back(thread(fetchtrading, ref(sub_stockMap_list[i]), url_common, api_token));
            //std::cout << sub_stockMap_list[i].size() << std::endl;
        }

        for (thread& t : threads) 
        {
            t.join();
            //To wait for a thread use the std::thread::join() function. This function makes the current thread wait until the thread identified by *this has finished executing.
            //For instance, to block the main thread until thread t1 has finished we would do

        }

    }  
};


// fetch trading data for all stock in one thread
//因为成员函数需要this指针，一般都是用独立函数跑线程的
int fetchtrading(map<string, Stock*>& stockMap, string url_common, string api_token) {
    for (auto iter = stockMap.begin(); iter != stockMap.end(); iter++) {  //iterator固定一个stock，一个stock应该有2N+1个数据
        //sub_stockMap_list[i]来确定stockMap.begin()和stockMap.end()
        /////////////////iter->secon就是 Stock*

        while (true)
        {
            string read_buffer; // read buffers for daily data

            // set up the program environment that libcurl needs
            curl_global_init(CURL_GLOBAL_ALL);
            CURL* handle;
            CURLcode result;
            handle = curl_easy_init();
            if (!handle)
            {
                cout << "curl_easy_init failed" << endl;
                return -1;
            }

            //前面所有股票setStartDay了 所以这里可以直接通过iter指向的股票getStartDay
            string start_date = iter->second->getStartDay(); //map<string, Stock*> stockMap, iter->second是Stock*是pointer，因为class里Startday是private，所以getStartDay()
            string end_date = iter->second->getEndDay();

            string url_request = url_common + iter->first + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d&fmt=json";
            // cout << url_request << endl;
            curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
            curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
            //curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, &read_buffer);

            // perform, then store the expected code in result
            result = curl_easy_perform(handle);

            if (result != CURLE_OK) {
                //cout << "Fetching failed. Retry for downloading..." << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                continue;
            }

            //json parsing
            Json::CharReaderBuilder builder;
            Json::CharReader* reader = builder.newCharReader();
            Json::Value root;   // will contains the root value after parsing.
            string errors;

            bool parsingSuccessful = reader->parse(read_buffer.c_str(), read_buffer.c_str() + read_buffer.size(), &root, &errors);
            if (not parsingSuccessful)
            {
                //cout << "Failed to parse JSON" << endl << read_buffer << errors << endl;
                //cout << "Failed to parse JSON:   Too many requests" << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            }
            else
            {
                // cout << "\nSuccess parsing json\n" << root << endl;
                string  sDate;
                double dValue;

                //8. ？？？？？？？？？怎么取到N
                for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); itr++)
                {
                    sDate = (*itr)["date"].asString();
                    dValue = (*itr)["adjusted_close"].asDouble();
                    iter->second->addTrade(make_pair(sDate, dValue)); //trades里每次都存一个stock
                    //iter->second是iter = stockMap.begin()
                    // trades里加pair 
                    // trades是： map<string, double> trades; // date, adj closed mapping
                    // void addTrade(const pair<string, double>& trade) { trades.insert(trade); }
                }
            }

            // warning for stocks with less than 2N+1 price data
            if ((int)iter->second->getTrades().size() != 2 * iter->second->getN() + 1) { //getTrades()就是return trades， trades是map //const map<string, double> & getTrades() const { return trades; }
                cout << iter->first << '\t' << iter->second->getTrades().size() << endl; //iter->first是symbol //iter->second是pointer，pointer指向function->getTrades()，再.size()，就是对这个stock取了多少天
            }

            // cleanup since you've used curl_easy_init
            curl_easy_cleanup(handle);
            // release resources acquired by curl_global_init()
            curl_global_cleanup();

            break;
        }
    }
    return 0;
}
