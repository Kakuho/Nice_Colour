// For now it will be PNG parser, but later it will support other filetypes (JPG).

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <bitset>
#include <bit>

struct ChunkTypeI{
  // A datastruct to hold the information of a chunk type
  // 5.4 https://www.w3.org/TR/png-3/#5Chunk-naming-conventions
  // ancillarly   - determines critical chunk(1) or ancillarly chunk(0)
  // private      - public defs are reserved by W3C
  // rsereved     - it must be 0, otherwise it does not follow png
  // safe-to-copy - "not of interest to decoders"
  bool ancillary_bit;
  bool private_bit;
  bool reserved_bit;
  bool safe_to_copy_bit;
  bool operator==(ChunkTypeI& rhs);
};

class PNG_Parser{
  // type alias to represent intent better
  using byte = char;
  std::ifstream ifst{nullptr};
  std::vector<byte> file;
  std::vector<byte> chunk;
  const std::size_t chunkIndex{8};

  // constants associated with PNG files

  const unsigned HEADER_SIZE{8};

  public:
  PNG_Parser() = default;
  void ReadData();
  void ReadChunk();
  // setters
  void SetChunk(std::vector<byte>& src){ chunk = src; }
  void SetChunk(std::vector<byte>&& src){ SetChunk(src); }
  // getters
  std::array<std::uint8_t, 8> GetHeader() const;
  std::vector<std::uint8_t> GetFirstChunk() const;
  // decoder functions
  ChunkTypeI DecodeChunkType() const;
  void DecodeIHDR();
  void DecodePLTE();
  void DecodeIDAT();
  void DecodeIEND();
  // output data
  void PrintData(std::ostream& ost) const;
  
  // FOR DEBUGGING PURPOSES ONLY

  void SetFile(std::string& file_name){
    ifst.open(file_name, std::ios::binary | std::ios::ate);
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

};

#endif
