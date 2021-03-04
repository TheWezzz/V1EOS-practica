#include <iostream>
#include <string>

std::string translate(std::string line, std::string append_line)
{ std::string result = line; // implementeer dit
  result.append(" ");
  result.append(append_line);
  return result; }

int main(int argc, char *argv[])
{ std::string input;

  if(argc != 2)
  { std::cerr << "Deze functie heeft exact 1 argument nodig" << std::endl;
    return -1; }

  while(true){
    std::getline(std::cin, input);
    if(input == "EOF"){
      break;
    }
  std::cout << translate(input, argv[1]) << std::endl;}
  return 0; }
