#include "parse.hpp"
#include <iostream>

int main() {
    std::vector<std::string> output = readFile("note.txt");
    std::vector<std::vector<float>> sheet = parseSheet(output);
    for (auto s : sheet) {
      std::cout<<s.size()<<' ';
      for (auto q : s) {
        std::cout<<q<<' ';
      }
      std::cout<<std::endl;
    }
  }
