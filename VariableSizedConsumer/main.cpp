#include <iostream>
#include <vector>
#include <random>
#include "SharedBuffer.h"
#include "ConstantParameters.h"
#include "GeneratedBufferArray.h"
#include "Hdf5Utils.h"
#include "H5Cpp.h"

int comsumeData(const char *name, const char *outputName, size_t blockCount)
  {
  // Create a shared buffer called [sharedMemoryName], large enough to hold [count] * [bufferCount] real_type's.
  TypedSharedBuffer<GeneratedBufferArray> buffer(name, boost::interprocess::open_only);

  // Store an id for the last revisions used when copying from the source buffers
  std::vector<size_t> revision(BufferCount, std::numeric_limits<size_t>::max());

  // Generated arrays for output data
  std::vector<GeneratedBuffer> outputElements(blockCount);

  std::cout << "Creating local data copy" << outputName << std::endl;

  // Lock and write our data to the file.
  for (size_t i = 0; i < blockCount; ++i)
    {
    std::cout << "  Constructing block " << i << std::endl;

    // Find the block we want to read from.
    size_t blockIndex = i % BufferCount;

    // Lock access to the data in the shared buffer.
    auto &inputBlock = buffer.data()->at(blockIndex);
    auto &outputBlock = outputElements[i];

    // Find a new piece of data in inputBlock
    LockedData lockedInput(&inputBlock, LockedData::ConstOnly);
    if (revision[blockIndex] == inputBlock.revision())
      {
      // This will block until revision is increased.
      inputBlock.waitForChange(&lockedInput);
      }
    revision[blockIndex] = inputBlock.revision();

    LockedData lockedOutput(&outputBlock);
    *lockedOutput.data() = *lockedInput.constData();
    }

  // Now save our generated data to file.
  H5::H5File file(outputName, H5F_ACC_TRUNC);
  H5::DataSet dataset = createBufferHdfDataSet(file, "RandomData", blockCount);

  std::cout << "Writing data to file " << outputName << std::endl;
  storeBufferToDataSet(dataset, outputElements);

  return 1;
  }

int main(int argc, char *argv[])
  {
  if (argc != 3)
    {
    std::cerr << "Incorrect number of arguments passed - expected 3, got " << argc;
    return 0;
    }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> rand(3, 25);

  // Consume a random (3 -> 25) amount of data.
  comsumeData(argv[1], argv[2], rand(gen));
  return 1;
  }
