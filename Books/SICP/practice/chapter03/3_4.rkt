#lang racket

(define (make-account balance secret)
 (define input_sec '())
 (define incorrect_count 0)

 (define (call-the-cops) (print "dial 911..."))

 (define (withdraw amount)
   (if (not (eq? input_sec secret))
	   (begin "Incorrect password"
		(set! incorrect_count (+ incorrect_count 1))
		(if (= 7 incorrect_count)
		    (begin (call-the-cops)
			       (set! incorrect_count 0)
			)
			empty
		)
	   )
       (if (>= balance amount)
	       (begin (set! balance (- balance amount))
		           balance)
		   "Insufficient funds"
	   )
   )
 )

 (define (deposite amount)
   (if (not (eq? input_sec secret))
	   "Incorrect password"
	   (begin (set! balance (+ balance amount))
		      balance)
	   ))

 (define (dispatch sec cmd)
   (set! input_sec sec)
   (cond ((eq? cmd 'withdraw) withdraw)
         ((eq? cmd 'deposite) deposite)
		 (else (error "Unknow request -- MAKE-ACCOUNT"
			   cmd))))
 dispatch)

(define acc (make-account 100 'secret-password))
((acc 'secret-password 'withdraw) 40)
((acc 'some-other-password 'withdraw) 50)
((acc 'some-other-password 'withdraw) 50)
((acc 'some-other-password 'withdraw) 50)
((acc 'some-other-password 'withdraw) 50)
((acc 'some-other-password 'withdraw) 50)
((acc 'some-other-password 'withdraw) 50)
((acc 'some-other-password 'withdraw) 50)
