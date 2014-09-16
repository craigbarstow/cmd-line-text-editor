//Craig Barstow
//Test Editor

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <math.h>
#include <locale>
#include <sstream>
using namespace std;
#include "document.h"
#define NOT_FOUND -1

void Show_Commands() { //display all available commands
 cout << "h                        Show this list" << endl;
 cout << "g <name>                 Get a file" << endl;
 cout << "n                        Toggle line numbering" << endl;
 cout << "q                        Quit without saving file" << endl;
 cout << "x [name]                 Save file and exit" << endl;
 cout << "s [name]                 Save file" << endl;
 cout << "a [N]                    Add lines after line N" << endl;
 cout << "t [M [N]]                Type lines M through N" << endl;
 cout << "d M [N]                  Delete lines M through N" << endl;
 cout << "m M [N] L                Move lines M through N to follow line L" << endl;
 cout << "c M [N] L                Copy lines M through N to follow line L" << endl;
 cout << "f [M [N]] /word/         Type lines M through N containing word" << endl;
 cout << "r [M [N]] /word1/word2/  Replace word1 by word2 on lines M through N" << endl;
}

bool get_file(text_ed & T, string cl) { //load file to be edited
 string filename;
 string::size_type start,end;
 start = cl.find_first_not_of(' ',1); //find start of filename substring starting after the 'g' command
 if (start == string::npos) { //if string not found return false
   return false;
 }
 end = cl.find(' ',start);
 if ( end == string::npos ) { //if end of filename substring cant be found, set it equal to the end of the string
   end = cl.size();
 }
 filename = cl.substr(start,end-start);
 return T.load_file(filename); //load the file 
}

bool save_file(text_ed & T, string cl) { //take out couts
  string fn, new_name,CL=cl;
  int start,end;
  bool ok;
  string current_file = T.filename();
  if (cl.find_first_not_of(" ", 1)==string::npos) {  //if no filename given, save as current filename
    T.save_file(current_file); 
    cout << "Saved as \"" << current_file <<"\""<< endl;
    return true;
  } else { //if filename give after s in command line, save as given filename
    new_name = cl.substr(2,cl.length());
    ok = T.save_file(new_name);
    if (!ok) { //if it cant save to the given filename return an error message
      cout << "Can't save to file \"" << T.filename() << "\"." << endl;
    }
    return ok; //return the boolean value indicating whether the save was successful
  }
}

bool type_lines(text_ed & T, string cl, bool toggle) { //type lines between two given integers. If only one integer is given, type just that line. If no integers are given, type the whole document.
  bool error_state=false;
  string aline,m_string,n_string;
  int i=0,m=0,n=0,p=1,m_begin, m_end, n_begin, n_end,document_length,count=0;
  if (cl.find_first_not_of(" ",1)!=string::npos){ //this section selects the first integer from the substring
    m_begin= cl.find_first_not_of(" ",1);
    m_end = cl.find_first_of(" ", m_begin)-2;
    m_string=cl.substr(m_begin,m_end);
    if (!(istringstream(m_string) >> m)) { //if the substring cant be converted to an integer, set error state to false
      error_state=true;
    } else {count++;} //if it can be converted, increment the count
    if (cl.find_first_not_of(" ", m_end+2)!=string::npos){ //this section selects the second integer from the substring
      n_begin = cl.find_first_not_of(" ", m_end+2);
      n_end = cl.find_first_of("",n_begin);
      n_string=cl.substr(n_begin,n_end);
      if (!(istringstream(n_string) >> n)){
	error_state=true;
      } else {count++;} //if it can be converted, increment the count
    }
    //SECTION TO MAKE SURE INTEGERS MAKE SENSE
    if (m<=0){ //if the line number is below 1, this automatically sets it to one
      error_state=true;
    }
    document_length = T.get_last_line();
    if (m>document_length) { //this makes sure that the starting position is less than the last line of the document
      error_state = true;
      cout << "The first line number given exceeds file length." << endl;
    }
    if (n>document_length) { //if the ending position is greater than the document length, toggle error state to true
      error_state=true;
    }
    if (count!=1 && n<m){ //if both a  start position and an end position are put in, and the end value is greater than the starting value, the error state will be toggled to true
      error_state=true;
    }
  }
  if (!error_state) { 
    switch (count) {
    case 0: //if no integers are input, print the entire document
      n = T.get_last_line();
      for (i = 1; i<=n ;i++) {
	if (toggle) { //if toggling line numbers is on, print line numbers
	  cout << setw(2) << i << "  ";
	}
	if (T.get_line(aline,i)) { //if the given line can be retrieved, display it
	  cout << aline << endl;
	} else { //if it can't, return false
	  return false;
	}
      }      
      break;
    case 1: //if one integer is input, print just that line
      if (toggle) { //if toggling line numbers is on, print line numbers
	cout << setw(2) << m << "  ";
      }
      if (T.get_line(aline,m)) { //if the line can be retrieved, display it
	cout << aline << endl;
      } else {return false;} 
      break; 
    case 2:
      for (i = m; i<=n; i++){ // for each line between the start and end line positions
	if (toggle) { //if toggling line numbers is on, print the line numbers
	  cout << setw(2) << i << " ";
	} 
	if (T.get_line(aline,i)) { //if the line can be retrieved, display it
	  cout << aline << endl;
	} else {
	  return false;
	}
      }
      break;
    }
    return true;
  }
  return false;
}

