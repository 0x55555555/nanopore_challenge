#include <iostream>
#include "SharedBuffer.h"
#include "ConstantParameters.h"
#include "H5Cpp.h"

int transformData(const char *nameIn, const char *nameOut)
  {
  // Create a shared buffer called [sharedMemoryName], large enough to hold [count] * [bufferCount] real_type's.
  SharedBuffer bufferIn(nameIn);

  // Create a shared buffer called [nameOut], for the transformed values.
  SharedBuffer::remove(nameOut);
  SharedBuffer bufferOut(
        nameOut,
        BufferCount * BlockElementCount * sizeof(real_type));

  // Lock and write our data to the file.
  TypedLockedData<real_type> lockedDataIn(&bufferIn, 0, BufferSize);
  auto dataIn = lockedDataIn.data();

  // Lock and write our data to the file.
  TypedLockedData<real_type> lockedDataOut(&bufferOut, 0, BufferSize);
  auto dataOut = lockedDataOut.data();

  return 1;
  }



int main(int argc, char *argv[])
  {
  if (argc != 3)
    {
    std::cerr << "Incorrect number of arguments passed - expected 3, got " << argc;
    return 0;
    }

  transformData(argv[1], argv[2]);
  return 1;
  }
