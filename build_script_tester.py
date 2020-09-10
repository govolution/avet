#!/usr/bin/python3

import os
import glob
import subprocess
import re

# get list of build scripts and create directory named testing
build_scripts = glob.glob('./build/build*.sh')
count = len(build_scripts)

if not os.path.isdir("./testing"):
    os.mkdir("./testing")


# Create File. If it exists already, it will be cleared
open("./testing/result.txt", 'w').close()


# prepare for summary
failed = "Build Scripts with errors:\n"


# run all build scripts and save output in buffer
buffer = []
current = 1
for script in build_scripts:
    print("\n[%d/%d]\t--Testing %s--" % (current, count, script[8:]))
    process = subprocess.run(
        [script], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    output = process.stdout.decode("utf-8")

    buffer.append("--Testing  %s--\n%s" % (script[8:], output))

    current_script = script[14:-3]

    # test for errors while building
    # Search for keywords in stdout and stderr
    keywords = ["cannot find", "fault", "unable", "error", "Error",
                "Unable", "No such file or directory", "warning", "undeclared"]
    if any(x in output for x in keywords):
        failed += "%s:\n" % current_script
        for item in keywords:
            finding = re.search(".*"+item+".*", output)
            if bool(finding):
                print(finding.group())
                failed += "\t%s:\n" % finding.group()

    # test if .exe is generated
    if os.path.isfile("./output/%s.exe" % current_script):
        print("%s executable could be created\n" % current_script)
    else:
        print("%s executable could NOT be created\n" % current_script)
        failed += "\t%s executable could NOT be created\n" % current_script
    current += 1


# print summary
summary = "---------------------------Summary---------------------------\n"
summary += failed

print(summary)


# write buffer in file result.txt
buffer.append(summary)
with open("./testing/result.txt", 'w') as result:
    result.writelines(buffer)
