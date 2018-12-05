#!/bin/bash
# This path assumes that format.sh is called from the avet folder, as should be the case then executing build scripts
./tools/sh_format $1 | tr -d "\n" | tr -d "x" | tr -d "\\" | tr -d "\"" | tr -d ";"

