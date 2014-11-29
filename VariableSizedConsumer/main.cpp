#include <iostream>
#include <random>
#include <vector>
#include "SharedBuffer.h"
#include "ConstantParameters.h"
#include "H5Cpp.h"

size_t fillBuffer(std::vector<real_type> &output, SharedBuffer &input, size_t blockCount, size_t startBlock)
  {
  output.resize(BlockElementCount * blockCount);

  for (size_t i = 0; i < blockCount; ++i)
    {
    const size_t realBlock = (startBlock + i) % BufferCount;
    const size_t element = realBlock * BlockElementCount;

    // Lock and write our block to the file.
    TypedLockedData<real_type> lockedData(&input, element, BlockElementCount);
    auto data = lockedData.data();


    }

  return blockCount;
  }

int comsumeData(const char *name, const char *outputName)
  {
  // Create a shared buffer called [sharedMemoryName], large enough to hold [count] * [bufferCount] real_type's.
  SharedBuffer buffer(name);

  static_assert(
      std::is_same<real_type, float>::value,
      "Need to adjust the PredType if real_type changes");

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> rand(3, 25);

  // Initialise the properties for our data set
  H5::DSetCreatPropList plist;
  // Fill with zero by default.
  float fillvalue = 0.0;
  plist.setFillValue(H5::PredType::NATIVE_FLOAT, &fillvalue);

  // Used to represent the current block we are reading from.
  size_t blockPosition = 0;

  std::vector<real_type> localBuffer;

  for (size_t i = 0; i < FileCount; ++i)
    {
    // Find how many blocks to save - between 3 and 25.
    size_t blocksToSave = rand(gen);

    // Ensure the local buffer can fit the data
    blockPosition += fillBuffer(localBuffer, buffer, blocksToSave, blockPosition);

    H5::H5File file(outputName, H5F_ACC_TRUNC);

    // Create a dataset with [count] rows.
    hsize_t fdim[] = { BufferCount };
    H5::DataSpace fspace( 1, fdim );

    // Create our new dataset - filled with xero by default.
    H5::DataSet dataset = file.createDataSet(
        "RandomData",
        H5::PredType::NATIVE_FLOAT,
        fspace,
        plist);

    dataset.write(localBuffer.data(), H5::PredType::NATIVE_FLOAT);
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

  comsumeData(argv[1], argv[2]);
  return 1;
  }
