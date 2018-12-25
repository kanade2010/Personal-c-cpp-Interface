#include <vector>
#include <iostream>

int main()
{
  std::vector<unsigned char> buffer;

  unsigned char frame[] = {0x00,0x01,0x02,0x03,0x04};

  printf("%02x\n", frame[4]);
  std::cout << frame[4] << "  " << buffer.size() << "\n";

  buffer.assign(frame, frame + 5);
  printf("%02x\n", buffer[4]);
  std::cout << buffer[0] << "  " << buffer[4] << "  " << buffer.size() << "\n";


  std::vector<unsigned char> buffer1(frame, frame + 5);

  std::cout << buffer1.size() << "\n";

  return 0;
}