#!/usr/bin/python3.9

import os, stat
import glob
import readline
import subprocess

def print_banner():
    banner = """
                       .|        ,       +
             *         | |      ((             *
                       |'|       `    ._____
         +     ___    |  |   *        |.   |' .---"|
       _    .-'   '-. |  |     .--'|  ||   | _|    |
    .-'|  _.|  |    ||   '-__  |   |  |    ||      |
    |' | |.    |    ||       | |   |  |    ||      |
 ___|  '-'     '    ""       '-'   '-.'    '`      |____
jgs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

"""
    print(banner)
    print("\nWelcome to the avet Assistant!\n")


# define rlinput (for user input with placeholder)
def rlinput(prompt, prefill=''):
	readline.set_startup_hook(lambda: readline.insert_text(prefill))
	try:
		return input(prompt)
	finally:
		readline.set_startup_hook()


# Enumerate and print available Build Scripts and asks for a choice
# also returns the choice
def print_build_scripts():
    os.chdir("./build")

    build_scripts = glob.glob('./build*.sh')
    build_scripts.sort()
    for i, script in enumerate(build_scripts):
        print("%d : %s" % (i, script.removeprefix("./")))
    
    print("\nWhich Script would you like to configure and build?")
    choice = int(input("Enter the corresponding number -> "))

    return build_scripts[choice]


# Print the information between the #Tags in the Build Script
def print_tag(choice, tag):
    print ("\n%s :" % tag)
    with open(choice, 'r') as file:
        switch = False
        for line in file:
            if line == "#%s_END\n" % tag:
                switch = False

            if switch:
                print(line, end='')   

            if line == "#%s_START\n" % tag:
                switch = True


# Here, the user is able to make changes of the options which are between the #CONFIGURATION Tags
# The whole Build Script with changes will be copied to a temporary script "avet_script_config.sh"
def build_script_configurator(choice):
    print("\nConfigure the Build Script")
    with open(choice, 'r') as file:
        script = file.readlines()
    with open("./avet_script_config.sh", 'w') as config:
        switch = False
        for line in script:
            
            if line == '#CONFIGURATION_END\n':
                switch = False

                # sandbox evasions
                print("\nDo you want to add sandbox evasions? [y/N]")
                answer = input("-> ")
                if answer.lower().strip() == "y" or answer.lower() == "yes":
                    modules = sandbox_evasions_pick()
                    
                for sand in modules:
                    evasion = rlinput("-> ", "add_evasion %s " % sand)
                    config.write(evasion + "\n")

            if switch:
                if line[0:2] == "# ":
                    print("\n"+line.strip())
                else:
                    # Replace host and port Variable with the decimal numbers configured in global_connect_config.sh
                    if "$GLOBAL" in line:
                        host, port = fetch_global_connect()
                        if line[0:6] == "LPORT=":
                            current_line = rlinput("-> " , "LPORT=" + port)
                        elif line[0:6] == "LHOST=":
                            current_line = rlinput("-> " , "LHOST=" + host)

                    # only configure the key, not the raw file
                    elif "generate_key" in line:
                        current_line = rlinput("-> " , line[0:-19]) 
                        current_line += line[-19:]
                    else:
                        current_line = rlinput("-> " , line.strip())
                    
                    config.write(current_line + "\n")
            else:
                config.write(line)
        
            if line == '#CONFIGURATION_START\n':
                switch = True
        print("\nExecutable will be created Shortly please wait.\n")


# In this function the user picks from a list of sandbox evasions.
# The chosen evasions are returned as a list.
def sandbox_evasions_pick():
    sandbox_modules = glob.glob("../source/implementations/evasion/*.h")
    sandbox_modules.sort()
    sandbox_modules.insert(0, "Finished Picking, Stop Here")
    to_add = []
    while True:
        for i, module in enumerate(sandbox_modules):
            print("%d : %s" % (i, module.removeprefix("../source/implementations/evasion/")))
            
        print("\nWhich module would you like to add?")
        choice = int(input("Enter the corresponding number -> "))
        if choice == 0:
            break
        to_add.append(sandbox_modules[choice].removeprefix("../source/implementations/evasion/").removesuffix(".h"))
        sandbox_modules.pop(choice)
    
    return to_add



# This fuction opens global_connect_config.sh and returns host and port
def fetch_global_connect():
    with open("./global_connect_config.sh") as connect:
        for line in connect:
            if line[0:13] == "GLOBAL_LHOST=":
                host = line[13:]
            if line[0:13] == "GLOBAL_LPORT=":
                port = line[13:]

    return host.strip(), port.strip()


# The tempory script will be executed
def build():
    os.chdir("..")

    st = os.stat('./build/avet_script_config.sh')
    os.chmod('./build/avet_script_config.sh', st.st_mode | stat.S_IEXEC)

    process = subprocess.run(["./build/avet_script_config.sh"], stdout=subprocess.PIPE)
    output = process.stdout.decode("utf-8")
    print(output)
    print("Your executable should be in the output folder!")


def main():
    print_banner()
    choice = print_build_scripts()
    print_tag(choice, "DESCRIPTION")
    build_script_configurator(choice)
    build()

    #os.remove("./build/avet_script_config.sh")

if __name__ == "__main__":
    main()
