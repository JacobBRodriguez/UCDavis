(defun someNum(this)
(cons this (cons 2 (cons 3 (cons 4 nil))))
)



(defun modo(this)
(mod this 3)
)



(defun Make_list(x y)
(if (> x y)
	(+ 2 3) 
	(- 2 3 ))
)


(defun primeRib(list1 list2 x)
(if (= (length list1) 0) 
	(delete 1 list2)
	 
		(if (= (mod (car list1) x) 0)
			(primeRib (cdr list1) (delete (car list1) list2) x )
			(primeRib (cdr list1) list2 x ) ) 
	 
	)
)

(defun Bigger(list1 list2 x)
(if (= (length list1) 0)
	(delete 1 list2)
	(Bigger (cdr list1) (primeRib (cdr list1) list2 (car list1)) (car list1)) )
)

(defun list_maker(list x y)
(if (> x y)
	(list)
	( cons x (list_maker list (+ x 1) y)) )
)

(defun remove_list(list1 x)
(if (= (length list1) 3)
	(delete 1 list1)
	(remove_list (delete (car list1) list1) (+ 1 x)) ) )



(defun prime(primeNum)
(Bigger (list_maker '() 2 primeNum) (list_maker '() 2 primeNum) 2 ) 
)