bool add_lines (text_ed & T, string cl) { //add lines to the array representing the document, but don't save it
  bool error_state = false;
  int m_begin, m_end, m, count=0;  
  string new_line, m_string;
  if (cl.find_first_not_of(" ",1)!=string::npos){ //this section selects the integer from the substring
    m_begin= cl.find_first_not_of(" ",1);
    m_end = cl.find_first_of(" ", m_begin)-2;
    m_string=cl.substr(m_begin,m_end);
    if (istringstream(m_string) >> m) { //if the substring can be converted to an integer, increment the count
      count++;
    }
    //SECTION TO MAKE SURE INTEGER MAKES SENSE
    if ((m < 0 || m > T.get_last_line()) && count==1) {
      error_state=true;
    }
    if (count==1 && m==0) { //if m is equal to zero, set count to zero so lines will be added at the beginning of the document
      count=0;
    }
  }
  if (!error_state) {
    switch (count) {
    case 0: //if no integer is present, add lines starting at document start
      cout << " Press Control +  D to stop"<<endl;
      while (getline(cin,new_line)){ //continue accepting new lines until ctrl d is entered
	T.insert_line(new_line,m=0);
	m++;
      }
      cin.clear();
      cout << endl;
      break;
    case 1:
      cout << " Press control-D to stop"<<endl;
      while (getline(cin,new_line)){ //continue accepting new lines until ctrl d is entered
	T.insert_line(new_line,m);
	m++;
      }
      cin.clear();
      cout << endl;
      break;
    default:
      return false;
    }
    return true; //return true after running switch statement contents
  }
    return false; //return false if error state is true
}

