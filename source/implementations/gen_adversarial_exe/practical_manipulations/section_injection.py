"""
Implementation of Section Injection practical manipulation from
Functionality-Preserving Black-Box Optimization of Adversarial Windows Malware
by Demetrio et al.

Can be used standalone with random bytes or via genetic optimizer.
"""

import os
import random
import string
import sys

import lief


def section_injection_on_bytes(exe_bytes: bytearray, section_population, vector_t):
    """
    Implementation of Section Injection practical manipulation. 
    Intended for use with genetic optimizer.

    Returns the bytes with the practical manipulation applied.
    """

    content = bytearray()
    for i, section in enumerate(section_population):
        content += section[:int(round(len(section) * vector_t[i]))]

    exe_object: lief.PE.Binary = lief.parse(exe_bytes)

    new_section = lief.PE.Section(
        ''.join(random.choice(string.ascii_lowercase) for i in range(5)))

    new_section.content = content
    new_section.characteristics = lief.PE.SECTION_CHARACTERISTICS.MEM_DISCARDABLE
    exe_object.add_section(new_section)

    builder = lief.PE.Builder(exe_object)
    builder.build()

    return bytearray(builder.get_build())


def section_injection(exe_path, amount):
    """
    Implementation of Section Injection practical manipulation.

    Create an adversarial example with practical manipulation applied.
    Random bytes are used and new sample has "_section_injection" as postfix.
    """

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


# run as script
if __name__ == "__main__":
    amount = 512
    if len(sys.argv) == 3:
        amount = int(sys.argv[2])

    section_injection(sys.argv[1], amount)
