(defun matchString(word string2)
(if (string= (car word) (car string2))
	string2
	 (if (string= (car string2) nil) 
		'(jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjgof)
		( matchString word (cdr string2)) )  )
)


(defun matchWord(word string2 x y)
(if (and (= (+ x 1) (length word)) (string= (char word x) '*))
	'1
	(if (or (and (= x (length word)) (/= y (length string2) ) ) (and (/= x (length word)) (= y (length string2) ))) 
	'2
	(if (and (= x (length word)) (= y (length string2) ))
	'1
	(if (string= (char word x) (char string2 y))
		(matchWord word string2 (+ x 1) (+ y 1))
		 (if (string= (char word x) '*) 
			(if (string= (char word (+ x 1)) (char string2 y) )
				(matchWord word string2 (+ x 1) y)
				(matchWord word string2  x (+ y 1)) )
			'2
			 )
		)
	)
)) )

(defun removeKebab(string1 string2)
(if (and (string= (car string1) nil) (string/= (car string2) nil) )
	nil
	(if (and (string= (car string1) nil) (string= (car string2) nil))
		t	
		(if (string= (car string1) (car string2))
			(removeKebab (cdr string1) (cdr string2))
		
			(if (string= (car string1) '!)
				(removeKebab (cdr string1) (matchString (cdr string1) string2) )
				
				(if (= (matchWord (string (car string1)) (string (car string2)) '0 '0) '1)
					(removeKebab (cdr string1) (cdr string2))
					nil )
						
		)
	)
)
) )

(defun match(string1 string2)
(removeKebab string1 string2)
)


(defun this(x)
(princ x)
(princ (+ x 1))
(princ (+ x 1)) )












