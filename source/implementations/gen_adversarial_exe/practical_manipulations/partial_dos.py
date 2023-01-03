"""
Implementation of Partial DOS proposed in
Explaining Vulnerabilities of Deep Learning to Adversarial Malware Binaries
by Demetrio et al.

Can be used standalone with random bytes or via genetic optimizer.
This file is not relevant for thesis.
"""

import os
import sys
import copy


def partial_dos_on_bytes(exe_bytes: bytearray, section_population, vector_t):
    content = bytearray()
    for i, section in enumerate(section_population):
        content += section[:int(round(len(section) * vector_t[i]))]
    range_to_perturb = list(range(2, 0x3C))

    raw_bytes = copy.deepcopy(exe_bytes)

    counter = 0
    for i in range_to_perturb:
        raw_bytes[i] = content[counter % len(content)]
        counter += 1

    return raw_bytes


def partial_dos(exe_path):
    print(f"Executing Partial DOS manipulation on: {exe_path}")

    range_to_perturb = list(range(2, 0x3C))

    with open(exe_path, 'r+b') as f:
        raw_bytes = bytearray(f.read())

        for i in range_to_perturb:
            raw_bytes[i] = ord(os.urandom(1))

        with open(f"{exe_path[:-4]}_partial_dos.exe", 'wb') as nf:
            nf.write(raw_bytes)


# run as script
if __name__ == "__main__":
    partial_dos(sys.argv[1])
