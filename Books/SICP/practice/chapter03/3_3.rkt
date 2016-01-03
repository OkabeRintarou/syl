#lang racket

(define (make-account balance secret)
 (define input_sec '())

 (define (withdraw amount)
   (if (not (eq? input_sec secret))
	   "Incorrect password"
       (if (>= balance amount)
	       (begin (set! balance (- balance amount))
		           balance)
		   "Insufficient funds"
	   )
   ))

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
((acc 'some-other-password 'deposite) 50)
((acc 'secret-password 'withdraw) 20)
((acc 'secret-password 'deposite) 100)
