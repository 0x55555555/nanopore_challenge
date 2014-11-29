#include "Hdf5Utils.h"

H5::DataSet createBufferHdfDataSet(H5::H5File file, const char *name, size_t bufferCount)
  {
  // Initialise the properties for our data set
  H5::DSetCreatPropList plist;
  // Fill with zero by default.
  float fillvalue = 0.0;
  plist.setFillValue(H5::PredType::NATIVE_FLOAT, &fillvalue);

  // Create a dataset with [count] rows.
  hsize_t fdim[] = { BlockElementCount, bufferCount };
  H5::DataSpace fspace( 2, fdim );

  static_assert(
      std::is_same<real_type, float>::value,
      "Need to adjust the PredType if real_type changes");

  // Create our new dataset - filled with xero by default.
  H5::DataSet dataset = file.createDataSet(
        name,
        H5::PredType::NATIVE_FLOAT,
        fspace,
        plist);

  return dataset;
  }
