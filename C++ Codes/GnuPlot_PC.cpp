#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GnuPlot_PC.h"
#include <vector>
#include <iostream>
using namespace std;


void plot_CAAR(vector<double> xData, vector<double> yData1, vector<double> yData2, vector<double> yData3) {
    FILE* gnuplotPipe, * tempDataFile;
    const char* tempDataFileName1 = "Beat_Group";
    const char* tempDataFileName2 = "Miss_Group";
    const char* tempDataFileName3 = "Meet_Group";
    double x, y, x2, y2, x3, y3;
    int i;
    int dataSize = (int)xData.size();

    gnuplotPipe = _popen("D:\\gnuplot\\bin\\gnuplot.exe", "w");
    if (gnuplotPipe) {

        fprintf(gnuplotPipe, "set term wxt\n");
        fprintf(gnuplotPipe, "set grid\n");
        fprintf(gnuplotPipe, "set key left bottom\n");
        fprintf(gnuplotPipe, "set key font 'Sans, 5'\n");
        fprintf(gnuplotPipe, "set xlabel 'days'\n");
        fprintf(gnuplotPipe, "set xtics font 'Sans, 5'\n");
        fprintf(gnuplotPipe, "set xlabel font 'Sans, 7'\n");
        fprintf(gnuplotPipe, "set ylabel 'Averaged CAAR'\n");
        fprintf(gnuplotPipe, "set ytics font 'Sans, 5'\n");
        fprintf(gnuplotPipe, "set ylabel font 'Sans, 7'\n");
        fprintf(gnuplotPipe, "set title 'Averaged CAAR of All Three groups'\n");
        fprintf(gnuplotPipe, "set title font 'Sans, 10'\n");
        fprintf(gnuplotPipe, "plot \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n", tempDataFileName1, tempDataFileName2, tempDataFileName3);
        fflush(gnuplotPipe);

        tempDataFile = fopen(tempDataFileName1, "w");
        for (i = 0; i < dataSize; i++) {
            x = xData[i];
            y = yData1[i];
            fprintf(tempDataFile, "%lf %lf\n", x, y);
        }
        fclose(tempDataFile);


        tempDataFile = fopen(tempDataFileName2, "w");
        for (i = 0; i < dataSize; i++) {
            x2 = xData[i];
            y2 = yData2[i];
            fprintf(tempDataFile, "%lf %lf\n", x2, y2);
        }
        fclose(tempDataFile);

        tempDataFile = fopen(tempDataFileName3, "w");
        for (i = 0; i < dataSize; i++) {
            x3 = xData[i];
            y3 = yData3[i];
            fprintf(tempDataFile, "%lf %lf\n", x3, y3);
        }
        fclose(tempDataFile);

        printf("press enter to continue...");
        getchar(); //this one get the enter key remaining in the buffer!!!
        getchar();

        remove(tempDataFileName1);
        remove(tempDataFileName2);
        remove(tempDataFileName3);
        //fprintf(gnuplotPipe, "exit\n");
        _pclose(gnuplotPipe);
    }

    else {
        printf("gnuplot not found...");
    }
}