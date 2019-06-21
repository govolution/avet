#pragma once

#include "../debug_print/debug_print.h"


// Dummy function to handle cases where no command is executed.
// In that case, both arguments command and command_size are ignored.
void no_command(const char *command, int command_size)  {
    DEBUG_PRINT("\"no_command\" command_exec function called.\n");     
}
