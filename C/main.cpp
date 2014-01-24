#include <sys/time.h>
#include <iostream>
#include <stdlib.h>

const static size_t xsize = 64;
const static size_t ysize = 32;
const static int iters = 100000;

double dtime() {
	double tseconds = 0;
	struct timeval t;
	gettimeofday( &t, NULL);
	tseconds = (double) t.tv_sec + (double) t.tv_usec*1.0e-6;
	return tseconds;
}

class Naive {
public:
    void init() {
        if( posix_memalign ( (void**) &p1, xsize*ysize*sizeof(float), 32) != 0) {
            std::cout << "Allocation error";
            exit(1);
        }
        if( posix_memalign ( (void**) &p2, xsize*ysize*sizeof(float), 32) != 0) {
            std::cout << "Allocation error";
            exit(1);
        }
        for( size_t y = 0; y < ysize; y++) {
            for( size_t x = 0; x < xsize; x++) {
                p1[y*xsize+x] = 0;
                p2[y*xsize+x] = 0;
            }
        }
        for( size_t x = 0; x < xsize; x++) {
            p1[0+x] = 1;
            p2[(ysize-1)*xsize+x] = 1;
        }
    }

    
    
    void jacobi() {

        float * __restrict pp1 = (float*) __builtin_assume_aligned (p1, 16);
        float * __restrict pp2 = (float*) __builtin_assume_aligned (p2, 16);


        for( size_t iter = 0; iter < iters; iter++) {
            for( size_t y = 1; y < ysize-1; y++) {
                   for( size_t x = 1; x < xsize-1; x++) {
                       pp2[y*xsize+x] = ( pp1[(y)*xsize+x+1] +
                                          pp1[(y)*xsize+x-1] +
                                          pp1[(y-1)*xsize+x] +
                                          pp1[(y+1)*xsize+x])*0.25;
                                      
                }
            }
            std::swap(pp1,pp2);
        }
    }
        
    float* __restrict p1;
    float* __restrict p2;
};

int main(int argc, char** argv) {


    Naive test;

    test.init();
    double start = dtime();
    test.jacobi();
    double end = dtime();
    
    int flops = (xsize-2)*(ysize-2)*4*iters;

    std::cout << flops/(end-start)*1.0e-6 << "\n";

    return 1;

}
