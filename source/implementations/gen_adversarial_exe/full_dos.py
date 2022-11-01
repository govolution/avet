"""
Implementation of Full DOS practical manipulation from
Adversarial EXEmples: A Survey and Experimental Evaluation of 
Practical Attacks on Machine Learning for Windows Malware Detection
by Demetrio et al.

In this implementation, a new executable is created. 
"""

import os
import sys

import lief

exe_path = sys.argv[1]
print(f"Executing Full DOS manipulation on: {exe_path}")

exe_object: lief.PE.Binary = lief.parse(exe_path)

# 1
pe_header_offset = exe_object.dos_header.addressof_new_exeheader
range_to_perturb = list(range(2, 0x3c)) + list(range(0x40, pe_header_offset))

with open(exe_path, 'r+b') as f:
    # 2
    raw_bytes = bytearray(f.read())

    # 3 + 4
    for i in range_to_perturb:
        raw_bytes[i] = ord(os.urandom(1))

    with open(f"{exe_path[:-4]}_full_dos.exe", 'wb') as nf:
        # 5
        nf.write(raw_bytes)
