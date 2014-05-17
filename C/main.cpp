#include <sys/time.h>
#include <iostream>
#include <stdlib.h>
#include "iacaMarks.h"


const static size_t xsize = 66;
const static size_t ysize = 54;
const static int iters = 4000000;

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

        //p1 = (float*) malloc ( (xsize*ysize*sizeof(float)+16)*2 ) ;
		//        p2 = (float*) malloc (xsize*ysize*sizeof(float)+16) ;
		// p1 += (( 16 - (size_t) p1%16 ) - 4)/4;
        //p2 = p1 + xsize*ysize*sizeof(float);
		//p2 += (( 16 - (size_t) p2%16 ) - 4)/4;

		xstride = (xsize/16+1)*16;
		size_t buffer_size = xstride*ysize*sizeof(float);

		p1 = (float*) aligned_alloc( 4096, buffer_size*2);
		p2 = p1 + buffer_size;

        for( size_t i = 0; i < buffer_size/sizeof(float)*2; i++) {
			p1[i] = 0.5;
        }

    }


    inline void jacobi() {

		float* src = (float*) __builtin_assume_aligned
			(p1 + xstride, 64);

		float* dst = (float*) __builtin_assume_aligned
			(p2 + xstride, 64);

		//IACA_START;
        for( size_t iter = 0; iter < iters; iter++) {
            for( size_t y = 1; y < ysize-1; y++) {



				float* src_line = (float*) __builtin_assume_aligned( src + y*xstride, 64);
				float* src_above = (float*) __builtin_assume_aligned( src + (y+1)*xstride, 64);
				float* src_below = (float*) __builtin_assume_aligned( src + (y-1) *xstride, 64);
				float* dst_line = (float*) __builtin_assume_aligned( dst + y*xstride, 64);


                for( size_t x = 0; x < xsize-2; x++) {
                    dst_line[x] = ( src_above[x] +
									src_line[x+8] +
									src_line[x-8] +
									src_below[x])*0.25f;
                }


            }
			//IACA_END;

        }

    }

	size_t xstride;

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


        for( iter=0; iter < iters; iter++) {
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


    Aligned test;

    test.init();
    double start = dtime();
    test.jacobi();
    double end = dtime();

    double flops = (xsize-2)*(ysize-2)*1.0*iters;

    std::cout << flops/(end-start)*4.0e-9 << "GFlop/s\n";
	std::cout << (end-start)*2.5e9/ (flops/8) << "\n";

    return 0;

}
