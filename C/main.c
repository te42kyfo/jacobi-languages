#include <sys/time.h>

const static xsize = 64;
const static ysize = 32;

double dtime() {
	double tseconds = 0;
	struct timeval t;
	gettimeofday( &t, NULL);
	tseconds = (double) t.tv_sec + (double) t.tv_usec*1.0e-6;
	return tseconds;
}

class naive {
 public:
    init() {
        p1 = 
        
    jacobi();
    float* p1, p2;
};

int main(int argc, char** argv) {


    return 1;

}
