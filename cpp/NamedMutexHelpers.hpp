#pragma once
#include <string>
#include <regex>

std::string getSafeNamedMutexName(const char* a_name){
  const char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
  std::string result = "";
  while(*a_name) {
    if ( *a_name == '#' ||
         *a_name == '\\' ||
         *a_name == '/' ||
         *a_name == ':' ||
         *a_name == '<' ||
         *a_name == '>' ||
         *a_name == '"' ||
         *a_name == '?' ||
         *a_name == '*' ||
         *a_name == '|'
       ) {
      result += '#';
      result += hex[*a_name & 0xf0 >> 4];
      result += hex[*a_name & 0x0f];
      result += ';';
    } else {
      result += *a_name;
    }
    ++a_name;
  }
  return result;
}
