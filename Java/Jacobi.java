import java.io.*;

public class Jacobi {
 
    public static void main(String[] args) {
               

        int xsize = 256;
        int ysize = 128;
        int iters = 1000000;

        float[][] A = new float[ysize][xsize];
        float[][] B = new float[ysize][xsize];
        float[][] C;
        for( int y = 0; y < ysize;y++) {
            for( int x = 0; x < xsize;x++) {
                A[y][x] = 0;
                B[y][x] = 0;
            }
        }

        for( int x = 0; x < xsize;x++) {
            A[0][x] = 1;
            A[ysize-1][x] = 1;
            B[0][x] = 1;
            B[ysize-1][x] = 1;
        }


        long startTime = System.nanoTime();    
        for(int iter=1; iter < iters; iter++) {
            for( int y = 1; y < ysize-1; y++) {
                for( int x = 1; x < xsize-1; x++) {
                    A[y][x] = ( B[y-1][x] +
                                B[y+1][x] +
                                B[y][x-1] +
                                B[y][x+1]) * 0.25f;
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
 
