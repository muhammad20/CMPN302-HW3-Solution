import os

input_data_file = "input_p4.in"
output_data_file = "output_p4.out"
running_time_file = "p4.time"
student_output_file = "my_output_p4.out"

print("Compiling source code...")
os.system("rm " + student_output_file)
os.system("g++ -std=c++11 hw3_p4.cpp -o output")

print("Running binary file...")
os.system("./output " + str(input_data_file) + " " + str(student_output_file) + " " + str(running_time_file))

print("Validating output...")

with open(student_output_file, "r") as f:
    ans = int(f.readline().rstrip("\n"))

with open(output_data_file, "r") as f:
    ans2 = int(f.readline().rstrip("\n"))

if ans == ans2:
    print("Hurray Correct Submission :)")
else:
    print("Bad Luck Wrong Submission :(")
