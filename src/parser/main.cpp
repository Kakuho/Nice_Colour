#include "parser.hpp"
#include <cassert>

// tests for my parser

template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& ost, std::array<T, N> arr){
  for(std::size_t i = 0; i < N; i++){
    ost << arr[i] << '\n';
  }
  return ost;
}

void test_opening(){
  PNG_Parser pp;
  pp.SetFile("./../../assets/vaporeon.png");
}

void test_read(){
  PNG_Parser pp;
  // the hardcoded relative file path is painful :(
  pp.SetFile("./../../assets/vaporeon.png");
  pp.ReadData();
  pp.PrintData(std::cout);
}

void test_header(){
  // headers must have the following!
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
}

void test_chunk_header_decode(){
  PNG_Parser pp;
  // Example is cHNK = 0x63, 0x48, 0x4E, 0x6B
  pp.SetChunk(std::vector<char>{0x63, 0x48, 0x4E, 0x6B});
  ChunkTypeI sample1{1, 0, 0, 1};
  assert(pp.DecodeChunkType() == sample1);
}

int main(){
  test_chunk_header_decode();
}
