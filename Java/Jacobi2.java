import java.io.*;

public class Jacobi2 {
 
    public static void main(String[] args) {
               

        int xsize = 1024;
        int ysize = 1024;
        int iters = 100;

        float[] A = new float[ysize*xsize];
        float[] B = new float[ysize*xsize];
        float[] C;
        for( int y = 0; y < ysize;y++) {
            for( int x = 0; x < xsize;x++) {
                A[y*xsize+x] = 0;
                B[y*xsize+x] = 0;
            }
        }

        for( int x = 0; x < xsize;x++) {
            A[x] = 1;
            A[(ysize-1)*xsize+x] = 1;
            B[x] = 1;
            B[(ysize-1)*xsize+x] = 1;
        }
        

        int iter;
        int y;
        int x;

        long startTime = System.nanoTime();    

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
        }
        long walltime = System.nanoTime() - startTime;
        double flops = (xsize-2)*(ysize-2)*iters*4.0 / 
            (walltime / 1.0e9) * 1.0e-6;
        System.out.println( flops + " MFlop/s" );
    }
}
 
