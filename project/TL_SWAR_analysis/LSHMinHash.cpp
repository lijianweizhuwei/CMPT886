//
//  LSHMinHash.cpp
//  LLVMDemangle
//
//  Created by chauncey on 2018/4/9.
//

#include "LSHMinHash.hpp"
#include "MinHash.hpp"
using namespace SWAR_TL;

// Calculate the signature
int* LSHMinHash::getSigs(int* object, int object_size){
    return mh->signature(object, object_size);
}

// Calculate the LSH hash
int* LSHMinHash::hash(int* sigs, int sig_size){
    return hashSignature(sigs, sig_size);
}
