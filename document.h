//Craig Barstow
//Document.h
//Header file for text_ed class

class text_ed {
 private:
  string *document; // Array of strings, one for each line
  string *buffer;   // A scratch area; makes copy and move easier

  int last_line,buffer_size,capacity;

  string current_file; // The name of the file being edited

  bool copy_to_buffer(int start, int end);
     // Copy lines start through end to the buffer.
  bool copy_to_document(int start, int end, int location);
     // Copy the entire buffer to the document starting at line start.
  bool open_up(int start, int num_lines);
     // Create num_lines empty lines following start.
  void expand_document(); 
     // Add more lines to the document array, e.g., double capacity.

 public:
  text_ed();
  text_ed(text_ed&);
  text_ed& operator=(text_ed&);
  ~text_ed();
  string filename();
     // Return current_file.
  int get_last_line();
     // Return last_line.
  bool load_file(string filename);  
     // Read the file and remember the name in current_file.
  bool save_file();
     //function to check if saving to a given filename is possible and save it
  bool save_file(string filename);  
     // Save the text under the new name; remember the new name 
     // in current_file.
  bool insert_line(string newline,int n);
     // Insert newline as a new line following line n.
  bool delete_lines(int start, int end);
     // Delete lines from start position to end position.
  bool get_line(string& dest, int n);
     // Copy line n into dest.
  bool copy_lines(int start, int end, int location);
     // Copy lines start through end to follow line location.
  bool move_lines(int start, int end, int location);
     // Move lines start through end to follow line location.
  bool replace(string oldstr, string newstr, int start, int end);
     // Replace oldstr with newstr on lines start through end.
};
