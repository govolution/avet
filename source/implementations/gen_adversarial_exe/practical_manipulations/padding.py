"""
Implementation of Padding practical manipulation from
Functionality-Preserving Black-Box Optimization of Adversarial Windows Malware
by Demetrio et al.

Can be used standalone with random bytes or via genetic optimizer.
"""

import copy
import os
import sys


def padding_on_bytes(exe_bytes: bytearray,  section_population, vector_t):
    """
    Implementation of Padding practical manipulation. 
    Intended for use with genetic optimizer.

    Returns the bytes with the practical manipulation applied.
    """

    content = bytearray()
    for i, section in enumerate(section_population):
        content += section[:int(round(len(section) * vector_t[i]))]

    raw_bytes = copy.deepcopy(exe_bytes)

    # append content
    raw_bytes += content

    return raw_bytes


def padding(exe_path, amount):
    """
    Implementation of Padding practical manipulation.

    Create an adversarial example with practical manipulation applied.
    Random bytes are used and new sample has "_padding" as postfix.
    """

    print(f"Executing Padding manipulation on: {exe_path}")

    with open(exe_path, 'r+b') as f:
        raw_bytes = bytearray(f.read())

        # append content
        raw_bytes += os.urandom(amount)

        with open(f"{exe_path[:-4]}_padding.exe", 'wb') as nf:
            nf.write(raw_bytes)


# run as script
if __name__ == "__main__":
    amount = 512
    if len(sys.argv) == 3:
        amount = int(sys.argv[2])

    padding(sys.argv[1], amount)
