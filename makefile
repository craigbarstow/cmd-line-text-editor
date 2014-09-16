text_editor: text_editor.o document.o
	g++ -o text_editor text_editor.o document.o

text_editor.o: text_editor.cc document.h
	g++ -c text_editor.cc

document.o: document.cc document.h 
	g++ -c document.cc 