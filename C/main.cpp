#include <sys/time.h>
#include <iostream>
#include <stdlib.h>

const static size_t xsize = 32;
const static size_t ysize = 16;
const static int iters = 300000;

double dtime() {
	double tseconds = 0;
	struct timeval t;
	gettimeofday( &t, NULL);
	tseconds = (double) t.tv_sec + (double) t.tv_usec*1.0e-6;
	return tseconds;
}

class Aligned {
public:
    void init() {
        
        p1 = (float*) malloc (xsize*ysize*sizeof(float)+16) ;
        p2 = (float*) malloc (xsize*ysize*sizeof(float)+16) ;
        p1 += (( 16 - (size_t) p1%16 ) - 4)/4;
        p2 += (( 16 - (size_t) p2%16 ) - 4)/4;

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
        for( size_t iter = 0; iter < iters; iter++) {
            float* above = (float*) __builtin_assume_aligned
                (p1 + 0*xsize + 1, 16);
            float* center = (float*) __builtin_assume_aligned
                (p1 + 1*xsize +1, 16);
            float* below = (float*) __builtin_assume_aligned
                (p1 + 2*xsize+1, 16);
            float* dst = (float*) __builtin_assume_aligned
                (p2 + 1*xsize+1, 16);


            for( size_t y = 1; y < ysize-1; y++) {
                for( size_t x = 0; x < xsize-2; x++) {
                    dst[x] = ( center[x-1]+
                               center[x+1] +
                               above[x] +
                               below[x])*0.25;
                }
                above += xsize;
                center += xsize;
                below += xsize;
                dst += xsize;
            }
            std::swap(p1,p2);
        }
    }

        
    float* p1;
    float* p2;
};


class Plain {
public:
    void init() {
        
        p1 = (float*) malloc (xsize*ysize*sizeof(float)) ;
        p2 = (float*) malloc (xsize*ysize*sizeof(float)) ;
        
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
        for( size_t iter = 0; iter < iters; iter++) {
            for( size_t y = 1; y < ysize-1; y++) {
                for( size_t x = 0; x < xsize-1; x++) {
                    p1[y*xsize+x] = ( p2[(y+1)*xsize+x  ] +
                                      p2[(y-1)*xsize+x  ] +
                                      p2[(y  )*xsize+x+1] +
                                      p2[(y  )*xsize+x-1] ) * 0.25;
                    
                }
            }
            std::swap(p1,p2);
        }
    }

        
    float* p1;
    float* p2;
};

int main(int argc, char** argv) {


    Aligned test;

    test.init();
    double start = dtime();
    test.jacobi();
    double end = dtime();
    
    double flops = (xsize-2)*(ysize-2)*4.0*iters;

    std::cout << flops/(end-start)*1.0e-9 << "GFlop/s\n";

    return 1;

}
