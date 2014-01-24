#include <sys/time.h>
#include <iostream>

const static size_t xsize = 64;
const static size_t ysize = 32;

double dtime() {
	double tseconds = 0;
	struct timeval t;
	gettimeofday( &t, NULL);
	tseconds = (double) t.tv_sec + (double) t.tv_usec*1.0e-6;
	return tseconds;
}

class Naive {
public:
    init() {
        p1 = new float[xsize*ysize];
        p2 = new float[xsize*ysize];
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

        

    jacobi() {
        for( size_t iter = 0; iter < iters; iter++) {
            for( size_t y = 1; y < ysize-1; y++) {
                for( size_t x = 1; x < xsize-1; x++) {
                    p1[y*xsize+x] = ( p1[y    *xsize+x+1] +
                                      p1[y    *xsize+x-1] +
                                      p1[(y+1)*xsize+x  ] +
                                      p1[(y-1)*xsize+x  ]) * 0.25f;

                }
            }
            std::swap(p1,p2);
        }
    }
        
    float* p1, p2;
};

int main(int argc, char** argv) {


    Naive test;

    test.init();
    double start = dtime();
    test.jacobi();
    double end = dtime();
    std::cout << end-start << "\n";

    return 1;

}
