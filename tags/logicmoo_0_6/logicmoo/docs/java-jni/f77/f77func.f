	function func1 ()
	implicit none
	integer func1, retval
c
	retval = 101
	write (*,*) 'FUNC1: returning ', retval
	func1 = retval
	return
	end

	function func2 (str, i, f)
	implicit none
	integer func2, retval, i
	character*(*) str
	real f
c
	retval = 102
	write (*,*) 'FUNC2: str = ', str, ' i = ', i, ' f = ', f 
	write (*,*) 'FUNC2: returning ', retval
	func2 = retval
	return
	end

