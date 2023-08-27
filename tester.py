import os

N = int(input())

for i in range(1, N + 1):
    if i < 10 :
        test_file = "tests_1/t0" + str(i) + ".txt"
        ans_file = "answers_1/a0" + str(i) + ".txt"
    else:
        test_file = "tests/t" + str(i) + ".txt"
        ans_file = "answers_1/a" + str(i) + ".txt"

    os.system("./a.out < " + test_file + " > answer.txt")
    print("test " , i)
    os.system("diff answer.txt " + ans_file + " -y --suppress-common-lines")

# os.system("rm answer.txt")
print("tests finished")
