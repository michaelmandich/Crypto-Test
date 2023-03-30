import subprocess

# Run Python script
print("now beginning python section")
for num in [1000, 10000, 100000]:
    cmd = ["python3.6", "newfile.py", str(num)]
    subprocess.run(cmd)

print("now beginning C section")
# Run C program
for num in [1000, 10000, 100000]:
    cmd = ["./cversion", str(num)]
    subprocess.run(cmd)
