//Craig Barstow 
//Document.cc 
//File containing the functions for the text_ed class

#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include "document.h"
const int DEFCAP = 5;

//Note that the document holds capacity lines, numbered 1... capacity.
//Spot 0 in the array is ignored, so array length is always "capacity+1"

//PRIVATE

bool text_ed::copy_to_buffer(int start, int end) {  // Copy lines start through end to the buffer.
  int buff_capacity=end-start+1;
  int i,j,k;
  buffer= new string [buff_capacity+1]; 
  for (i=start,j=0; i<=end; i++,j++) { //copy document contents into buffer
    buffer[j]=document[i];
  }
  return true;
}

bool text_ed::copy_to_document(int start, int end, int location) { // Copy the entire buffer to the document starting at line start.
  int buff_capacity=end-start+1;
  int i,j;
  for (i=location+1,j=0; j<buff_capacity; i++,j++) {
    document[i]=buffer[j]; //copy buffer contents to document
  }
  delete [] buffer;
  buffer = NULL; //set buffer pointer to null so destructor function works properly
  return true;
}

bool text_ed::open_up(int start, int num_lines) { // Create num_lines empty lines following start.
 int space_left;
 int i;
 space_left = capacity-last_line;
 while (space_left <=num_lines) {
   expand_document();
   space_left = capacity - last_line;
 }
 for (i=last_line; i>start; i--) {
   document[i+num_lines] = document[i];
 }
 last_line += num_lines;
 return true;
}

void text_ed::expand_document() { // Add more lines to the document array, e.g., double capacity.
 int i;
 capacity *= 2; //double document capacity
   string* temp = new string[capacity+1];
   for (i=0; i<capacity/2; i++) { //copy contents of document into temp
     temp[i]=document[i];
   }
   delete [] document; //delete contents of original document
   document = temp; //set document equal to temp contents of temp
}

// PUBLIC

text_ed::text_ed() { //constructor function
 capacity = DEFCAP;
 last_line = 0;
 document = new string[capacity+1];
 buffer = NULL;
 buffer_size = 0;
}

text_ed::text_ed(text_ed& rhs) { //copy constructor
 document = NULL;
 (*this) = rhs;
}

text_ed& text_ed::operator=(text_ed& rhs) { //overloading assignment
 if ( this != &rhs ) {
   delete [] document;
   last_line = rhs.last_line;
   capacity = rhs.capacity;
   document = new string[capacity];
   for (int i=0; i<last_line; i++) {
     document[i] = rhs.document[i];
   }
 }
 return (*this);
}

text_ed::~text_ed() { //destructor function
 delete [] document;
 delete [] buffer;
}

string text_ed::filename() { // Return current_file.
 return current_file;
}

int text_ed::get_last_line() { // Return last_line.
 return last_line;
}

bool text_ed::load_file(string filename) { // Read the file and remember the name in current_file.
 string nextline;
 current_file = filename;
 last_line=0;
 ifstream form (filename.c_str());
 while (getline(form,nextline)) { //while lines can be retrieved, insert them into document array
   insert_line(nextline,last_line);
 }
 form.close();
 return true;
}

bool text_ed::save_file() { //function to check if saving to a given filename is possible and save it
 if (save_file(current_file)) { 
 return true;
 } else {
   return false;
 }
}

bool text_ed::save_file (string filename) { // Save the text under the new name; remember the new name in current_file.
 ofstream savefile;
 int i;
 savefile.open(filename.c_str());
 for (i=1; i<=last_line; i++) { //save each line from document arry to file
   savefile << document[i]; 
   savefile << endl;
 }
 savefile.close();
 return true;
}

bool text_ed::insert_line(string newline,int n) { // Insert newline as a new line following line n.
 open_up(n,1);
 document[n+1] = newline;
 if (n > last_line-1) {
   n=last_line+1;
     }
 return true;
}

bool text_ed::delete_lines(int start, int end) { // Delete lines from start position to end position.
  int i;
  int num_lines = end-start;
  for (i=start; i<=last_line; i++) { //delete lines from start to end positions
    document[i] = document[i+num_lines+1];
  }
  last_line = last_line-num_lines-1;
  return true;
}

bool text_ed::get_line(string& dest, int n) { // Copy line n into dest.
  if (n>last_line) {
    return false;
  }
 dest = document[n];
 return true;
}

bool text_ed::copy_lines(int start, int end, int location) { //copy lines start through end to follow line location.
  int buff_capacity=end-start+1;
  copy_to_buffer(start,end);
  open_up(location, buff_capacity);
  copy_to_document(start,end,location);
  return true;
}

bool text_ed::move_lines(int start, int end, int location) { //move lines between two given lines (start to end) to after the given line location
  if (location<start || location>end) { //make sure new line location isnt between the start and end selections
    copy_lines(start,end,location);
    delete_lines(start,end); //delete the original lines so there aren't duplicates
    return true;
  } else {
    cout << "Error. Make sure location given is not between the two lines selected." << endl;
    return false;
  }
}

bool text_ed::replace(string oldstr, string newstr, int start, int end) { //replace old word with new word between given lines (start to end)
  int i, j;
  for (i=start; i<=end; i++) { //for each line in the given area
    for(j=0;j<document[i].length(); j++){ //for character in string
      if(document[i].substr(j,oldstr.length())==oldstr){ //if substring found starting at the character position
	document[i].replace(j,oldstr.length(),newstr); //replace old string with new string
      }
    }
  }
  return true;
}