bool delete_line (text_ed & T, string cl) { //delete lines between two given line positions. If only one position is given, delete just that line
  bool error_state=false;
  string aline,M_string,N_string;
  int i = 0,m=0,n=0,p=1,m_start, m_end, n_start, n_end,number, count=0;
  if (cl.find_first_not_of(" ",1)!=string::npos){ //if first integer is part of substring isolate it
    m_start= cl.find_first_not_of(" ",1);
    m_end = cl.find_first_of(" ", m_start)-2;
    M_string=cl.substr(m_start,m_end);
    if (!(istringstream(M_string) >> m)) { //if substring representing first integer can't be converted to integer form, toggle error state to true
      error_state=true;
    } else {count++;} //else, increment count to show that first integer is present
    if (cl.find_first_not_of(" ", m_end+2)!=string::npos){ //if second integer is part of substring isolate it
      n_start = cl.find_first_not_of(" ", m_end+2);
      n_end = cl.find_first_of("",n_start);
      N_string=cl.substr(n_start,n_end);
      if (!(istringstream(N_string) >> n)){ //if substring representing second integer can't be converted to int form, toggle error state to true
	error_state=true;
      } else {count++;} //increment count to show that second integer is present
    }
    //section to make sure the integer values are reasonable
    if (m<=0){ //if integer value of starting position is less 1, toggle error
      error_state=true;
    }
    if (count==2) { //if two integers are present, if second is less than first, or if first is greater than document length, toggle error state
      if (n<m || n>T.get_last_line()) {
	error_state=true;
      }
    }
    if (!error_state) {
      if (count==2){ //if two integers are present, delete the lines between them
	T.delete_lines(m,n);
      }
      else if (count==1) { //if one integer is present, delete just that line
	T.delete_lines(m,m);
      }
    } else {
      return false; //return false if something other than 1 or two integers are present
    } return true; //if !error_state
  }
  return false; //return false if nothing is found after the 'd' statement in the command line, or if error state is true
}

bool copy_line (text_ed & T, string cl) { //Copy area between two given line numbers (inclusive) to after a third given line number if three numbers are given. If only two integers are given, move the line at the first position to after the position given by the second integer
  bool error_state=false;
  string aline,M_string,N_string,O_string;
  int i = 0,m=0,n=0,o=0,p=1,m_start, m_end, n_start, n_end, o_start, o_end, number, count=0;
  if (cl.find_first_not_of(" ",1)!=string::npos){ //section to isolate the first integer from the string entered into the command line
    m_start= cl.find_first_not_of(" ",1);
    m_end = cl.find_first_of(" ", m_start)-2;
    M_string=cl.substr(m_start,m_end);
    if (!(istringstream(M_string) >> m)) { //if the substring representing the first integer cannot be converted to an integer, toggle the error state to true
      error_state=true;
      m=1;
    } else {count++;} //increment count to show one integer is present
    if (cl.find_first_not_of(" ", m_end+2)!=string::npos){ //section to isolate the second integer from the string entered into the command line
      n_start = cl.find_first_not_of(" ", m_end+2);
      n_end = cl.find_first_of(" ",n_start);
      N_string=cl.substr(n_start,n_end);
      if (!(istringstream(N_string) >> n)){ //if the substring representing the second integer cannot be converted to an integer, toggle the error state to true
	error_state=true;
      } else {count++;} //increment count to show that two integers are present
      if (cl.find_first_not_of(" ", n_end+1)!=string::npos) { //section to isolate the third integer from the string entered into the command line
	  o_start = cl.find_first_not_of(" ", n_end+1);
	  o_end = cl.find_first_of("",o_start);
	  O_string=cl.substr(o_start,o_end);
	  if (istringstream(O_string) >> o){ //if substring can be converted to an integer, increment count to show a third integer is present
	    count++; 
	  } 
	}
    }
    //SECTION TO MAKE SURE INTEGERS MAKE SENSE
    if (m<1){ //if first integer is less than 1, toggle error state to true
      error_state=true;
    }
    if (n>T.get_last_line()) { //if second int is greater than document length, toggle error state
      error_state=true;
    }
    if (n<1) { //if second it is less than one, toggle error state to true
      error_state=true;
    }
    if (count==3) {
      if (n<m) { //if three integers are present and the first 
	error_state = true;
      }
      if (o<=0) { //if the third integer is less than three, toggle error state to true
	error_state = true;
      }
      if (o>T.get_last_line()) { //if third integer is greater than document length, reset it
	o=T.get_last_line();
      }
    }
    if (!error_state) { 
      if (count==3){ //if three integers are present, copy lines from the start position to the end position to after the given location
	T.copy_lines(m,n,o);
       }
      else if (count==2) { //if two integers are present, copy the line given by the first position to after the position denoted by the second integer
	T.copy_lines(m,m,n);
      }	
      return true; //return true if error state is false
    } else {
      return false; //return false if error state is true
    }
  }
  return false; //return false if nothing is found after the initial 'c' substring in the input string
}

