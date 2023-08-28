import os
import sys

specification = int(sys.argv[1])
N = int(sys.argv[2])



for i in range(1, N + 1):
    tests = "tests_" + str(specification) + "/"
    answers = "answers_" + str(specification) + "/"
    if i < 10 :
        test_file = tests + "t0" + str(i) + ".txt"
        ans_file = answers + "a0" + str(i) + ".txt"
    else:
        test_file =  tests + "t" + str(i) + ".txt"
        ans_file =  answers + "a" + str(i) + ".txt"

    os.system("./a.out < " + test_file + " > answer.txt")
    print("test " , i)
    os.system("diff answer.txt " + ans_file + " -y --suppress-common-lines")

# os.system("rm answer.txt")
print("tests finished")

