;; Multi-Layer Perceptron for X-OR
;; T.Katanosaka
;; 11/01/25
(use 'clojure.contrib.math)

(defn staircase-function [x th]
  (if (> x th)
    1
    0))

(defn sigmoid [a x]
  (/ 1 (- 1 (. Math (exp (- (* a x)))))))

(defn sigmoid [x]
  (/ 1 (- 1 (. Math (exp (- x))))))

(defn deff-sigmid [a x]
  (* (* a (sigmoid a x)) (- 1 (sigmoid a x))))
y
(defn x-or [x1 x2]
  (if (not (= x1 x2))
    1
    0))

(defn mlp [x1 x2 w01 w12 th01 th12]
  (starecase-function (sigmoid (- (+ (* (sigmoid (- (+ (* x1 (aget w01 0)) (* x2 (aget w01 1))) (aget ht01 0))) (aget w12 0))
                                     (* (sigmoid (- (+ (* x1 (aget w01 2)) (* x2 (aget w01 3))) (aget ht01 1))) (aget w12 1)))
                                  th12)) 0.5))
(defn renewal-w01 [x1 x2 w01 w12]
  )

(defn renewal-w12 [x1 x2 w01 w12]
  )

(defn renewal-th01 [x1 x2 w01 w12]
  )

(defn renewal-th12 [x1 x2 w01 w12]
  )

(loop [time 0 w01 (to-array '((rand) (rand) (rand) (rand))) w12 (to-array '((rand) (rand))) th01 (to-array '((rand) (rand))) th12 (rand)]
  (loop [epoch 0 error 0]
    (def x1 (staircase-function (rand) 0.5))
    (def x2 (staircase-function (rand) 0.5))
    (if (<= epoch 3)
      (recur (inc epoch) (+ error (expt (- (x-or x1 x2) (mlp )) 2)))
      (print time "\t" error "\n")))
  (if (> error 0)
    (recur (inc time) () () () ())))
  