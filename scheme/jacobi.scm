#!/usr/bin/guile -s
!#
(use-modules (srfi srfi-4))

(define (jacobi-iterate iterations)
  (let* ((xsize 32)
         (ysize 16)
         (size  (* xsize ysize))
         (data (make-f32vector size 0.0)))
    (do ((iter 0 (1+ iter))) ((= iter iterations))
      (do ((iy 1 (1+ iy))) ((= iy (1- ysize)))
        (do ((ix 1 (1+ ix))) ((= ix (1- xsize)))
          (let ((index (+ (* iy xsize) ix)))
            (f32vector-set! data index
                            (* 0.25 (+ (f32vector-ref data (1+ index))
                                       (f32vector-ref data (1- index))
                                       (f32vector-ref data (+  index xsize))
                                       (f32vector-ref data (-  index xsize)))))))))))
(jacobi-iterate (* 100 100))


