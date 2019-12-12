//
// Created by shiraz on 12.12.2019.
//
#include <unordered_map>
#include <list>
#include "Command.h"
#include "VarClass.h"
#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#define SPACE " "
#define OPEN_BRACKETS '('
#define COMMA ','
using namespace std;
class FlightSimulator {
 private:
  unordered_map<string, Command*> commands;
  unordered_map<string, VarClass*> symbolTableSimulator;
  unordered_map<string, VarClass*> symbolTableProgram;
 public:
  FlightSimulator(){};
  queue<string> lexer(string file_name) {
    queue<string> token;
    string line;
    ifstream f;
    f.open(file_name, ios::in);
    //reading line by line
    while (getline(f, line)) {
      //dispose tabs
      size_t first = line.find_first_not_of('\t');
      if (string::npos != first){
        size_t last = line.size();
        line =  line.substr(first, line.size() - first+1);
      }
      createQueue(token, line);
    }
    f.close();
    return token;
  }
  void createQueue(queue<string> &token, string line) {
    string pusher;
    string even = "";
    int index_comma = 0, index_bracket = 0, index_space = 0, npos = (int) std::string::npos;
    //separate by brackets
    if ((index_bracket = line.find(OPEN_BRACKETS)) != npos) {
      //until the (
      string part_by_brackets = line.substr(0, index_bracket);
      even = part_by_brackets[index_bracket - 2];
      //separated by space
      separateBySpaces(token, index_space, npos, part_by_brackets);
      //in the ()
      if (even.compare("=") == 0) {
        string inside_brackets = line.substr(index_bracket, line.size() - part_by_brackets.size());
        pusher = inside_brackets;
        token.push(pusher);
        cout << pusher << endl;
      } else {
        string inside_brackets = line.substr(index_bracket + 1, line.size() - part_by_brackets.size() - 2);
        //separate by ','
        separateByComma(token, index_comma, npos, inside_brackets);
      }
      line.erase(0, index_space + 1);
    }
    //there is no bracket
    if ((index_bracket = line.find(OPEN_BRACKETS)) == npos) {
      separateBySpaces(token, index_space, npos, line);
      line.erase(0, index_bracket + 1);
    }
  }
  void separateBySpaces(queue<string> &token, int index_space, int npos, string part_by_brackets) {
    string pusher = "";
    //separate by space
    while ((index_space = part_by_brackets.find(SPACE)) != npos) {
      if (pusher.compare("=") == 0) {
        pusher = part_by_brackets;
        token.push(pusher);
        cout << pusher << endl;
        part_by_brackets.erase(0, index_space + 1);
        return;
      }
      string part_by_space = part_by_brackets.substr(0, index_space);
      pusher = part_by_space;
      token.push(pusher);
      cout << pusher << endl;
      part_by_brackets.erase(0, index_space + 1);
    }
    //there is no space
    if ((index_space = part_by_brackets.find(SPACE)) == npos) {
      pusher = part_by_brackets;
      token.push(pusher);
      cout << pusher << endl;
      part_by_brackets.erase(0, index_space + 1);
    }
  }
  void separateByComma(queue<string> &token, int index_comma, int npos, string part_by_brackets) {
    string pusher;
    while ((index_comma = part_by_brackets.find(COMMA)) != npos) {
      pusher = part_by_brackets.substr(0, index_comma);
      token.push(pusher);
      cout << pusher << endl;
      part_by_brackets.erase(0, index_comma + 1);
    }
    // there is no ','
    if ((index_comma = part_by_brackets.find(COMMA)) == npos) {
      pusher = part_by_brackets;
      token.push(pusher);
      cout << pusher << endl;
      part_by_brackets.erase(0, index_comma + 1);
    }
  }
  void parser(queue<string> queue);

};