bool move_line (text_ed & T, string cl) {
  bool error_state=false;
  string aline,M_string,N_string,O_string;
  int i = 0,m=0,n=0,o=0,p=1,m_start, m_end, n_start, n_end, o_start, o_end, number, count=0;
  if (cl.find_first_not_of(" ",1)!=string::npos){ //section to isolate the substring representing the first integer from the given string
    m_start= cl.find_first_not_of(" ",1);
    m_end = cl.find_first_of(" ", m_start)-2;
    M_string=cl.substr(m_start,m_end);
    if (!(istringstream(M_string) >> m)) { //if substring cant be converted to an integer, toggle error state to true
      error_state=true;
    } else {count++;} //if it can, increment count to show first integer is present
    if (cl.find_first_not_of(" ", m_end+2)!=string::npos){ //section to isolate the substring representing the second integer from the given string
      n_start = cl.find_first_not_of(" ", m_end+2);
      n_end = cl.find_first_of(" ",n_start);
      N_string=cl.substr(n_start,n_end);
      if (!(istringstream(N_string) >> n)){ //if substring cant be converted to an integer, toggle error state to true
	error_state=true;
      } else {count++;} //if it can be, increment count to show second integer is present
      if (cl.find_first_not_of(" ", n_end+1)!=string::npos) { //section to isolate the substring representing the third integer from the given string
	o_start = cl.find_first_not_of(" ", n_end+1);
	o_end = cl.find_first_of("",o_start);
	O_string=cl.substr(o_start,o_end);
	if (istringstream(O_string) >> o){ //if substring can be converted to an integer, increment count to show third integer is present
	  count++;
	} 
      }
    }
    //SECTION TO MAKE SURE THE INTEGERS MAKE SENSE
    if (m<=0){ //if first integer is less than 1, toggle error state to true
      error_state=true;
    }
    if (count<=2){
      if (n>T.get_last_line()) { //if second int is greater than document length, reset it
	n=T.get_last_line();
      }
      if (n<=0) { //if second it is less than zero, toggle error state to true
	error_state=true;
      }
    }
    if (count==3) {
      if (n<m) { //if three integers are present and the first 
	error_state = true;
      }
      if (o<=0 || o>T.get_last_line()) { //if the third integer is less than three, toggle error state to true
	error_state = true;
      }
    }
    if (!error_state) {
      if (count==3){ //if three integers are present, move the lines from the first to the second integer to after the position given by the third integer
	T.move_lines(m,n,o);
       } else if (count==2) { //if two integers are present, move the first line to after the position given by the second
	T.move_lines(m,m,n); //n+1
      }	else if (count==1) {
	return false;  //return false if only one integer is found
      }
      return true; //if error state is false, return true
    }
  }
  return false; //if error state is true or the first integer isnt found, return false
}

