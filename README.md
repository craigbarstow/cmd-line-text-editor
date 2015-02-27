cmd-line-text-editor
====================

A command line text editor written in C++ for a class project

Available commands are as follows:

g <name>	Get an existing file and load it into the editor. (The angle brackets are not typed as part of the command.)
n	        Turn line numbering on if off and off if on (line numbering on means that the number of each line is displayed at the left when the t command is given). 
q	        Leave the editor without saving the text.
x [name]	leave the editor and save the text in a file. The brackets indicate that the name is optional; if it is left out, the current file name is used. 
s [name] 	Save the file under the current name if a new name is not provided, otherwise change the current name to name and save.
h	        Print a screen summarizing the available editor commands.
a [N]	    Start adding text after line N and before line N+1. Pressing return starts a new line, and typing control-D signals the end of add mode. If N is zero or missing, add lines at the beginning of the file.
t [M [N]] 	Display lines M through N on the screen. If M is given but not N, display only line M; if neither is given, display the whole document.
d M [N] 	  Delete lines M through N (just line M if N is not given) and move the remaining lines up to fill the gap.
m M [N] L 	Move lines M through N so that they come immediately after line L (and they no longer appear at their current location). When N is missing, move just line M
c M [N] L 	Copy lines M through N so that they come immediately after line L (but they still appear at their original location). When N is missing, copy just line M
f [M [N]] /word/ 	    Display each line between M and N on which word appears. The slash characters are part of the command, not part of the string to be found. 
R [M [N]] /old/new/ 	Replace each occurrence of string old with string new on each of lines M through N. The slash characters are part of the command, not part of the strings.
