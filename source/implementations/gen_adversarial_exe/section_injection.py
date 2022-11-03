"""
Implementation of Section Injection practical manipulation from
Functionality-Preserving Black-Box Optimization of Adversarial Windows Malware
by Demetrio et al.

Creates new Executable with suffix _section_injection.
"""

import os
import random
import string
import sys

import lief

exe_path = sys.argv[1]
amount = 64

if len(sys.argv) == 3:
    amount = int(sys.argv[2])


print(f"Executing Section Injection manipulation on: {exe_path}")

exe_object: lief.PE.Binary = lief.parse(exe_path)


new_section = lief.PE.Section(
    ''.join(random.choice(string.ascii_lowercase) for i in range(5)))
new_section.content = [ord(os.urandom(1)) for _ in range(amount)]
new_section.characteristics = lief.PE.SECTION_CHARACTERISTICS.MEM_DISCARDABLE
exe_object.add_section(new_section)

builder = lief.PE.Builder(exe_object)
builder.build()
builder.write(f"{exe_path[:-4]}_section_injection.exe")
