#include <iostream>
#include "SharedBuffer.h"
#include "ConstantParameters.h"
#include "GeneratedBufferArray.h"
#include "H5Cpp.h"

int transformData(const char *nameIn, const char *nameOut)
  {
  // Create a shared buffer called [sharedMemoryName], large enough to hold [count] * [bufferCount] real_type's.
  TypedSharedBuffer<GeneratedBufferArray> bufferIn(nameIn, boost::interprocess::open_only);

  // Create a shared buffer called [nameOut], for the transformed values.
  SharedBuffer::remove(nameOut);
  TypedSharedBuffer<GeneratedBufferArray> bufferOut(nameOut, boost::interprocess::open_or_create);
  bufferOut.initialise();

  // Transform the data to a stream between 0 and 1.
  auto transformer = [](real_type t)
    {
    // scale to 0 -> 1000.0;
    float between0and1000 = 0.5f * (t + 1000.0f);

    // mod it to between 0 -> 1
    return fmod(between0and1000, 1.0f);
    };

  std::cout << "Transforming data from " << nameIn << " to " << nameOut << std::endl;

  // Lock and write our data to the file.
  for (size_t i = 0; i < bufferIn.data()->size(); ++i)
    {
    std::cout << "  Transforming block " << i << std::endl;

    // Lock and write our data to the file.
    LockedData lockedDataIn(&bufferIn.data()->at(i), LockedData::ConstOnly);
    auto dataIn = lockedDataIn.constData();

    // Lock and write our data to the file.
    LockedData lockedDataOut(&bufferOut.data()->at(i));
    auto dataOut = lockedDataOut.data();

    // Transform the data in [dataIn] into the data in [dataOut]
    std::transform(
      dataIn->begin(),
      dataIn->end(),
      dataOut->begin(),
      transformer);
    }

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
