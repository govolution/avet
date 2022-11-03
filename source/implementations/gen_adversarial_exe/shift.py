"""
Implementation of Shift practical manipulation from
Adversarial EXEmples: A Survey and Experimental Evaluation of 
Practical Attacks on Machine Learning for Windows Malware Detection
by Demetrio et al.

Creates new Executable with suffix _shift.
"""

import math
import os
import struct
import sys

import lief


exe_path = sys.argv[1]
desired_amount = 64

if len(sys.argv) == 3:
    amount = int(sys.argv[2])


print(f"Executing Shift manipulation on: {exe_path}")

exe_object: lief.PE.Binary = lief.parse(exe_path)

# calculate how much to shift based on alignment
pe_header_offset = exe_object.dos_header.addressof_new_exeheader
section_file_alignment = exe_object.optional_header.file_alignment
shift_by = int(math.ceil(
    desired_amount / section_file_alignment)) * section_file_alignment

optional_header_size = exe_object.header.sizeof_optional_header
first_section_offset = exe_object.sections[0].offset
coff_header_size = 24
section_entry_length = 40
size_of_raw_data_pointer = 20


with open(exe_path, 'r+b') as f:
    raw_bytes = bytearray(f.read())

    for i, _ in enumerate(exe_object.sections):
        # calculate positions of PointerToRawData
        p_raw_data = (
            pe_header_offset
            + coff_header_size
            + optional_header_size
            + (i * section_entry_length)
            + size_of_raw_data_pointer
        )

        # increase PointerToRawData by shift_by
        old_pointer_to_raw_data = struct.unpack(
            "<I", raw_bytes[p_raw_data: p_raw_data + 4])[0]
        new_pointer_to_raw_data = struct.pack(
            "<I", old_pointer_to_raw_data + shift_by)
        raw_bytes[p_raw_data: p_raw_data + 4] = new_pointer_to_raw_data

    raw_bytes = raw_bytes[:first_section_offset] + os.urandom(shift_by) + raw_bytes[first_section_offset:]

    with open(f"{exe_path[:-4]}_shift.exe", 'wb') as nf:
        nf.write(raw_bytes)