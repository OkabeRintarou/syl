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

;练习1-29
(define (simpson-rule f a b n)
  (define beg a)
  (define h (/ (- b a) n))
  (define (sum term a next n)
    (if (> a n)
      0
      (+ (term a) (sum term (next a) next n))))
  (define (inc x) (+ x 1))
  (define (simpson-term a)
    (* (f (+ beg (* a h)))
       (cond ((or (= a 0) (= a n)) 1)
             ((= (remainder a 2) 1) 4)
             (else 2))))
  (* (/ h 3)
     (sum simpson-term a inc n)))
(define (test-cube x) (* x x x)) 
;(simpson-rule test-cube 0 1 100) --> 1/4
;(simpson-rule test-cube 0 1 1000) --> 1/4

(define (test-function x) (+ (* (/ 1.0 3) x x x) (* 2 x x) 9))  
;(simpson-rule test-function 0 1 100) --> 9.749999999999998
;(simpson-rule test-function 0 1 1000) --> 9.75

;练习1-30
(define (sum term a next b)
  (define (iter a result)
    (if (> a b)
        result
        (iter (next a) (+ (term a) result))))
  (iter a 0))

;练习1-31
(define (product term a next b)
  (define (iter a result)
    (if (> a b)
        result
        (iter (next a) (* (term a) result))))
  (iter a 1))
(define (compute-pi)
 #| ##########################################
π   2 * 4 * 4 * 6 * 6 * 8 ...
- = -   -   -   -   -   -
4   3 * 3 * 5 * 5 * 7 * 7 ...

 ############################################ |#
  (define beg 1)
  (define end 100000)
  (define (numerator-term a)
    (if (= (remainder a 2) 0)
        (+ a 2)
        (+ a 1)))
  (define (numerator-next a)
    (+ a 1))
  (define (denominator-term a) 
    (if (= (remainder a 2) 1)
       (+ a 2)
       (+ a 1)))
  (define (denominator-next a)
    (+ a 1))
  (* 4 (/ (product numerator-term beg numerator-next end)
          (product denominator-term beg denominator-next end))))

; 练习1-32
(define (accumulate-recursion combiner null-value term a next b)  ;递归
  (if (> a b)
      null-value
      (combiner (term a) (accumulate-recursion combiner null-value term (next a) next b))))
(define (accumulate-iteration combiner null-value term a next b)  ;迭代
  (define (accumulate-iteration-iter a result)
    (if (> a  b)
        result
        (accumulate-iteration-iter (next a) (combiner (term a) result))))
  (accumulate-iteration-iter a null-value))
        
(define (test-sum-recursion term a next b)
  (accumulate-recursion + 0 term a next b))
(define (test-sum-iteration term a next b)
  (accumulate-iteration + 0 term a next b))
(define (test-product-recursion term a next b)
  (accumulate-recursion * 1 term a next b))
(define (test-product-iteration term a next b)
  (accumulate-iteration * 1 term a next b))

;练习1-33
(define (filtered-accumulate filter combiner null-value term a next b)
  (define (filtered-accumulate-iter a result)
    (if (> a b)
        result
        (if (filter a)
            (filtered-accumulate-iter (next a) (combiner result (term a)))
            (filtered-accumulate-iter (next a) result))))
  (filtered-accumulate-iter a null-value))
(define (prime-sum a b)
  (define (term a) a)
  (define (next a) (+ a 1))
  (filtered-accumulate prime? + 0 term a next b))
(define (coprime-product n)
  (define (term a) a)
  (define (next a) (+ a  1))
  (define (coprime? i)
    (= (gcd i n) 1))
  (filtered-accumulate coprime? * 1 term 2 next n))

;练习1-35
(define (fixed-point f first-guess)
  (define tolerance 0.00001)
  (define (abs x) (if (< x 0) (- x) x))
  (define (close-enough? x y)
    (< (abs (- x y)) tolerance))
  (define (try guess)
    (let ((next (f guess)))
      (if (close-enough? guess next)
          next
          (try next))))
  (try first-guess))
(define (practice1-35)
  (fixed-point (lambda (x) (+ 1 (/ 1.0 x))) 1.0))

;练习1-36
(define (fixed-point-ex f first-guess)
  (define tolerance 0.00001)
  (define (abs x) (if (< x 0) (- x) x))
  (define (close-enough? x y)
    (< (abs (- x y)) tolerance))
  (define (try guess)
    (display guess)
    (newline)
    (let ((next (f guess)))
      (if (close-enough? guess next)
          next
          (try next))))
  (try first-guess))
(define (practice1-36-no) ;无阻尼
  (fixed-point-ex (lambda (x) (/ (log 1000) (log x))) 1.1))
(define (practice1-36-yes);有阻尼
  (fixed-point-ex (lambda (x) (* (/ 1 2) (+ (/ (log 1000) (log x)) x))) 1.1))

;练习1-37
(define (cont-frac-recursion n d k) ;递归
  (define (cont-frac-impl i )
    (if (= i k)
        (/ (n k) (d k))
        (/ (n i) (+ (d i) (cont-frac-impl (+ i 1) )))))
  (cont-frac-impl 1))
(define (cont-frac-iteration n d k) ;迭代
  (define (cont-frac-iter i result)
    (if (= i 0)
        result
        (cont-frac-iter (- i 1) (/ (n i) (+ result (d i))))))
  (cont-frac-iter k 0))

;练习1-38
(define (practice1-38-impl k)
  (define (D i)
    (cond ((= i 1) 1)
          ((= (remainder i 3) 2) (* 2 (/ (+ i 1)  3)))
          (else 1)))
  (cont-frac-iteration (lambda (i) 1.0) D k))
(define (practice1-38)
  (+ 2 practice1-38-impl 100)) ;  --> e≈2..7182818284590453

;练习1-39
(define (tan-cf x k)
  (define (d n) (- (* 2 n) 1))
  (define (n i) 
    (if (= i 1) 
        x
        (- (* x x))))
 (cont-frac-iteration n d k))

;练习1-40
(define dx 0.00001)
  (define (deriv g)
    (lambda (x)
      (/ (- (g (+ x dx)) (g x)) dx)))
(define (newton-transform g)
  (lambda (x)
    (- x (/ (g x) ((deriv g) x)))))

(define (newtons-method f guess) ;牛顿法
  (fixed-point (newton-transform f) guess))

(define (cubic a b c)
  (define (cube x) (* x x x))
  (define (square x) (* x x))
  (lambda (x)
    (+ (cube x) (* a (square x)) (* b x) c)))
(define (practice1-40 a b c)
  (newtons-method (cubic a b c) 1))

;练习1-41
(define (double f)
  (lambda (x)
    (f (f x))))
(define (inc x)
  (+ x 1))
(define (practice1-41)
  (((double (double double)) inc) 5))

;练习1-42
(define (practice1-42)
  (define (compose f g)
    (lambda (x)
      (f (g x))))
  (define (square x)
    (* x  x))
  ((compose square inc) 6))

;练习1-43
(define (repeated f n)
  (lambda (x)
    (if (= n 1)
        (f x)
        ((compose f (repeated f (- n 1))) x))))
        
((repeated (lambda (x) (* x x)) 2) 5)