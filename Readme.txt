{\rtf1\ansi\ansicpg1252\cocoartf1265\cocoasubrtf210
{\fonttbl\f0\fnil\fcharset0 Cambria;}
{\colortbl;\red255\green255\blue255;}
\paperw15840\paperh12240\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\deftab720
\pard\pardeftab720\ri4320\qc

\f0\b\fs32 \cf0 Text Editor Instructions
\b0\fs24 \
\pard\pardeftab720\ri4320
\cf0 This program is is a basic line editor that is run in the command line and can carry out all of the commands listed below. I have included the text_editor.cc main file as well as document.h that contains the class header for the text_ed class, document.cc that contains the class functions, and the makefile. I also included a basic text file (test.txt) to edit, but any text file will do.  \
\pard\pardeftab720\ri4320

\b \cf0 \
\pard\pardeftab720\ri4320
\cf0 \ul \ulc0 Available Commands:\
\pard\pardeftab720\li1440\fi-1440\ri4320

\b0 \cf0 \ulnone g <name>	Get an existing file and load it into the editor. (The angle brackets are not typed as part of the command.)\
n	Turn line numbering on if off and off if on (line numbering on means that the number of each line is displayed at the left when the t command is given). \
q	Leave the editor without saving the text.\
x [name]	leave the editor and save the text in a file. The brackets indicate that the name is optional; if it is left out, the current file name is used. \
s [name] 	Save the file under the current name if a new name is not provided, otherwise change the current name to 
\i name
\i0  and save.\
h	Print a screen summarizing the available editor commands.\
a [N]	Start adding text after line N and before line N+1. Pressing return starts a new line, and typing control-D signals the end of add mode. If N is zero or missing, add lines at the beginning of the file.\
t [M [N]] 	Display lines M through N on the screen. If M is given but not N, display only line M; if neither is given, display the whole document.\
d M [N] 	Delete lines M through N (just line M if N is not given) and move the remaining lines up to fill the gap.\
m M [N] L 	Move lines M through N so that they come immediately after line L (and they no longer appear at their current location). When N is missing, move just line M\
c M [N] L 	Copy lines M through N so that they come immediately after line L (but they still appear at their original location). When N is missing, copy just line M\
\pard\pardeftab720\li2160\fi-2160\ri4320
\cf0 f [M [N]] /word/ 	Display each line between M and N on which 
\i word
\i0  appears. The slash characters are part of the command, not part of the string to be found. \
\pard\pardeftab720\li1440\fi-1440\ri4320
\cf0 R [M [N]] /old/new/	Replace each occurrence of string 
\i old
\i0  with string 
\i new
\i0  on each of lines M through N. The slash characters are part of the command, not part of the strings.\
}