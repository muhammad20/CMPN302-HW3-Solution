import os

input_data_file = "input_p5.in"
output_data_file = "output_p5.out"
running_time_file = "p5.time"
student_output_file = "my_output_p5.out"

print("Compiling source code...")
os.system("rm " + student_output_file)
os.system("g++ -std=c++11 hw3_p5.cpp -o output")

print("Running binary file...")
os.system("./output " + str(input_data_file) + " " + str(student_output_file) + " " + str(running_time_file))

print("Validating output...")

with open(student_output_file, "r") as f:
    ans_num = int(f.readline().rstrip("\n"))
    ans = f.readline().rstrip("\n")

with open(output_data_file, "r") as f:
    ans2_num = int(f.readline().rstrip("\n"))
    ans2 = f.readline().rstrip("\n")

if ans == ans2 and ans_num == ans2_num:
    print("Hurray Correct Submission :)")
else:
    print("Bad Luck Wrong Submission :(")
