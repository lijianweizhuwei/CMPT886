#ifndef LSH_hpp
#define LSH_hpp

#include <stdio.h>
namespace SWAR_TL {
    
    class LSH {
    private:
        const long LARGE_PRIME =  433494437;
        const int DEFAULT_BANDS = 3;
        const int DEFAULT_BUCKETS = 10;
        int bands = DEFAULT_BANDS;
        int buckets = DEFAULT_BUCKETS;
        
    public:
        //Instantiates a LSH instance with s  bands and b buckets (per band)
        LSH(int stages, int buckets){
            this->bands = stages;
            this->buckets = buckets;
        };
        int * hashSignature(int* signature, int sig_size);
       
    };
    
} // end llvm namespace

#endif /* LSH_hpp */



