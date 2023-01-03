"""
Implementation of Extend practical manipulation from
Adversarial EXEmples: A Survey and Experimental Evaluation of
Practical Attacks on Machine Learning for Windows Malware Detection
by Demetrio et al.

Can be used standalone with random bytes or via genetic optimizer.
"""

import copy
import itertools
import math
import os
import struct
import sys

import lief


def extend_on_bytes(exe_bytes: bytearray, section_population, vector_t):
    """
    Implementation of Extend practical manipulation. 
    Intended for use with genetic optimizer.

    Returns the bytes with the practical manipulation applied.
    """

    content = bytearray()
    for i, section in enumerate(section_population):
        content += section[:int(round(len(section) * vector_t[i]))]

    amount = len(content)

    exe_object: lief.PE.Binary = lief.parse(exe_bytes)

    # calculate how much to extend based on alignment
    pe_header_offset = exe_object.dos_header.addressof_new_exeheader
    section_file_alignment = exe_object.optional_header.file_alignment
    extension = int(math.ceil(
        amount / section_file_alignment)) * section_file_alignment

    new_pe_header_offset = exe_object.dos_header.addressof_new_exeheader + extension
    optional_header_size = exe_object.header.sizeof_optional_header
    coff_header_size = 24
    section_entry_length = 40
    size_of_raw_data_pointer = 20

    raw_bytes = copy.deepcopy(exe_bytes)

    # increase offset to pe header at location 0x3C by extension
    raw_bytes[0x3C:0x40] = struct.pack(
        "<I", pe_header_offset + extension)

    # increase SizeOfHeaders by extension
    raw_bytes[pe_header_offset + 60 + 20 + 4: pe_header_offset + 60 + 20 + 4 +
              4] = struct.pack("<I", exe_object.optional_header.sizeof_headers + extension)

    # insert null bytes * extension times
    pattern = itertools.cycle("\x00")
    [raw_bytes.insert(pe_header_offset, ord(next(pattern)))
        for _ in range(extension)]

    for i, _ in enumerate(exe_object.sections):
        # calculate positions of PointerToRawData
        p_raw_data = (
            new_pe_header_offset
            + coff_header_size
            + optional_header_size
            + (i * section_entry_length)
            + size_of_raw_data_pointer
        )

        # increase PointerToRawData by extension
        old_pointer_to_raw_data = struct.unpack(
            "<I", raw_bytes[p_raw_data: p_raw_data + 4])[0]
        new_pointer_to_raw_data = struct.pack(
            "<I", old_pointer_to_raw_data + extension)
        raw_bytes[p_raw_data: p_raw_data + 4] = new_pointer_to_raw_data

    # perturb enlarged dos header
    range_to_perturb = list(range(2, 0x3c)) + \
        list(range(0x40, new_pe_header_offset))

    counter = 0
    for i in range_to_perturb:
        raw_bytes[i] = content[counter % len(content)]
        counter += 1

    return raw_bytes


def extend(exe_path, amount):
    """
    Implementation of Extend practical manipulation.
    
    Create an adversarial example with practical manipulation applied.
    Random bytes are used and new sample has "_extend" as postfix.
    """

    print(f"Executing Extend manipulation on: {exe_path}")

    exe_object: lief.PE.Binary = lief.parse(exe_path)

    # calculate how much to extend based on alignment
    pe_header_offset = exe_object.dos_header.addressof_new_exeheader
    section_file_alignment = exe_object.optional_header.file_alignment
    extension = int(math.ceil(
        amount / section_file_alignment)) * section_file_alignment

    new_pe_header_offset = exe_object.dos_header.addressof_new_exeheader + extension
    optional_header_size = exe_object.header.sizeof_optional_header
    coff_header_size = 24
    section_entry_length = 40
    size_of_raw_data_pointer = 20

    with open(exe_path, 'r+b') as f:
        raw_bytes = bytearray(f.read())

        # increase offset to pe header at location 0x3C by extension
        raw_bytes[0x3C:0x40] = struct.pack(
            "<I", pe_header_offset + extension)

        # increase SizeOfHeaders by extension
        raw_bytes[pe_header_offset + 60 + 20 + 4: pe_header_offset + 60 + 20 + 4 +
                  4] = struct.pack("<I", exe_object.optional_header.sizeof_headers + extension)

        # insert null bytes * extension times
        pattern = itertools.cycle("\x00")
        [raw_bytes.insert(pe_header_offset, ord(next(pattern)))
         for _ in range(extension)]

        for i, _ in enumerate(exe_object.sections):
            # calculate positions of PointerToRawData
            p_raw_data = (
                new_pe_header_offset
                + coff_header_size
                + optional_header_size
                + (i * section_entry_length)
                + size_of_raw_data_pointer
            )

            # increase PointerToRawData by extension
            old_pointer_to_raw_data = struct.unpack(
                "<I", raw_bytes[p_raw_data: p_raw_data + 4])[0]
            new_pointer_to_raw_data = struct.pack(
                "<I", old_pointer_to_raw_data + extension)
            raw_bytes[p_raw_data: p_raw_data + 4] = new_pointer_to_raw_data

        # perturb enlarged dos header
        range_to_perturb = list(range(2, 0x3c)) + \
            list(range(0x40, new_pe_header_offset))
        for i in range_to_perturb:
            raw_bytes[i] = ord(os.urandom(1))

        with open(f"{exe_path[:-4]}_extend.exe", 'wb') as nf:
            nf.write(raw_bytes)


# run as script
if __name__ == "__main__":
    amount = 512
    if len(sys.argv) == 3:
        amount = int(sys.argv[2])

    extend(sys.argv[1], amount)
