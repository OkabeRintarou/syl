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

