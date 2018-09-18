#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
  streampos size;
  char * memblock;

  ifstream file ("../testdata.raw", ios::in|ios::binary|ios::ate);
  if (file.is_open())
  {
    size = file.tellg();
    cout << "size = " << size << "B" << endl;
    memblock = new char [size];
    file.seekg (0, ios::beg);
    file.read (memblock, size);
    file.close();

    cout << "the entire file content is in memory" << endl;
    cout << hex << (int)memblock[2];

    delete[] memblock;
  }
  else cout << "Unable to open file";
  return 0;
}