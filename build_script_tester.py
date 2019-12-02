#!/usr/bin/python3

import os
import glob
import subprocess

# get list of build scripts and create directory named testing
build_scripts = glob.glob('./build/build_*.sh')
count = len(build_scripts)

if not os.path.isdir("./testing"):
    os.mkdir("./testing")


# Create File. If it exists already, it will be cleared
open("./testing/result.txt", 'w').close()


# run all build scripts and save output in buffer
buffer = []
current = 1
for script in build_scripts:
    print("\n[%d/%d]\t--Testing %s--" % (current, count, script[8:]))
    output = subprocess.run([script], stdout=subprocess.PIPE)
    buffer.append("--Testing  %s--\n%s" % (script[8:], output.stdout.decode("utf-8")))
    
    current_script = script[14:-3]
    # test if .exe is generated
    summary = "-------Summary-------\n"
    if os.path.isfile("./output/%s.exe" % current_script):
        summary += "%s executable could be created\n" % current_script
        print("%s.exe could be created\n" % current_script)
    else:
        summary += "%s executable could NOT be created\n" % current_script
        print("%s.exe could NOT be created\n" % current_script)
    current += 1

print(summary)

buffer.append(summary)
# write buffer in file result.txt
with open("./testing/result.txt", 'w') as result:
    result.writelines(buffer)
