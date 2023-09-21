// For now it will be PNG parser, but later it will support other filetypes (JPG).

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <bitset>
#include <bit>

struct ChunkPropertyI{
  // A datastruct to hold the information of a chunk type's properties
  // 5.4 https://www.w3.org/TR/png-3/#5Chunk-naming-conventions
  // ancillarly   - determines critical chunk(1) or ancillarly chunk(0)
  // private      - public defs are reserved by W3C
  // rsereved     - it must be 0, otherwise it does not follow png
  // safe-to-copy - "not of interest to decoders"
  bool ancillary_bit;
  bool private_bit;
  bool reserved_bit;
  bool safe_to_copy_bit;
  bool operator==(ChunkPropertyI& rhs);
};

//---------------------------------------------------------------//

class PNG_Parser{
  std::ifstream ifst{ nullptr };          // pointer to the filestream
  std::vector<std::uint8_t> m_data;       // vector holds all the bytes
  std::vector<std::uint8_t> chunk;        // chunk is a data struct defined in the spec
  std::size_t ckIndex{0};
  bool eof = 0;
  // constants associated with PNG files
  const unsigned HEADER_SIZE{8};
  // flags for colortypes
  //enum class ColorType{Greyscale, TrueColour, Indexed-Colour, GreyscaleAlpha, TrueColourAlpha};


  public:
  // Initialisation - TBF
  PNG_Parser() = default;
  void ReadData();
  // getters
  std::array<std::uint8_t, 8> GetHeader() const;
  std::vector<std::uint8_t> GetFirstChunk() const;
  std::vector<std::uint8_t> GetChunk() const{ return chunk; }
  bool GetEOF() const{ return eof;}
  // Byte level
  std::uint32_t ConcatenateBytes(std::uint8_t b1, std::uint8_t b2, std::uint8_t b3, std::uint8_t b4)
    const;
  // functions to fetch data
  void LoadChunk();
  void LoadHeader();
  void Check();
  // decoder functions
  struct ChunkTypeWrap{
    // struct to hold a Chunk which will consist of the name and the handler
    std::string name;
    void(*handler)() = nullptr;
  };

  ChunkPropertyI DecodeChunkProperties() const;
  std::uint32_t GetChunkType() const;
  std::string GetChunkTypeName() const;
  void PrintChunkInformation(std::ostream& ost) const;
  void DecodeIHDR();
  void DecodePLTE();
  void DecodeIDAT();
  void DecodeIEND();
  // output data
  void PrintData(std::ostream& ost) const;
  //---------------------------------------------------------------//
  // MISC functions - should be used for debgging only
  bool FileGoodState() const{return ifst.rdstate() == std::ios_base::goodbit?1:0;}
  void SetChunk(std::vector<std::uint8_t>& src){ chunk = src; }
  void SetChunk(std::vector<std::uint8_t>&& src){ SetChunk(src); }
  void SetData(std::vector<std::uint8_t>& src){ m_data = src; }
  void SetData(std::vector<std::uint8_t>&& src){ SetData(src); }
  // I dont know what way is best to initialise, so i will assume a transient 
  // state 
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
