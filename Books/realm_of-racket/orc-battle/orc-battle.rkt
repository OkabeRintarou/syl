#lang racket
(require 2htdp/universe 2htdp/image)

; struct
(struct orc-world (player lom attack# target) #:transparent #:mutable)
(struct player (health agility strength) #:transparent #:mutable)

(struct monster (image [health #:mutable]) #:transparent)
(struct orc monster (club) #:transparent)         ;逆戟鲸
(struct hydra monster () #:transparent)           ;九头蛇
(struct slime monster (sliminess) #:transparent)  ;史莱姆
(struct brigand monster () #:transparent)         ;盗贼


; constant variables
(define MAX-HEALTH  35)
(define MAX-AGILITY 35)
(define MAX-STRENGTH 35)

(define ORC-IMAGE     (bitmap "graphics/orc.png"))
(define HYDRA-IMAGE   (bitmap "graphics/hydra.png"))
(define SLIME-IMAGE   (bitmap "graphics/slime.bmp"))
(define BRIGAND-IMAGE (bitmap "graphics/brigand.bmp"))
(define V-SPACER (rectangle 0 10 "solid" "white"))
(define H-SPACER (rectangle 10 0 "solid" "white"))

(define PER-ROW 4)
(define MONSTER#  16)
(define MONSTER-HEALTH0  25)
(define CLUB-STRENGTH 10)
(define SLIMESS  2)

; initialize world
(define (initialize-orc-world)
  (define player0 (initialize-player))
  (define lom0 (initialize-monsters))
  (orc-world player0 lom0 (random-number-of-attacks player0) 0))

(define (initialize-player)
  (player (MAX-HEALTH MAX-AGILITY MAX-STRENGTH)))
(define (initialize-monsters)
  (build-list
   MONSTER#
   (lambda (_)
     (define health (random+ MONSTER-HEALTH0))
     (case (random 4)
       [(0) (orc ORC-IMAGE health (random+ CLUB-STRENGTH))]
       [(1) (hydra HYDRA-IMAGE health)]
       [(2) (slime SLIME-IMAGE health (random+ SLIMESS))]
       [(3) (brigand BRIGAND-IMAGE health)]))))

(define (random-number-of-attacks p)
  (random-quotient (player-agility p) ATTACKS#))
(define (random-quotient x y)
  (define div (quotient x y))
  (if (> 0 div) 0 (random+ (add1 div))))
(define (random+ n)
  (add1 (random n)))
; on keydown
(define (player-acts-on-monsters w k)
  (cond 
    [(zero? (orc-world-attack# w)) (void)]
    [(key=? "s" k) (stab w)]
    [(key=? "h" k) (heal w)]
    [(key=? "f" k) (flail w)]
    [(key=? "e" k) (end-turn w)]
    [(key=? "n" k) (initialize-orc-world)]
    [(key=? "right" k) (move-target w +1)]
    [(key=? "left" k) (move-target w -1)]
    [(key=? "down" k) (move-target w (+ PER-ROW))]
    [(key=? "up" k) (move-target w (- PER-ROW))])
  (give-monster-turn-if-attack#=0 w)
  w)
; render
(define (render-orc-battle w)
  (render-orc-world w (orc-world-target w) (instructions w)))

(define (render-the-end w)
  (render-orc-world w #f (message (if (lose? w) LOSE WIN))))

(define (render-orc-world w t additional-text)
  (define i-player (render-payer (orc-world-player w)))
  (define i-monster (render-monsters (ore-world-monsters w)))
  (above V-SPACER
         (beside H-SPACER
                 i-player
                 H-SPACER H-SPACER H-SPACER
                 (above i-monster
                        V-SPACER V-SPACER V-SPACER
                        additional-text)
                 H-SPACER)
         V-SPACER))

(define (render-player (initialize-player))
  
                  

(define (instructions w)
  (define na (number->string (orc-world-attack# w)))
  (define ra (string-append REMAINING na))
  (define txt (text ra INSTRUCTION-TEXT-SIZE ATTACK-COLOR))
  (above txt INSTRUCTION-TEXT))
(define (message str)
  (text str MESSAGE-SIZE MESSAGE-COLOR))
; end the world
(define (end-of-orc-world w) 
  (or (win? w) (lose? w)))

; main
(define (start)
  (big-bang (initialize-orc-world)
            (on-key player-acts-on-monsters)
            (to-draw render-orc-battle)
            (stop-when end-of-orc-battle? render-the-end)))
