# Ntests problem 
Ntests = 9
strForT1 = 1 $(Ntests)
strForT2 = 2 $(Ntests)

compile_t1: number_one.c clean 
	gcc number_one.c
test_t1: clean compile_t1 tests_1/ answers_1/
	python tester.py $(strForT1)

compile_t2: clean number_two.c clean tests_2/ answers_2/
	gcc number_two.c
test_t2: compile_t2
	python tester.py  $(strForT2)


clean:
	rm -rf *.out
	rm  -f answer.txt
	
