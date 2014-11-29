#include <iostream>
#include "SharedBuffer.h"
#include "ConstantParameters.h"
#include "H5Cpp.h"

int comsumeData(const char *name, const char *outputName)
  {
  // Create a shared buffer called [sharedMemoryName], large enough to hold [count] * [bufferCount] real_type's.
  SharedBuffer buffer(name);

  H5::H5File file(outputName, H5F_ACC_TRUNC);

  // Initialise the properties for our data set
  H5::DSetCreatPropList plist;
  // Fill with zero by default.
  float fillvalue = 0.0;
  plist.setFillValue(H5::PredType::NATIVE_FLOAT, &fillvalue);

  // Create a dataset with [count] rows.
  hsize_t fdim[] = { BlockElementCount * BufferCount };
  H5::DataSpace fspace( 1, fdim );

  static_assert(
      std::is_same<real_type, float>::value,
      "Need to adjust the PredType if real_type changes");

  // Create our new dataset - filled with xero by default.
  H5::DataSet dataset = file.createDataSet(
      "RandomData",
      H5::PredType::NATIVE_FLOAT,
      fspace,
      plist);

  // Lock and write our data to the file.
  TypedLockedData<real_type> lockedData(&buffer, 0, BufferSize);
  auto data = lockedData.constData();

  dataset.write(data, H5::PredType::NATIVE_FLOAT);

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
