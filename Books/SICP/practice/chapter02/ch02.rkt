#lang racket


;练习2-2
(define (make-segment beg end)
  (cons beg end))
(define (start-segment segment)
  (car segment))
(define (end-segment segment)
  (cdr segment))
(define (make-point x y)
  (cons x y))
(define (x-point point)
  (car point))
(define (y-point point)
  (cdr point))
(define (midpoint-segment segment)
  (define (average x y)
    (/ (+ x y) 2))
  (let* ((beg (start-segment segment))
         (end (end-segment segment))
         (beg-x (x-point beg))
         (beg-y (y-point beg))
         (end-x (x-point end))
         (end-y (y-point end)))
    (make-point (average beg-x end-x)
                (average beg-y end-y))))
(define (print-point p)
  (newline)
  (display "(")
  (display (x-point p))
  (display ",")
  (display (y-point p))
  (display ")"))

;练习2-3

;练习2-4
(define (practice2-4)
  (define (cons x y)
    (lambda (m) (m x y)))
  (define (car z)
    (z (lambda (p q) p)))
  (define (cdr z)
    (z (lambda (p q) q)))
  (display "2-4"))

;练习2-5
(define (practice2-5)
  (define (cons a b)
    (* (expt 2 a)
       (expt 3 b)))
  (define (car c)
    (if (= 0 (remainder c 2))
        (+ 1 (car (/ c 2)))
        0))
  (define (cdr c)
    (if (= 0 (remainder c 3))
        (+ 1 (cdr (/ c 3)))
        0))
  (display (car (cons 3 2)))
  (display (cdr (cons 9 8))))
  
;练习2-6
(define one (lambda (f) (lambda (x) (f x))))
(define two (lambda (f) (lambda (x) (f (f x)))))


;练习2-17
(define (last-pair lst)
  (if (empty? lst)
      (error "list can't be empty!")
      (if (empty? (cdr lst))
          (car lst)
          (last-pair (cdr lst)))))

;练习2-18
(define (reverse lst)
  (define (iter remainder-list result)
    (if (empty? remainder-list)
        result
        (iter (cdr remainder-list) (cons (car remainder-list) result))))
  (iter lst '()))

;练习2-19
(define (cc amount coin-values)
  (cond ((= amount 0) 1)
        ((or (< amount 0) (no-more? coin-values)) 0)
        (else 
         (+ (cc amount
                (except-first-denomination coin-values))
            (cc (- amount (first-denomination coin-values)) 
                coin-values)))))
(define (first-denomination coin-values)
  (car coin-values))
(define (except-first-denomination coin-values)
  (cdr coin-values))
(define (no-more? coin-values)
  (empty? coin-values))

;练习2-20
(define (same-parity a . b)
 (filter (if (even? a)
             even?
             odd?)
         (cons a b)))
       
;练习2-21
(define (practive2-21)
  (define (square-list items)
    (if (null? items)
        empty
        (cons (* (car items) (car items)) (square-list (cdr items)))))
  (define (square-list-version-2 items)
    (map (lambda (x) (* x x)) items))
  (display (square-list (list 1 2 3 4 5)))
  (newline)
  (display (square-list-version-2 (list 1 2 3 4 5))))

;练习2-23
(define (practice2-23)
  (define (for-each proc items)
    (if (null? items)
        (void)
        (begin
          (proc (car items))
          (for-each proc (cdr items)))))
  (for-each (lambda (x) (newline) (display x)) 
            (list 57 321 88)))

;练习2-27
(define (deep-reverse items)
  (define (iter items result)
    (cond ((null? items) result)
          ((not (pair? (car items)))
           (iter (cdr items) (cons (car items) result)))
          (else
           (iter (cdr items) (cons (deep-reverse (car items)) result)))))
  (iter items '()))

(define x (list (list 1 2) (list 3 4)))

;练习2-28
(define (fringe items)
  (define (iter items result)
    (cond ((null? items) result)
          ((not (pair? (car items)))
           (iter (cdr items) (append result (list (car items)))))
          (else
           (iter (cdr items) (append result (iter (car items) '()))))))
  (iter items '()))

;练习2-29
(define (make-mobile left right)
  (list left right))
(define (make-branch length structure)
  (list length structure))
(define (left-branch mobile)
  (car mobile))
(define (right-branch mobile)
  (cadr mobile))
(define (branch-length branch)
  (car branch))
(define (branch-structure branch)
  (cadr branch))
(define (total-weight mobile)
  (+ (branch-weight (left-branch mobile))
     (branch-weight (right-branch mobile))))
(define (branch-weight branch)
  (
(define mobile (make-mobile (make-branch 10 25)
                            (make-branch 5 20)))
