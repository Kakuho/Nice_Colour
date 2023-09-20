#include "parser.hpp"

void PNG_Parser::ReadData(){
  // need to read binary data into my program from the png file
  uint8_t ch;
  while(ifst >> ch){
    file.push_back(ch);
  }
}

void PNG_Parser::PrintData(std::ostream& ost) const{
  for(std::size_t i = 0; i < file.size(); i++){
    if(i%16 == 0){
      ost << '\n';
    }
    if(i % 4 == 0){
      ost << '\t';
    }
    ost << std::hex << std::setw(2) << std::setfill('0')
        << (static_cast<int>(file[i]) & 0xFF) << ", ";
  }
  ost << '\n';
}

std::array<std::uint8_t, 8> PNG_Parser::GetHeader() const{
  std::array<std::uint8_t, 8> header;
  for(int i = 0; i < HEADER_SIZE; i++){
    header[i] = file[i];
  }
  return header;
}



// ===================================================================== //
// DEBUGGING FUNCTIONS


// ===================================================================== //

