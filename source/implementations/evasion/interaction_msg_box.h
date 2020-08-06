#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"


// Spawn message box with hardcoded arithmetic task which only executes of it is solved correctly.
// 
// No arg
void interaction_msg_box(char *arg1) {
    DEBUG_PRINT("Applying interactive_msg_box evasion technique.\n");
  
    char *call = "powershell.exe ";
    char *pwsh = "Add-Type -AssemblyName System.Windows.Forms; Add-Type -AssemblyName System.Drawing;$window = New-Object System.Windows.Forms.Form;$window.Width = 500;$window.Height = 150;$Label = New-Object System.Windows.Forms.Label;$Label.Location = New-Object System.Drawing.Size(10,10);$Label.Text = 'Rechne aus: 5+5';$Label.AutoSize = $True;$window.Controls.Add($Label);$windowButton = New-Object System.Windows.Forms.Button;$windowButton.Location = New-Object System.Drawing.Size(10,60);$windowButton.Size = New-Object System.Drawing.Size(50,30);$windowButton.Text = 'OK';$windowButton.Enabled = $false;$windowTextBox = New-Object System.Windows.Forms.TextBox;$windowTextBox.Location = New-Object System.Drawing.Size(10,30);$windowTextBox.Size = New-Object System.Drawing.Size(350,350);$windowTextBox.add_TextChanged({$windowButton.Enabled = $windowTextBox.Text});$window.Controls.Add($windowTextBox);$windowButton.Add_Click({$calc = [int]$windowTextBox.Text;if($calc -eq 10) {Write-Host '0';}else {Write-Host '-1';}$window.Dispose();});$window.Controls.Add($windowButton);[void]$window.ShowDialog();";
    char *command = (char *) malloc(strlen(pwsh) + strlen(call) + 1); 
    FILE *fp;
    char buf[128];
    char retval[10];

    strcpy(command, call);
    strcat(command, pwsh);


    if ((fp = popen(command, "r")) == NULL) {
        DEBUG_PRINT("Error opening pipe!\n");
        exit(0);
    }

    while (fgets(buf, 128, fp) != NULL) {
        strcat(retval, buf);
    }
    if (strstr(retval, "0")) {
        DEBUG_PRINT("Proceed!\n");
    } else {
        exit(0);
    }
}