(defn x-or [x1 x2]
  (let [net1 (and (= x1 true) (= x2 true)) net2 (not (or (= x1 true) (= x2 true)))]
    (print (not (or net1 net2)) "\n")))

(x-or true true)
(x-or true false)
(x-or false true)
(x-or false false)

