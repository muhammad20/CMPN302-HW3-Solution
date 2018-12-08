import os

input_data_file = "input_p1.in"
output_data_file = "output_p1.out"
running_time_file = "p1.time"
student_output_file = "my_output_p1.out"

print("Compiling source code...")
os.system("rm " + student_output_file)
os.system("g++ -std=c++11 hw3_p1.cpp -o output")

print("Running binary file...")
os.system("./output " + str(input_data_file) + " " + str(student_output_file) +" "+ str(running_time_file))

print("Validating output...")

with open(student_output_file, "r") as f:
    num = int(f.readline())
    arr = [i.rstrip("\n") for i in f.readline().split(" ")]
    if num != len(arr):
        arr = arr[:num]

with open(output_data_file, "r") as f:
    num2 = int(f.readline())
    arr2 = [i.rstrip("\n") for i in f.readline().split(" ")]
    if num2 != len(arr2):
        arr2 = arr2[:num2]

if num == num2 and arr == arr2:
    print("Hurray Correct Submission :)")
elif num != num2:
    print("Bad Luck Wrong Submission (Wrong Length of the common nodes) :(")
    #print(num)
    #print(num2)
elif arr != arr2:
    print("Bad Luck Wrong Submission (Wrong common nodes) :(")
    #print(arr)
    #print(arr2)
