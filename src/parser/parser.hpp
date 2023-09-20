// For now it will be PNG parser, but later it will support other filetypes (JPG).

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <bitset>
#include <bit>

class PNG_Parser{
  std::ifstream ifst{nullptr};
  std::vector<std::uint8_t> file;
  // ===================================================================== //
  // constants associated with PNG files
  const unsigned HEADER_SIZE{8};
  // ===================================================================== //
  

  public:
  PNG_Parser() = default;
  void ReadData();
  void PrintData(std::ostream& ost) const;
  std::array<std::uint8_t, 8> GetHeader() const;
  
  // ===================================================================== //
  // FOR DEBUGGING PURPOSES ONLY

  void SetFile(std::string& file_name){
    ifst.open(file_name, std::ios::in);
    if(ifst.rdstate() == std::ios_base::badbit){
      std::cout << "ERROR_OCCURED_IN_READING_FILE::state_is_badbit" << '\n';
    }
    if(ifst.rdstate() == std::ios_base::failbit){
      std::cout << "ERROR_OCCURED_IN_READING_FILE::state_is_failbit" << '\n';
    }
    if(ifst.rdstate() == std::ios_base::eofbit){
      std::cout << "ERROR_OCCURED_IN_READING_FILE::state_is_eofbit" << '\n';
    }

  }

  void SetFile(std::string&& file_name){
    SetFile(file_name);
  }

  // ===================================================================== //
};

#endif