bool find_and_type(text_ed & T, string cl, bool toggle) { //search area from first given line to second for a given word. Type all lines where word is found.
  bool error_state=false;
  string aline,m_string,n_string, word;
  int i = 0,m=0,n=0,p=1,m_begin, m_end, n_begin, n_end,document_length, word_start, word_end,count=0; 
  if (cl.find_first_not_of(" ",1)!=string::npos){ //section to isolate substring representing the first integer from the string
    m_begin= cl.find_first_not_of(" ",1);
    m_end = cl.find_first_of(" ", m_begin)-2;
    m_string=cl.substr(m_begin,m_end);
    if (istringstream(m_string) >> m) { //if substring can be converted to an integer, increment count to show the first integer is present
      count++;}
    if (cl.find_first_not_of(" ", m_end+2)!=string::npos){ //section to isolate substring representing the second integer from the string
      n_begin = cl.find_first_not_of(" ", m_end+2);
      n_end = cl.find_first_of("",n_begin);
      n_string=cl.substr(n_begin,n_end);
      if (istringstream(n_string) >> n){ //if substring can be converted to an integer, increment count to show the second integer is present
	count++;
      }
    }
    //SECTION TO MAKE SURE THE INTEGER VALUES MAKE SENSE
    document_length = T.get_last_line();
    if ((m<=0 || m>document_length) && count!=0){ //if first integer is less than the starting line number toggle error state on
      error_state=true;
    }
    if (n>document_length) { //if ending integer value is greater than the document length, toggle error state on
      error_state=true;
    }
    if (n<m && count==2){ //if the second integer is greater than the first, toggle error state on
      error_state=true;
    }
  }
  //section to isolate the substring representing the word to be searched for from the string
  if (cl.find_first_of("/")!=string::npos){ 
    word_start = cl.find_first_of("/")+1;
    word_end = cl.find_first_of("/",word_start);
    word = cl.substr(word_start,word_end-word_start); //select the correct substring from the string
  } else { //if word can't be found, return false, as there is no point in continuing
    return false;
  }

  if (!error_state) {
    switch (count) {
    case 0: //if no integers are present, search the whole document for the word
      n = T.get_last_line();
      for (i=1; i<=document_length; i++){
	if (T.get_line(aline,i)) {
	  if (aline.find(word)!=string::npos){
	    if (toggle) {  
	      cout << setw(2) << i << " ";
	    } else {
	      cout << " ";
	    }
	    cout << aline << endl;
	  }
	} else {
	  cout << "No line " << i << endl;
	  return false;
	}
      }
      break;
    case 1: //if 1 integer is present, search just that line for the given word
      if (T.get_line(aline,m)) {
	if (aline.find(word)!=string::npos){
	  if (toggle) {  
	    cout << setw(2) << m << " ";
	  } else {
	    cout << " ";
	  }
	  cout << aline << endl;
	} else {
	  cout << "No line " << i << endl;
	  return false;
	}
      }
      break;
    case 2: //if two integers are present, search from the starting int to the ending int for the word
      for (i = m; i<=n; i++){
	if (T.get_line(aline,i)) {
	  if (aline.find(word)!=string::npos){
	    if (toggle) {  
	      cout << setw(2) << i << " ";
	    } else {
	      cout << " ";
	    }
	    cout << aline << endl;
	  }
	} else {
	  cout << "No line " << i << endl;
	  return false;
	}
      }
    }
    return true; //return true if !error state
  } 
  return false; //return true if error state
}

