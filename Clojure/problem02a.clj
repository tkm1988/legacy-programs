;;Simple Perceptron Program
;;2012/01/13
;;T.Katanosaka
(use 'clojure.contrib.math)

(defn staircase-function [var]
  (if (> var 0)
    1
    0))

(defn sgn [var]
  (if (> var 0)
    1
    (if (= var 0)
      0
      -1)))

(defn learning-theta [lambda sig_t sig_s]
  (* lambda (* sig_t (* (staircase-function (* (- sig_t) sig_s))))))

(defn main [th_ast l0]
  (loop [t 0 th (rand)]
    (let [x (rand) l l0]
      (print t  "\t" th "\t" l "\t" (expt (- th_ast th) 2) "\n")
      (if (and (< (- th_ast th) 0.001) (> (- th_ast th) -0.0001))
        th
        (recur (inc t)
               (+ th (learning-theta l (sgn (- th_ast x)) (sgn (- th x)))))))))

(def theta 0.5)
(def lambda-zero 0.0001)

(main theta lambda-zero)