//
//  LSH.cpp
//  LLVMDemangle
//
//  Created by chauncey on 2018/4/9.
//
#include "LSH.hpp"
#include <algorithm>
using namespace SWAR_TL;
using namespace std;

int * LSH::hashSignature(int * signature, int sig_size){
    //signature is for each object
    int hash[bands];
    
    // Number of rows per stage
    int rows =  sig_size / bands;
    
    for (int i = 0; i < sig_size; i++) {
        int stage = min(i / rows, bands - 1);
        hash[stage] = (int)((hash[stage] + (long) signature[i] * LARGE_PRIME) % buckets);
    }
    return hash;
};