bool find_and_replace(text_ed & T, string cl) { 
  bool error_state=false;
  string aline,m_string,n_string, word, second_word;
  int i = 0,m=0,n=0,p=1,m_begin, m_end,m_length, n_begin, n_end, n_length, document_length, word_start, word_end, second_word_start, second_word_end, count=0;
    //SECTION TO ISOLATE INTEGERS FROM THE GIVEN STRING
  if (cl.find_first_not_of(" ",1)!=string::npos){ //section to isolate first integer from string
    m_begin= cl.find_first_not_of(" ",1);
    m_end = cl.find_first_of(" ", m_begin+1); 
    m_length = m_end-m_begin;
    m_string=cl.substr(m_begin,m_length);
    if (istringstream(m_string) >> m) { //if first substring can be converted to an integer, increment the count to show it is present
      count++;
    }
    if (cl.find_first_not_of(" ", m_end+1)!=string::npos){ //section to isolate second integer from the string
      n_begin = cl.find_first_not_of(" ", m_end+1);
      n_end = cl.find_first_of(" ",n_begin);
      n_length = n_end-n_begin;
      n_string=cl.substr(n_begin,n_length);
      if (istringstream(n_string) >> n){ //if the second substring can be converted to an integer, increment the count to show it is present
	count++;
      }
    }
  }
   //SECTION TO MAKE SURE THE INTEGER VALUES MAKE SENSE
    document_length = T.get_last_line();
    if ((m<=0 || m>document_length)&& count!=0){ //if first integer is less than the starting line number, set it to the starting line number
      error_state=true;
    }
    if (n>document_length) { //if ending integer value is greater than the document length, toggle error state to true
      error_state=true;
    }
    if (n<m && count==2){ //if the second integer is greater than the first, toggle error state on
      error_state=true;
    }

    //SECTION TO ISOLATE WORDS FROM THE STRING
    if (cl.find_first_of("/")!=string::npos){ //portion to isolate first word
      word_start = cl.find_first_of("/")+1;
      word_end = cl.find_first_of("/",word_start);
      word = cl.substr(word_start,word_end-word_start);
    } else {error_state=true;} //if first word can't be found, toggle error state to true
    if (cl.find_first_of("/",word_end+1)!=string::npos){ //portion to isolate the replacement word
      second_word_start = cl.find_first_not_of("/",word_end);
      second_word_end = cl.find_first_of("/",second_word_start);
      second_word = cl.substr(second_word_start, second_word_end-second_word_start);
    } else {error_state=true;} //if second word can't be found, toggle error state to true
    if (!error_state) {
      switch (count) {
      case 0: //if no integers present, find and replace throughout document
	T.replace(word,second_word,1,document_length); //if no integer present, replace old word with new word throughout the document
	break;
      case 1: //if one integer is present, find and replace on just that line
	T.replace(word,second_word,m,m);
	break;
      case 2: //if two integers present, replace old word with new word from line m to line n
	T.replace(word,second_word,m,n);
	break;
      }
      return true;
    } else {
      return false; //if error state is true, return false
    }
  }


bool Do_Command (text_ed & T, bool& toggle) { //
 char command;
 string command_line;
 bool success, filename_presence=false;
 cout << "File is \"" << T.filename() << "\"." << endl; //print out filename
 cout << "--> ";
 getline(cin,command_line);  //get input from command line
 command = command_line[0];
 switch (command) {
 case 'h':
 case 'H': Show_Commands(); break;
 case 'a':
 case 'A': success = add_lines(T,command_line); break;
 case 'q':
 case 'Q': return false;
 case 's':
 case 'S':
   if (command_line.find_first_not_of(" ",1)!=string::npos) { //if filename is present, toggle bool to true
     filename_presence = true;
   } 
   if (filename_presence) { //if filename is present, save file to filename and retrieve new filename
     success = save_file(T,command_line) && get_file(T,command_line);
   } else { //otherwise save the file under the current name
     success = save_file(T,command_line);
   }
   break;
 break;
 case 't':
 case 'T': success = type_lines(T,command_line, toggle); break;
 case 'x':
 case 'X': success = save_file(T,command_line);
	   return false;
	   break;
 case 'g':
 case 'G': success = get_file(T,command_line); break;
 case 'n':
 case 'N': if (!toggle) {
	    toggle=true;
	   } else {
	    toggle=false;
	   }
	   success = true; 
	   break;
 case 'd':
 case 'D': success = delete_line(T,command_line); break;
 case 'c':
 case 'C': success = copy_line(T,command_line); break;
 case 'm':
 case 'M': success = move_line(T,command_line); break;
 case 'f':
 case 'F': success = find_and_type(T,command_line, toggle); break;
 case 'r':
 case 'R': success = find_and_replace(T,command_line); break;
 default: if (command) {
              cout << "\"" << command << "\" is not a valid command." << endl;
      cout << "Type \"h\" for help." << endl;
      success = true;
          } else {
    success = true;
  }
 }
 if ( !success ) cout << "Error" << endl;
 return true;
}

int main (int argc, char** argv) {
 bool toggle=true;  //initiate with line numbering on
 text_ed T;
 if (argc > 1) {
   if (!T.load_file(argv[1]))
     cerr << "Could not open file \"" << argv[1] << "\"." << endl;
 }
 bool keep_going;
 do {
   keep_going = Do_Command(T,toggle);
 } while (keep_going);
}
