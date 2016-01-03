#lang racket

(define (make-monitored func)
 (define count 0)
 (define (dispatch cmd)
  (cond ((eq? cmd 'how-many-call?) count)
		((eq? cmd 'reset-count) (set! count 0))
		(else (begin 
			   (set! count (+ count 1))
			   (func cmd)))))
 dispatch)

(define s (make-monitored sqrt))

(s 100)
(s 'how-many-call?)
(s 200)
(s 'how-many-call?)
(s 'reset-count)
(s 'how-many-call?)
