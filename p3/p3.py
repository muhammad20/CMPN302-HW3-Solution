import os

input_data_file = "input_p3.in"
output_data_file = "output_p3.out"
running_time_file = "p3.time"
student_output_file = "my_output_p3.out"

print("Compiling source code...")
os.system("rm " + student_output_file)
os.system("g++ -std=c++11 hw3_p3.cpp -o output")

print("Running binary file...")
os.system("./output " + str(input_data_file) + " " + str(student_output_file) + " " + str(running_time_file))

print("Validating output...")

graph = dict()
colors = []

with open(input_data_file, "r") as f:
    n, m = f.readline().rstrip("\n").split(" ")
    n = int(n)
    m = int(m)
    colors = [-1] * n
    for i in range(n):
        graph[i] = []
    for i in range(m):
        x, y = f.readline().rstrip("\n").split(" ")
        x, y = int(x), int(y)
        graph[x].append(y)
        graph[y].append(x)

with open(student_output_file, "r") as f:
    for i in range(n):
        x, y = f.readline().rstrip("\n").split(" ")
        x, y = int(x), int(y)
        colors[x] = y

set_colors = set()

for i in range(n):
    for j in graph[i]:
        if colors[j] == colors[i]:
            print("Bad Luck Wrong Submission :(")
            print("same Color at vertex {} & {}".format(j, i))
            exit(-1)
if len(set_colors) <= 5:
    print("Hurray Correct Submission :)")
else:
    print("Correct Coloring But Wrong optimal greedy algorithm :(")
