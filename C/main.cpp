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
        
        A = (float*) malloc (xsize*ysize*sizeof(float)) ;
        B = (float*) malloc (xsize*ysize*sizeof(float)) ;
        
        if( A == NULL || B == NULL) {
            std::cerr << "Allocation failed\n";
            exit(1);
        }
        
        for( size_t y = 0; y < ysize; y++) {
            for( size_t x = 0; x < xsize; x++) {
                A[y*xsize+x] = 0;
                B[y*xsize+x] = 0;
            }
        }
        for( size_t x = 0; x < xsize; x++) {
            A[0+x] = 1;
            B[(ysize-1)*xsize+x] = 1;
        }
    }

    
    
    void jacobi() {
        size_t iter;
        size_t y;
        size_t x;
        float *C;

       
        for( iter=1; iter < iters; iter++) {
            for( y = 1; y < ysize-1; y++) {
                for( x = 1; x < xsize-1; x++) {
                    A[y*xsize+x] = ( B[(y-1)*xsize+x  ] +
                                     B[(y+1)*xsize+x  ] +
                                     B[(y  )*xsize+x-1] +
                                     B[(y  )*xsize+x+1] ) * 0.25f;
                }
            }
            C = A;
            A = B;
            B = C;
            //           std::swap(A,B);
        }

    }

        
    float* __restrict A;
    float* __restrict B;
};

int main(int argc, char** argv) {


    Plain test;

    test.init();
    double start = dtime();
    test.jacobi();
    double end = dtime();
    
    double flops = (xsize-2)*(ysize-2)*4.0*iters;

    std::cout << flops/(end-start)*1.0e-9 << "GFlop/s\n";

    return 1;

}
