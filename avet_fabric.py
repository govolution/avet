#!/usr/bin/python3
# Author: Daniel Sauder
# License: https://www.gnu.org/licenses/gpl.txt or LICENSE file
# Web: https://github.com/govolution/avet

import glob, os, readline
import subprocess, stat

def rlinput(prompt, prefill=''):
	readline.set_startup_hook(lambda: readline.insert_text(prefill))
	try:
		return input(prompt)
	finally:
		readline.set_startup_hook()

# read and print build scripts
file_counter=0;
file_list = []
os.chdir("./build")
for file in glob.glob("build*.sh"):
	print(str(file_counter)+": "+file)
	file_list.append(file)
	file_counter+=1

choice=input("Input number of the script you want use and hit enter: ")
fname=file_list[int(choice)]

print("\nNow you can edit the build script line by line.\n")
commands=[]
commands.append("#/bin/bash")
# read the build scipt
with open(fname) as fi:
	for line in fi:
		line=line.rstrip()
		if line[0:2]!="#!" and line[0]=="#":
			print(line)
		elif line[0:2]!="#!":
			cmd=rlinput("", line)
			commands.append(cmd)

print("\nThe following commands will be executed:")

for cmd in commands:
	print(cmd)

input("\nPress enter to continue.")

os.chdir("..")
with open("build/tmpbuildscript.sh", mode="w") as f:
	f.write('\n'.join(commands))


print("\nExecution...\n\nPlease stand by...")
st = os.stat('./build/tmpbuildscript.sh')
os.chmod('./build/tmpbuildscript.sh', st.st_mode | stat.S_IEXEC)

subprocess.check_output(['/bin/bash','./build/tmpbuildscript.sh'], stderr=subprocess.STDOUT)

print("\nThe output file should be placed in the current directory.\n\nBye...")
