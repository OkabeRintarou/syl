#lang racket
;练习1-3
(define (practice1-3 x y z)
  (if (> x y)
      (if (> y z) (+ x y) (+ x z))
      (if (> x z) (+ x y) (+ y z))))

;;;;
(define (sqrt x)
  (define (sqrt-iter guess)
    (if (good-enough? guess)
        guess
        ( sqrt-iter (improve guess))))
  (define (good-enough? guess)
    (< (abs (- (square guess) x)) 0.001))
   (define (improve guess)
    (average guess (/ x guess)))
  (define (abs x)
    (if (< x 0) (- x) x))
  (define (square x)
    (* x x))
  (define (average x y)
    (/ (+ x y) 2))
  (sqrt-iter 1.0)
  )

;练习1-8
(define (cub x)
  (define (cub-iter guess)
    (if (good-enough? guess)
        guess
        (cub-iter (improve guess))))
  (define (improve guess)
    (/ (+ (/ x (square guess)) (* 2 guess)) 3))
  (define (good-enough? guess)
    (< (abs (- (* guess guess guess) x)) 0.0001))
  (define (abs x)
    (if (< x 0) (- x) x))
  (define (square x)
    (* x x))
  (cub-iter 1.0))

(define (count-change amount)
  (define (cc amount kinds-of-coins)
    (cond ((= amount 0) 1)
          ((or (< amount 0) (= kinds-of-coins 0)) 0)
          (else (+ (cc amount (- kinds-of-coins 1))
                   (cc (- amount (first-denomination kinds-of-coins)) kinds-of-coins)))))
  (define (first-denomination kinds-of-coins)
    (cond ((= kinds-of-coins 1) 1)
          ((= kinds-of-coins 2) 5)
          ((= kinds-of-coins 3) 10)
          ((= kinds-of-coins 4) 25)
          ((= kinds-of-coins 5) 50)))
  (cc amount 5))

;练习1-11
(define (practice1-11-recursion n)
  (if (< n 3)
      n
      (+ (practice1-11-recursion (- n 1))
         (* (practice1-11-recursion (- n 2)) 2)
         (* (practice1-11-recursion (- n 3)) 3))))
(define (practice1-11-iteration n)
  (define (prac-iter a b c count)
            (if (= count 0)
                c
                (prac-iter (+ a (* 2 b) (* 3 c))a b (- count 1))))
  (prac-iter 2 1 0 n)
  )

;练习1-12
(define (practice1-12 n m)
  (cond ((= m 0) 1)
        ((= m n) 1)
        ((+ (practice1-12 (- n 1) (- m 1)) (practice1-12 (- n 1) m)))))

(define (sine angle)
  (define (cube x) (* x x x))
  (define (abs x) (if (< x 0) (- x) x))
  (define (p x) (- ( * 3 x) (* 4 (cube x))))
  (if (not (> (abs angle) 0.1))
      angle 
      (p (sine (/ angle 3.0))))
  )

;练习1-16
(define (practice1-16 b n) ;迭代
  (define (even? n)
    (= (remainder n 2) 0))
  (define (prac-iter a b n)
    (cond [(= n 0) a]
          [(even? n) (prac-iter a (* b b) (/ n 2))]
          [else (prac-iter (* a b) b (- n 1))]))
  (prac-iter 1 b n))


(define (fast-expt b n);递归
  (define (even? n)
    (= (remainder n 2) 0))
  (define (square x) (* x x))
  (cond ((= n 0) 1)
        ((even? n) (square (fast-expt b (/ n 2))))
        (else (* b (fast-expt  b (- n 1))))))

;练习1-17
(define (practice1-17 a b) ;递归
  (define (even? n)
    (= (remainder n 2) 0))
  (define (double x) (+ x x))
  (define (halve x) (/ x 2))
  (cond [(= b 0) 0]
        [(even? b) (double (practice1-17 a (halve b)))]
        [else (+ (practice1-17 a (- b 1)) a)])) 

;练习1-18
(define (practice1-18 a b);迭代
  (define (double x) (+ x x))
  (define (halve x) (if (even? x) (/ x 2) (/ (- x 1) 2)))
  (define (even? n)
    (= (remainder n 2) 0))
  (define (iter m a b)
    (cond [(= b 1) m]
          [(even? (halve b)) (iter m (double a) (halve b))]
          [else (iter (+ m (double a)) (double a) (halve b))]))
  (define m (if (even? b) 0 a))
  (iter m a b))

;练习1-19
(define (practice1-19 n)
  
  (define (even? n)
    (= (remainder n 2) 0))
  (define (fib-iter a b p q count)
    (cond ((= count 0) b)
          ((even? count)
           (fib-iter a
                     b
                     (+ (* p p) (* q q))
                     (+ (* 2 p q) (* q q))      
                     (/ count 2)))
          (else (fib-iter (+ (* b q) (* a q) (* a p))
                          (+ (* b p) (* a q))
                          p
                          q
                          (- count 1)))))
  (fib-iter 1 0 0 1 n));

;欧几里得算法
(define (gcd a b)
  (if (= b 0)
      a
      (gcd b (remainder a b))))

;√n 增长阶素数判断
(define (smallest-divisor n)
  (define (square x) (* x x))
  (define (divided? a b)
    (= (remainder b a) 0))
  (define (find-divisor n test-divisor)
    (cond ((> (square test-divisor) n ) n)
          ((divided? test-divisor n) test-divisor)
          (else (find-divisor n (+ test-divisor 1)))))
  (find-divisor n 2))
(define (prime? n)
  (= (smallest-divisor n) n))


