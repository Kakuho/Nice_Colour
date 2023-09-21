#include "parser.hpp"

// For ChunkPropertyI

bool ChunkPropertyI::operator==(ChunkPropertyI& rhs){
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

//---------------------------------------------------------------//

void PNG_Parser::ReadData(){
  // ASSUMPTION: cursor of ifst points to the last byte in the filestream
  std::size_t number_of_bytes = ifst.tellg();
  ifst.seekg(0);
  // raw buffer to hold all the bytes
  char buffer[number_of_bytes];
  ifst.read(buffer, number_of_bytes);
  // now read the contents of the raw buffer into m_data
  m_data.reserve(number_of_bytes);
  for(std::size_t k{0}; k < number_of_bytes; k++){
    m_data.push_back(static_cast<std::uint8_t>(buffer[k]) );
  }
}

void PNG_Parser::PrintData(std::ostream& ost) const{
  // function useful for debugging, produces a hexdump
  for(std::size_t i = 0; i < m_data.size(); i++){
    if(i % 4 == 0){
      ost << '\t';
    }
    if(i % 16 == 0){
      ost << '\n';
    }
    ost << std::hex << std::setw(2) << std::setfill('0')
        << (static_cast<unsigned int>(m_data[i]) & 0xFF) << ", ";
  }
  ost << '\n';
}

std::array<std::uint8_t, 8> PNG_Parser::GetHeader() const{
  std::array<std::uint8_t, 8> header;
  for(int i = 0; i < HEADER_SIZE; i++){
    header[i] = m_data[i];
  }
  return header;
}

std::vector<std::uint8_t> PNG_Parser::GetFirstChunk() const{
  // 5.3 https://www.w3.org/TR/png-3/#5Introduction
  //---------------------------------------------------------------//
  // A png chunk looks consists of the following parts:
  // chunk = Length + chunktype + chunkdata + CRC
  // The total lenght of the chunk is as such:
  // 4 from Length
  // 4 from chunktype
  // 4 from chunkdata
  // 4 from CRC
  const int start{8}; 
  std::uint32_t length = ConcatenateBytes(
      m_data[start], m_data[start+1], m_data[start+2],  m_data[start+3]);
  std::size_t length_of_chunk{ 4 + 4 + length + 4 };
  // I use a std::vector because I dont know what lenght the vector will be (length segment is 
  // dependent on which png file is being parsed)
  std::vector<std::uint8_t> chunk;
  for(int i = 0; i < length_of_chunk; i++){
    chunk.push_back( m_data[i + start] );
  }
  return chunk;
}


//---------------------------------------------------------------//
// bytes

std::uint32_t PNG_Parser::ConcatenateBytes(std::uint8_t b1, std::uint8_t b2, std::uint8_t b3,
    std::uint8_t b4) const{
  std::uint32_t length = (b1 << 24);
  length |= (b2 << 16);
  length |= (b3 << 8);
  length |= b4;
  return length;
}


//---------------------------------------------------------------//
// functions to fetch data

void PNG_Parser::LoadChunk(){
  // The main function to load a chunk into data
  //---------------------------------------------------------------//
  // 5.3 https://www.w3.org/TR/png-3/#5Introduction
  //---------------------------------------------------------------//
  // A png chunk looks consists of the following parts:
  // chunk = Length + chunktype + chunkdata + CRC
  // The total lenght of the chunk is as such:
  // 4 from Length
  // 4 from chunktype
  // (Length) bytes from chunkdata
  // 4 from CRC
  //Check(ckIndex);
  if(ckIndex == 0){
    // ckIndex is 0 means that the next loaded chunk will be the png signature
    LoadHeader();
    ckIndex += 8;
  }
  if(eof == 1){
    return;
    std::cout << "End of file!" << '\n';
  }
  else{
    chunk.clear();
    // ckIndex is not 0, so next loaded will be a chunk
    std::uint32_t length = ConcatenateBytes(
      m_data[ckIndex], m_data[ckIndex + 1], m_data[ckIndex + 2], m_data[ckIndex + 3]
      );
    std::size_t length_of_chunk{ 4 + 4 + length + 4 };
    // I use a std::vector because I dont know what lenght the vector will be 
    // (length segment is dependent on the length of each chunk)
    for(int i = 0; i < length_of_chunk; i++){
      chunk.push_back( m_data[ckIndex + i] );
    }
    // check for if the length exceeds the data buffer
    if(ckIndex + length_of_chunk > m_data.size()){
      std::cout << "EOF!" << '\n';
      eof = 1;
    }
    else{
      ckIndex += length_of_chunk;
    }
    std::cout << "length of chunk: " << length  
              << " | length of data buffer: " << m_data.size()
              << " | ckIndex: " << ckIndex 
              << " | Chunk Type Byte String: " << std::hex << GetChunkType()
              << " | Chunk Type Decoded: "     << GetChunkTypeName() << '\n';
  }
}

void PNG_Parser::LoadHeader(){
  // Should be get the header
  auto tmp = GetHeader();
  chunk = std::vector<std::uint8_t>{tmp.begin(), tmp.end()};
}

//---------------------------------------------------------------//

ChunkPropertyI PNG_Parser::DecodeChunkProperties() const{
  if(chunk.empty()){
    std::cout << "ERROR_NO_CHUNK_LOADED" << '\n';
    // THROW EXCEPTION!
  }
  else{
    // the least significant bit is bit 0. So a mask of 0b0010'0000 is suitable for all chunks.
    // types are determined by bit 5 of the resepective byte in the chunk header 
    // (thus there is 4 bits, as in ChunkPropertyI
    std::uint8_t mask = 0b0010'0000;
    ChunkPropertyI buffer{
      .ancillary_bit = static_cast<bool>(chunk[0] & mask),
      .private_bit = static_cast<bool>(chunk[1] & mask),
      .reserved_bit = static_cast<bool>(chunk[2] & mask),
      .safe_to_copy_bit = static_cast<bool>(chunk[3] & mask),
    };
    return buffer;
  }
}

std::uint32_t PNG_Parser::GetChunkType() const{
  const int START = 4;
  std::uint32_t type = ConcatenateBytes(chunk[START], chunk[START+1], chunk[START+2],
      chunk[START+3]
      );
  return type;
}

std::string PNG_Parser::GetChunkTypeName() const{
  // Returns a string which contains the name of the chunk.
  std::uint32_t type = GetChunkType();
  switch(type){
    // Critical Chunks
    case 0x49484452:
      return std::string{"IHDR"};
    case 0x504C5445:
      return std::string{"PLTE"};
    case 0x49444154:
      return std::string{"IDAT"};
    case 0x49454E44:
      return std::string{"IEND"};
    // Ancilliarly Chunks
    case 0x6348524D:
      return std::string{"cHRM"};
    case 0x624b4744:
      return std::string{"bKGD"};
    case 0x74455874:
      return std::string{"tEXt"};
    case 0x67414d41:
      return std::string{"gAMA"};
    case 0x70485973:
      return std::string{"pHYS"};
    case 0x74494D45:
      return std::string{"tIME"};
    case 0x7A545874:
      return std::string{"zTXT"};
    default:
      return std::string{"ERROR: There's nothing here OWO"};
  }
}

void PNG_Parser::PrintChunkInformation(std::ostream& ost) const{
  // A Chunk consists of length, type, data and CRC
  // I want to make a nice debugging function for this
}



// DEBUGGING FUNCTIONS

