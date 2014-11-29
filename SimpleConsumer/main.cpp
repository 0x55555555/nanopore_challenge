#include <iostream>
#include "SharedBuffer.h"
#include "ConstantParameters.h"
#include "GeneratedBufferArray.h"
#include "H5Cpp.h"

int comsumeData(const char *name, const char *outputName)
  {
  // Create a shared buffer called [sharedMemoryName], large enough to hold [count] * [bufferCount] real_type's.
  TypedSharedBuffer<GeneratedBufferArray> buffer(name, boost::interprocess::open_only);

  H5::H5File file(outputName, H5F_ACC_TRUNC);

  // Initialise the properties for our data set
  H5::DSetCreatPropList plist;
  // Fill with zero by default.
  float fillvalue = 0.0;
  plist.setFillValue(H5::PredType::NATIVE_FLOAT, &fillvalue);

  std::cout << "Writing data to file " << outputName << std::endl;

  // Create a dataset with [count] rows.
  hsize_t fdim[] = { BlockElementCount, buffer.data()->size() };
  H5::DataSpace fspace( 2, fdim );

  static_assert(
      std::is_same<real_type, float>::value,
      "Need to adjust the PredType if real_type changes");

  // Create our new dataset - filled with xero by default.
  H5::DataSet dataset = file.createDataSet(
      "RandomData",
      H5::PredType::NATIVE_FLOAT,
      fspace,
      plist);

  H5::DataSpace writespace( 2, fdim );
  hsize_t start[2] = { 0, 0 }; // Start of hyperslab
  hsize_t block[2] = { BlockElementCount, 1 };  // Block sizes

  // Lock and write our data to the file.
  for (size_t i = 0; i < buffer.data()->size(); ++i)
    {
    std::cout << "  Writing block " << i << std::endl;
    
    GeneratedBuffer *genBuffer = buffer.data()->at(i);
    LockedData lockedData(genBuffer);
    auto data = lockedData.constData();

    H5::DataSpace readspace = genBuffer->createDataSpace();

    start[1] = i;
    writespace.selectHyperslab(H5S_SELECT_SET, block, start);

    // Write the section in to the selected portion of the file
    dataset.write(data->data(), H5::PredType::NATIVE_FLOAT, readspace, writespace);
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
