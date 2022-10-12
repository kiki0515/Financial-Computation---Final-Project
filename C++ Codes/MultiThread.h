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

    void fetchtrading_multi(map<string, Stock*>& stockMap, const Benchmark& benchmark, const string url_common,  const string api_token)
    {
        int batch_size = (int)stockMap.size() / no_of_threads;
        std::cout << "The number of thread is " << no_of_threads << std::endl;
        std::cout << "The batch size is " << batch_size << std::endl << std::endl;
        std::cout << "Warning: here are stocks with less than 2N+1 price data: " << std::endl;

        vector<thread> threads;
        vector<map<string, Stock*>> sub_stockMap_list;
        vector<string> stockTickers; //record the stock symbols in the same order as stockMap
        for (auto iter = stockMap.begin(); iter != stockMap.end(); iter++) {
            string annouceday = iter->second->getAnnounceDay();
            iter->second->setStartDay(benchmark.PrevNDays(annouceday));
            iter->second->setEndDay(benchmark.NextNDays(annouceday));
            iter->second->setN(benchmark.GetN());
            stockTickers.emplace_back(iter->first);
        }

        for (int i = 0; i < no_of_threads; i++)
        {
            int start = i * batch_size;
            auto start_ = stockMap.find(stockTickers[start]);

            int end = (i + 1) * batch_size;
            auto end_ = stockMap.find(stockTickers[end]);
            if (i == no_of_threads - 1) {
                end_ = stockMap.end();
            }
            map<string, Stock*> stockMap_sliced(start_, end_);
            sub_stockMap_list.emplace_back(stockMap_sliced);
        }


        for (int i = 0; i < no_of_threads; i++)
        {
            threads.emplace_back(thread(fetchtrading, ref(sub_stockMap_list[i]), url_common, api_token));
            //std::cout << sub_stockMap_list[i].size() << std::endl;
        }

        for (thread& t : threads)
        {
            t.join();
        }

    }
};


// fetch trading data for all stock in one thread
int fetchtrading(map<string, Stock*>& stockMap, string url_common, string api_token) {
    for (auto iter = stockMap.begin(); iter != stockMap.end(); iter++) {

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

            string start_date = iter->second->getStartDay();
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
                for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); itr++)
                {
                    sDate = (*itr)["date"].asString();
                    dValue = (*itr)["adjusted_close"].asDouble();
                    iter->second->addTrade(make_pair(sDate, dValue));
                }
            }
            // warning for stocks with less than 2N+1 price data
            if ((int)iter->second->getTrades().size() != 2 * iter->second->getN() + 1) {
                cout << iter->first << '\t' << iter->second->getTrades().size() << endl;
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
