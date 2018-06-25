(defun getItem(row num)
(nth num row)
)

(defun replaceItem(row num x)
(cond
	( (= x 1) (cons num (cons (getItem row 1) (cons (getItem row 2) (cons (getItem row 3) nil)))) )
	( (= x 2) (cons (getItem row 0) (cons num (cons (getItem row 2) (cons (getItem row 3) nil)))) )
	( (= x 3) (cons (getItem row 0) (cons (getItem row 1) (cons num (cons (getItem row 3) nil)))) )
	( (= x 4) (cons (getItem row 0) (cons (getItem row 1) (cons (getItem row 2) (cons num nil)))) )
	(t nil) )
)

(defun getRow(list)
(cdr list) )

(defun checkRow(list num)
(cond
	( (= (getItem list 0) num) '1)
	( (= (getItem list 1) num) '1)
	( (= (getItem list 2) num) '1)
	( (= (getItem list 3) num) '1)
	(t '0) )
)

(defun checkColumn(list num c)
(cond
	( (= (getItem (getItem list '0) c) num) '1)
	( (= (getItem (getItem list '1) c) num) '1)
	( (= (getItem (getItem list '2) c) num) '1)
	( (= (getItem (getItem list '3) c) num) '1)
	(t '0) )
)

(defun checkSquare(list num r c)
(cond
	( (and (or (= r '1) (= r '2)) (or (= c '1) (= c '2))) (cond
									( (= (getItem (getItem list '0) '0) num) '1)
									( (= (getItem (getItem list '0) '1) num) '1)
									( (= (getItem (getItem list '1) '0) num) '1)
									( (= (getItem (getItem list '1) '1) num) '1)
									(t '0) ) )
	( (and (or (= r '1) (= r '2)) (or (= c '3) (= c '4))) (cond
									( (= (getItem (getItem list '0) '2) num) '1)
									( (= (getItem (getItem list '0) '3) num) '1)
									( (= (getItem (getItem list '1) '2) num) '1)
									( (= (getItem (getItem list '1) '3) num) '1)
									(t '0) ) )
	( (and (or (= r '3) (= r '4)) (or (= c '1) (= c '2))) (cond
									( (= (getItem (getItem list '2) '0) num) '1)
									( (= (getItem (getItem list '2) '1) num) '1)
									( (= (getItem (getItem list '3) '0) num) '1)
									( (= (getItem (getItem list '3) '1) num) '1)
									(t '0) ) )
	( (and (or (= r '3) (= r '4)) (or (= c '3) (= c '4))) (cond
									( (= (getItem (getItem list '2) '2) num) '1)
									( (= (getItem (getItem list '2) '3) num) '1)
									( (= (getItem (getItem list '3) '2) num) '1)
									( (= (getItem (getItem list '3) '3) num) '1)
									(t '0) ) )
	(t '5) )
)




(defun seduk(list num r c)
(if (= num 5)
	(cons '3 list)
	(if (= r '5)
	(cons '2 list)
	(if (= c '5)
		(seduk list '1 (+ r 1) '1)
		(if (/= (getItem (getItem list (- r 1)) (- c 1)) '0)
			(seduk list '1 r (+ c 1))
			(cond
				( (= (checkRow (getItem list (- r 1)) num) '1) (seduk list (+ num 1) r c) )
				( (= (checkColumn list num (- c 1) ) '1) (seduk list (+ num 1) r c) )
				( (= (checkSquare list num r c) '1) (seduk list (+ num 1) r c) )
				(t (seduk (replaceItem list (replaceItem (getItem list (- r 1)) num c) r) '1 r (+ c 1)) ) )
		)
	)
) )
)

(defun sudoku-solver(list)
(if (= (car (seduk list '1 '1 '1) )'3)
	(cons nil list)
	(cons t (cons (cdr (seduk list '1 '1 '1) ) '() )) )
)

(princ (sudoku-solver '((0 0 5 0) (0 0 2 0) ( 0 0 0 0) (0 0 0 0) ) ))











