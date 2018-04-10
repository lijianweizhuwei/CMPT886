#include "Similarity.hpp"
#include <iostream>
using namespace std;
//private static Scanner sc;
char filePath[] = "/Users/eric/grad/cmpt886/project/data.txt";
char perfFilePath[] = "/Users/eric/grad/cmpt886/project/performance.txt";
//private BufferedReader reader;
int bufferSize = 5*1024*1024; // Read with buffer size 5M
int rowNumber = -1;
int colunmNumberWithType = -1;
int colunmNumber = -1;
int p = -1; // Number of permutations
int buckets = -1; // Number of buckets per band
int bands = -1; // Number of bands

double weightOfA = -1;
double weightOfB = -1;
double weightOfC = -1;

int SWAR = 0;
int TypeLegalization = 1;
int Unchanged = -1;

//int dataMatrixWithTypeContain3[][]; // The data matrix(transposed)
//double[][] dataMatrixWithTypeAndOptPerf; // The data matrix(transposed)
//int[][] dataMatrix; // The data matrix(transposed)
//double[][] perfData; // The performance data matrix(transposed)

int perfSize = 3;
//int[][] hashes; // The hash value matrix(transposed)
//int[][] sigs; // The signature matrix(transposed)
double optThreshold = 0.0;
double optTPRate = 0.0;
int simPairWithOptThreshold = 0;
int simPairWithOptThresholdAndSameType = 0;

char newInstrParas[100];

int main()
{
    cout << "Input the number of parameters：" << endl;
    cin >> colunmNumberWithType;
    colunmNumber = colunmNumberWithType - 1;
    cout << "Input the number of objects：" << endl;
    cin >> rowNumber;

    p = 100;
    bands = 25;
    buckets = 4;

    cout << "The weight of A：" << endl;
    cin >> weightOfA;
    cout << "The weight of B：" << endl;
    cin >> weightOfB;
    cout << "The weight of C：" << endl;
    cin >> weightOfC;
    
    new Similarity();
}
