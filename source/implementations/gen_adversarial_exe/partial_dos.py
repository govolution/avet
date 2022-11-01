"""
Implementation of Partial DOS proposed in
Explaining Vulnerabilities of Deep Learning to Adversarial Malware Binaries
by Demetrio et al.

In this implementation a new executable is created and manipulated bytes are random for now. 
"""

import os
import sys


exe_path = sys.argv[1]
print(f"Executing Partial DOS manipulation on: {exe_path}")

range_to_perturb = list(range(2, 0x3C))

with open(exe_path, 'r+b') as f:
    raw_bytes = bytearray(f.read())

    for i in range_to_perturb:
        raw_bytes[i] = ord(os.urandom(1))

    with open(f"{exe_path[:-4]}_partial_dos.exe", 'wb') as nf:
        nf.write(raw_bytes)
