"""
Implementation of Padding practical manipulation from
Functionality-Preserving Black-Box Optimization of Adversarial Windows Malware
by Demetrio et al.

Creates new Executable with suffix _padding.
"""

import os
import sys


exe_path = sys.argv[1]
amount = 64

if len(sys.argv) == 3 :
    amount = int(sys.argv[2])


print(f"Executing Padding manipulation on: {exe_path}")

with open(exe_path, 'r+b') as f:
    raw_bytes = bytearray(f.read())

    raw_bytes += os.urandom(amount)

    with open(f"{exe_path[:-4]}_padding.exe", 'wb') as nf:
        nf.write(raw_bytes)