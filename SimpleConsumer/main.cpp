#include <iostream>
#include "SharedBuffer.h"
#include "ConstantParameters.h"
#include "GeneratedBufferArray.h"
#include "Hdf5Utils.h"
#include "H5Cpp.h"

int comsumeData(const char *name, const char *outputName)
  {
  // Create a shared buffer called [sharedMemoryName], large enough to hold [count] * [bufferCount] real_type's.
  TypedSharedBuffer<GeneratedBufferArray> buffer(name, boost::interprocess::open_only);

  H5::H5File file(outputName, H5F_ACC_TRUNC);
  H5::DataSet dataset = createBufferHdfDataSet(file, "RandomData", buffer.data()->size());

  std::cout << "Writing data to file " << outputName << std::endl;
  storeBufferToDataSet(dataset, *buffer.data());

  return 1;
  }

int main(int argc, char *argv[])
  {
  if (argc != 3)
    {
    std::cerr << "Incorrect number of arguments passed - expected 3, got " << argc;
    return 0;
    }

  comsumeData(argv[1], argv[2]);
  return 1;
  }
