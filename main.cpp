// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $
// Jeanette Mui, jemui@ucsc.edu

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>

#include <fstream>
#include <typeinfo>
#include <regex> 

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

const string cin_name = "-";
int count {0};

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char** argv) {
   sys_info::execname (argv[0]);
   scan_options (argc, argv);

   str_str_map test;
   int count {1};
   bool check {false};

   // line parsing into key, value, xpair<key,value> 
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex blank_regex {R"(^\s*$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};

   ifstream infile(argv[1]);
   
   for(;;) {
      string line;
      getline(infile, line);

      if(!line.empty() and !infile.fail()){
         // print out test file name
         cout <<  argv[1];
      } 
      else {
         // checks if file exists
         if(check == false ) {
            for(int i = 1; i < argc; ++i) {
               cerr << "keyvalue: " << argv[i] << 
                     ": No such file or directory" << endl; 
               check = true;         
            }
            if(check == true) {
               return EXIT_FAILURE;
            }
         }
         // read from cin
         getline(cin, line);
         cout << cin_name;

      } 

      if(infile.eof()) break;
      
      smatch result; 
      // ignore comment and blank lines
      if(regex_search(line, result, comment_regex) or 
            regex_search(line, result, blank_regex)) {
         cout << ": " << count++ << ": " << line <<endl;
         break;
      }
      // if line is only "=", print out listmap
      else if(line.compare("=") == 0) {
         cout << ": " << count++ << ": " << line <<endl;

         for(str_str_map::iterator itor = test.begin(); 
               itor!=test.end(); ++itor){
            cout << (*itor).first << " = " << (*itor).second << endl;
         }

         cout << (*test.end()).first << " = " 
               << (*test.end()).second << endl;
      } 
      else {
         cout << ": " << count++ << ": " << line <<endl;
         if(regex_search(line, result, key_value_regex)) {

            if(line.find_first_of("=") == 0) {
               // print out pairs that contain a value result[2]
               for(str_str_map::iterator itor = test.begin(); 
                     itor!=test.end(); ++itor){
                  if( (*itor).second == result[2])
                     cout << (*itor).first << " = " 
                           << (*itor).second << endl;
               }
            }
            else {
               // create and insert xpair into listmap 
               str_str_pair pair (result[1], result[2]);
               test.insert(pair);
            }

         } else if(regex_search(line, result, trimmed_regex)) {
            // find key 
            test.find(result[1]);
         }
      }
   }

   str_str_map::iterator itor = test.begin();
   test.erase (itor);

   return EXIT_SUCCESS;
}

