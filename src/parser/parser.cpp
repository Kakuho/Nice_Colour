#include "parser.hpp"

// For ChunkTypeI

bool ChunkTypeI::operator==(ChunkTypeI& rhs){
  if( ancillary_bit == rhs.ancillary_bit
      && private_bit == rhs.private_bit
      && reserved_bit == rhs.reserved_bit
      && safe_to_copy_bit == rhs.safe_to_copy_bit){
    return true;
  }
  else{
    return false;
  }
}

// ===================================================================== //

void PNG_Parser::ReadData(){
  std::size_t number_of_bytes = ifst.tellg();
  ifst.seekg(0);
  file.resize(number_of_bytes);
  ifst.read(file.data(), number_of_bytes);
}

void PNG_Parser::PrintData(std::ostream& ost) const{
  // function useful for debugging, produces a hexdump
  for(std::size_t i = 0; i < file.size(); i++){
    if(i % 4 == 0){
      ost << '\t';
    }
    if(i % 16 == 0){
      ost << '\n';
    }
    ost << std::hex << std::setw(2) << std::setfill('0')
        << (static_cast<unsigned int>(file[i]) & 0xFF) << ", ";
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

std::vector<std::uint8_t> PNG_Parser::GetFirstChunk() const{
  // 5.3 https://www.w3.org/TR/png-3/#5Introduction
  // A png chunk looks consists of the following parts:
  // chunk = Length + chunktype + chunkdata + CRC
  // The total lenght of the chunk is as such:
  // 4 from Length
  // 4 from chunktype
  // 4 from chunkdata
  // 4 from CRC
  const int start{8}; 
  std::uint32_t length{
    file[start]     << 12  |
    file[start + 1] << 8   |
    file[start + 2] << 4   |
    file[start + 3]
  };
  std::size_t length_of_chunk{ 4 + 4 + length + 4 };
  // I use a std::vector because I dont know what lenght the vector will be (length segment is 
  // dependent on which png file is being parsed)
  std::vector<std::uint8_t> chunk;
  for(int i = 0; i < length_of_chunk; i++){
    chunk.push_back( file[i + start] );
  }
  return chunk;
}

ChunkTypeI PNG_Parser::DecodeChunkType() const{
  if(chunk.empty()){
    std::cout << "ERROR_NO_CHUNK_LOADED" << '\n';
  }
  else{
    // the least significant bit is bit 0. So a mask of 0b0010'0000 is suitable for all chunks.
    // types are determined by bit 5 of the resepective byte in the chunk header 
    // (thus there is 4 bits, as in ChunkTypeI
    std::uint8_t mask = 0b0010'0000;
    ChunkTypeI buffer{
      chunk[0] & mask,
      chunk[1] & mask,
      chunk[2] & mask,
      chunk[3] & mask,
    };
    return buffer;
  }
}

// DEBUGGING FUNCTIONS

