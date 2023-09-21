#include "parser.hpp"
#include <cassert>

// useful operator overload for debugging

template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& ost, std::array<T, N>& arr){
  for(std::size_t i = 0; i < arr.size(); i++){
    ost << arr[i] << '\n';
  }
  return ost;
}


template<typename T>
std::ostream& operator<<(std::ostream& ost, std::vector<T>& vec){
  for(std::size_t i = 0; i < vec.size(); i++){
    ost << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(vec[i]) << ' ';
  }
  ost << '\n';
  return ost;
}

// ===================================================================== //

// tests for my parser

bool test_opening(){
  PNG_Parser pp;
  pp.SetFile("./../../assets/vaporeon.png");
  assert(pp.FileGoodState() == 1);
  return 1;
}

void test_read(){
  PNG_Parser pp;
  // the hardcoded relative file path is painful :(
  pp.SetFile("./../../assets/vaporeon.png");

  pp.ReadData();
  pp.PrintData(std::cout);
}

bool test_header(){
  // the png signature must contain the following byte string
  // 89 50 4E 47 0D 0A 1A 0A
  PNG_Parser pp;
  pp.SetFile("./../../assets/vaporeon.png");
  pp.ReadData();
  std::array<std::uint8_t, 8> header = pp.GetHeader();
  assert(header[0] == 0x89);
  assert(header[1] == 0x50);
  assert(header[2] == 0x4E);
  assert(header[3] == 0x47);
  assert(header[4] == 0x0D);
  assert(header[5] == 0x0A);
  assert(header[6] == 0x1A);
  assert(header[7] == 0x0A);

  std::cout << "Header read successfully :D" << '\n';
  return 1;
}

bool test_chunk_header_decode(){
  PNG_Parser pp;
  // Example is cHNK = 0x63, 0x48, 0x4E, 0x6B
  pp.SetChunk( std::vector<std::uint8_t>{0x63, 0x48, 0x4E, 0x6B} );
  ChunkPropertyI sample1{1, 0, 0, 1};
  assert(pp.DecodeChunkProperties() == sample1);
  return 1;
}

bool test_load_chunk(){
  PNG_Parser pp;
  // Example:
  // https://en.wikipedia.org/wiki/PNG#Examples
  // 
  // Order of operation:
  //
  // 1) First should read the png signature
  // 2) Then it should read the IHDR chunk 
  // 3) Finally, it should read the IDAT chunk
  //
  //
  // 89 50 4E 47 0D 0A 1A 0A                                                  
  // 00 00 00 0D 49 48 44 52 00 00 00 01 00 00 00 01 08 02 00 00 00 90 77 53 DE
  // 00 00 00 0C 49 44 41 54 08 D7 63 F8 CF C0 00 00 03 01 01 00 18 DD 8D B0
  pp.SetData( std::vector<std::uint8_t>{
      0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,
      0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x08, 0x02, 0x00, 0x00, 0x00, 0x90, 0x77, 0x53, 0xDE, 
      0x00, 0x00, 0x00, 0x0C, 0x49, 0x44, 0x41, 0x54, 0x08, 0xD7, 0x63, 0xF8, 0xCF, 0xC0, 0x00, 0x00, 0x03, 0x01, 0x01, 0x00, 0x18, 0xDD, 0x8D, 0xB0
      } );
  pp.PrintData(std::cout);
  // First load, should fetch the header
  pp.LoadChunk();
  auto chunk = pp.GetChunk();
  assert(chunk[0] == 0x89);
  assert(chunk[1] == 0x50);
  assert(chunk[2] == 0x4E);
  assert(chunk[3] == 0x47);
  assert(chunk[4] == 0x0D);
  assert(chunk[5] == 0x0A);
  assert(chunk[6] == 0x1A);
  assert(chunk[7] == 0x0A);
  // Second Load
  pp.LoadChunk();
  chunk = pp.GetChunk();
  std::cout << chunk << '\n';
  // third Load
  pp.LoadChunk();
  chunk = pp.GetChunk();
  std::cout << chunk << '\n';
  return 1;
}

void test_remi(){
  PNG_Parser pp;
  // ===================================================================== //
  // preamble
  pp.SetFile("./../../assets/vaporeon.png");
  pp.ReadData();
  // ===================================================================== //
  for(std::size_t k{0}; k < 50; k++){
    if( !pp.GetEOF() ){
      pp.LoadChunk();
      auto chunk = pp.GetChunk();
      //std::cout << chunk << '\n';
    }
  }
}

void test_length_chunk(){
  PNG_Parser pp;
}


// Massive tests, run to have confidence and gamification :D
void test_suite(){
  std::size_t sum{0};
  sum += test_opening();
  std::cout << sum << " passed\n";
  sum += test_header();
  std::cout << sum << " passed\n";
  sum += test_chunk_header_decode();
  std::cout << sum << " passed\n";
}

int main(){
  test_remi();
}
