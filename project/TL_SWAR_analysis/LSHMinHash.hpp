#ifndef LSHMinHash_hpp
#define LSHMinHash_hpp

#include <stdio.h>
#include "LSH.hpp"
#include "MinHash.hpp"

namespace SWAR_TL {
    class LSHMinHash : public LSH{
        private:
            MinHash * mh;
        public:
            LSHMinHash(int bands, int buckets, int object_size, int permutationNumber):LSH(bands, buckets){
                mh = new MinHash(permutationNumber, object_size);
            }
            int* getSigs(int* object, int object_size);
            int* hash(int* sigs, int sig_size);
    };
}

#endif /* LSHMinHash_hpp */
