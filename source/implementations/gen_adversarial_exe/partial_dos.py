"""
Implementation of Partial DOS proposed in
Explaining Vulnerabilities of Deep Learning to Adversarial Malware Binaries
by Demetrio et al.

In this implementation the manipulations are in place and manipulated bytes are random for now. 
"""

import os
import sys

import lief

exe_path = sys.argv[1]
print(exe_path)

exe_object : lief.PE.Binary = lief.parse(exe_path)

print('DOS Header')
print(exe_object.dos_header)

# 1
coff_header_offset = exe_object.dos_header.addressof_new_exeheader

with open(exe_path, 'r+b') as f:
    # 3
    for i in range(2, 60):
        f.seek(i)
        f.write(os.urandom(1))