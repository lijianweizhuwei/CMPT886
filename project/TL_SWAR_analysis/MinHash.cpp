#include "MinHash.hpp"
#include <math.h>
#include <limits>
#include <algorithm>
using namespace SWAR_TL;

//Compute the exact similarity index between two objects or signatures
double MinHash::similarityIndex(int* s1, int* s2, int s_size) {
    int elementNumber1 = s1[s_size - 2]; //number of element in this vector
    int elementSize1 = s1[s_size - 1]; //size of element in this vector
    int elementNumber2 = s2[s_size- 2];
    int elementSize2 = s2[s_size - 1];
    
    int widen_size = s_size + 2;
    int s3[widen_size];
    int s4[widen_size];
    
    for(int i = 0; i < widen_size; i++){
        if(i < s_size - 2)
            s3[i] = s1[i];
        else
            s3[i] = 1;
    }
    
    for(int i = 0; i < s_size - 2; i++){
        s4[i] = s2[i];
    }
    
    if( lastPowerOf2(elementNumber1) == lastPowerOf2(elementNumber2) )
        s4[s_size - 2] = 1;
    else
        s4[s_size - 2] = 0;
    
    if( lastPowerOf2(elementSize1) == lastPowerOf2(elementSize2) )
        s4[s_size - 1] = 1;
    else
        s4[s_size - 1] = 0;
    
    if( nextPowerOf2(elementNumber1) == nextPowerOf2(elementNumber2) ){
        s4[s_size] = 1;
    }else{
        s4[s_size] = 0;
    }
    
    if( nextPowerOf2(elementSize1) == nextPowerOf2(elementSize2) ){
        s4[s_size + 1] = 1;
    }else{
        s4[s_size + 1] = 0;
    }
    
    return jaccardIndex(s3, s4, widen_size);
}

int MinHash::lastPowerOf2(int num){
    return (floor(log(num)/log(2)));
}

int MinHash::nextPowerOf2(int num){
    return (ceil(log(num)/log(2)));
}

//Compute the exact jaccard index between two objects or signatures
double MinHash::jaccardIndex(int* s1, int* s2, int s_size) {
    int intersectionSize = 0, unionSize = 0;
    for (int i = 0; i < s_size; i++){
        if(s1[i] == s2[i]){
            if(s1[i] == 1){
                intersectionSize++;
            }else{
                unionSize++;
            }
        }
    }
    return double(intersectionSize) / double(s_size - (intersectionSize + unionSize));
}

//Signature size.
int sig_size;
//Object size is also the size of vectors if the sets are provided
int object_size;

int *sig = new int [sig_size];

//Initializes hash functions to compute MinHash signatures for objects.
//Computes the signature for this set.
int* MinHash::signature(int* object, int object_size) {
    for (int i = 0; i < sig_size; i++) {
        sig[i] = __INT_MAX__;
    }

    // Loop over true values, instead of loop over all values of objects to speedup computation
    for (int i = 0; i < object_size; i++) {
        int r = object[i];
        //If c has 1 in row r, then for each i, set SIG(i, c) to the smaller of the current value of SIG(i, c) and hi(r)
        for (int j = 0; j < sig_size; j++) {
            sig[j] = std::min(sig[j], h(j, r));
        }
    }

    return sig;
}

//Computes an estimation of Jaccard similarity (the number of elements incommon) between two signatures
double MinHash::similarity(int* sig1, int* sig2, int sig_size) {
    double sim = 0;
    for (int i = 0; i < sig_size; i++) {
        if (sig1[i] == sig2[i]) {
            sim += 1;
        }
    }
    return sim / sig_size;
}

//Random a and b coefficients for the random hash functions.
long ** hash_coefs;
//long hash_coefs[][2] = {};

//Compute hash function coefficients using provided Random.
void MinHash::init(int size, int obj_size) {
    
    this->object_size = obj_size;
    this->sig_size = size;
    
    // h = (a * x) + b, a and b should be randomly generated
    
//    hash_coefs = (long **) malloc(sizeof(long *) * sig_size);
//    hash_coefs = [sig_size][2];
    hash_coefs = (long **)malloc(sizeof(long *) * sig_size);
    for(int i = 0; i < sig_size; i++){
        *(hash_coefs + i) = (long *) malloc(sizeof(long) * 2);
    }
    srand((int)time(0));
    for (int i = 0; i < sig_size; i++) {
//        hash_coefs[i] = (long *) malloc(sizeof(long) * 2);
        hash_coefs[i][0] = rand() % object_size; // a
        hash_coefs[i][1] = rand() % object_size; // b
    }
}

// Computes hi(x) as (a_i * x + b_i) % objects_size.
int MinHash::h(int i, int x) {
    return (int)((hash_coefs[i][0] * (long) x + hash_coefs[i][1]) % object_size);
}
