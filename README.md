# Financial Computation-Final Project

## Topic：Evaluate the impact of quarterly earnings report on stock price movement

In this project, our team investigated the relationship between Russell 3000 stocks’ earning surprise percentage and their CAAR (Cumulative Average Abnormal Return) based on IWV benchmark before and after the earning announcement date. We divided all Russell 3000 stocks equally into 3 groups (beat, meet and miss) according to their earning surprise percentage and implemented bootstrapping 40 times with batch size 80 to all 3 groups. We found that in general, stocks with higher surprise percentage would have higher CAAR after earning announcement date.


### Project Design Flow Chart
![image](https://user-images.githubusercontent.com/89110858/195433029-a5084cb5-c278-435b-acca-0c11675c8826.png)
![image](https://user-images.githubusercontent.com/89110858/195433318-7fa877de-abf0-4777-827d-80761e75db14.png)


### Project Tasks:
Task 1: Use liburl to retrieve historical price data from eodhistoricaldata.com: A function 
retrieves the adjusted close prices for selected Russell 3000 stocks and IWV
(Russell 3000 ETF used as market benchmark) into memory.

This program is able to:
o Retrieve historical price data for all selected stocks. Parse the retrieved data for dates and adjusted closing prices.
o Based on the 2nd quarter of 2021 earnings announcement for Russell 3000 stocks (See the Earnings Announcements sheet), sort all the surprise% in ascending order, and split all the stocks into 3 groups with relatively equivalent numbers of stocks:
    i. Highest surprise group: Beat Estimate Group
    ii. Lowest surprise group: Miss Estimate Group
    iii. The rest stocks in between: Meet Estimate Group
o Calculate AAR, AAR-STD, CAAR, CAAR-STD for each group by implementing bootstrapping
o Populate the stock maps and AAR/CAAR matrix
o Show the gnuplot graph with CAAR for all 3 groups

### Conclusion & Analysis
Three groups are sorted by their earnings surprise of 2021 Q2. Earnings reveal the financial health and economic conditions of businesses. An earnings surprise occurs when a company's reported quarterly or annual profits are above or below analysts' expectations. When a company's profit performance fails to match the expectations set by the investment community, investors often express their disappointment by selling shares.
Our group concluded that: earning releases of Russell 3000 stocks have a huge impact on their future stock prices, and stocks with higher surprise will have higher CAAR after earning announcement date. This conclusion can be explained in three aspects from graphs CAAR and AAR as following table:
![image](https://user-images.githubusercontent.com/89110858/195433506-49b7fcff-822c-4fcb-8282-665e7ada9d57.png)
![image](https://user-images.githubusercontent.com/89110858/195433568-02bca508-95a9-4259-ae78-bb01fff60fd7.png)


![image](https://user-images.githubusercontent.com/89110858/195433530-c53a3b36-9aed-40be-8638-df15a035bba3.png)


