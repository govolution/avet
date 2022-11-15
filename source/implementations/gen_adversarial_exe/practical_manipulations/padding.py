"""
Implementation of Padding practical manipulation from
Functionality-Preserving Black-Box Optimization of Adversarial Windows Malware
by Demetrio et al.

Creates new Executable with suffix _padding.
"""

import copy
import os
import sys


def padding_on_bytes(exe_bytes: bytearray,  section_population, vector_t):

    content = bytearray()
    for i, section in enumerate(section_population):
        content += section[:int(round(len(section) * vector_t[i]))]

    raw_bytes = copy.deepcopy(exe_bytes)

    raw_bytes += content

    return raw_bytes


def padding(exe_path, amount):

    print(f"Executing Padding manipulation on: {exe_path}")

    with open(exe_path, 'r+b') as f:
        raw_bytes = bytearray(f.read())

        raw_bytes += os.urandom(amount)

        with open(f"{exe_path[:-4]}_padding.exe", 'wb') as nf:
            nf.write(raw_bytes)


# run as script
if __name__ == "__main__":
    amount = 512
    if len(sys.argv) == 3:
        amount = int(sys.argv[2])

    padding(sys.argv[1], amount)
