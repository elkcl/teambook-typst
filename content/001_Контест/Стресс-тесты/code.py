import subprocess
import sys

_, f1, f2, gen, iters = sys.argv

for i in range(int(iters)):
    print('Test', i + 1)
    test = subprocess.run(["python", gen], encoding = "utf-8", capture_output = True).stdout
    v1 = subprocess.run(["./%s" % f1], input = test, encoding = "utf-8", capture_output = True).stdout
    v2 = subprocess.run(["./%s" % f2], input = test, encoding = "utf-8", capture_output = True).stdout
    if v1 != v2:
        print("FAIL!\nInput:")
        print(test)
        print("Correct output:")
        print(v1)
        print("Wrong output:")
        print(v2)
        sys.exit()
    print("No output differences found.")
