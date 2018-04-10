//
//  MinHash.hpp
//  LLVMDemangle
//
//  Created by chauncey on 2018/4/9.
//

#ifndef MinHash_hpp
#define MinHash_hpp

#include <stdio.h>
namespace SWAR_TL {
    class MinHash {
        public:
            MinHash(int size, int object_size) {
                init(size, object_size);
            }
            double similarityIndex(int * s1, int * s2, int s_size);
            double jaccardIndex(int * s1, int * s2, int s_size);
            int lastPowerOf2(int num);
            int nextPowerOf2(int num);
            int* signature(int* object, int object_size);
            double similarity(int* sig1, int* sig2, int sig_size);
            void init(int size, int object_size);
            int h(int i, int x);

        private:
        //Signature size.
            int sig_size;
        //Random a and b coefficients for the random hash functions.
            long ** hash_coefs;
        //Object size (is also the size of vectors if the sets are provided
            int object_size;
    };
}

#endif /* MinHash_hpp */
