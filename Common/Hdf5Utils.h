#pragma once
#include "H5Cpp.h"
#include "ConstantParameters.h"
#include "GeneratedBuffer.h"
#include <iostream>

/// \brief Create a dataset capable of holding [bufferCount] GeneratedBuffer's.
/// \param file The file to create inside
/// \param bufferCount The number of buffers
/// \return The dataset with space for the new data.
H5::DataSet createBufferHdfDataSet(H5::H5File file, const char *name, size_t bufferCount);


/// \brief Write all the data in [input] to the [dataset].
/// This function expects the [dataset] to accomodate the number of elements data in input.
/// \note [input] should present an array which can be queried for size() and an element at(index).
template <typename T> void storeBufferToDataSet(H5::DataSet dataset, T &input)
  {
  // Create a space to write to the file with.
  // This is changed in the loop below to point at
  // each of the sub elements of the generated buffer.
  hsize_t fdim[] = { BlockElementCount, input.size() };
  H5::DataSpace writespace( 2, fdim );
  hsize_t start[2] = { 0, 0 }; // Start of hyperslab
  hsize_t block[2] = { BlockElementCount, 1 };  // Block sizes

  // Lock and write our data to the file.
  for (size_t i = 0; i < input.size(); ++i)
    {
    std::cout << "  Writing block " << i << std::endl;

    // Lock access to the data in the shared buffer.
    auto &genBuffer = input.at(i);
    LockedData lockedData(&genBuffer, LockedData::ConstOnly);
    auto data = lockedData.constData();

    // Find a read space for the input buffer
    H5::DataSpace readspace = genBuffer.createDataSpace();

    // Point the output space at the right sub elements.
    start[1] = i;
    writespace.selectHyperslab(H5S_SELECT_SET, block, start);

    // Write the section in to the selected portion of the file
    dataset.write(data->data(), H5::PredType::NATIVE_FLOAT, readspace, writespace);
    }
  }
