#pragma once

#include "ConstantParameters.h"
#include <atomic>
#include <array>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

namespace H5
{
class DataSpace;
}

class LockedData;

/// \brief A container for a single block of generated data
/// \note Uses size information in [ConstantParameters.h]
class GeneratedBuffer
  {
public:
  /// \brief A fixed size buffer of [real_type]'s for generating into
  /// \note Access using the [LockedData] class.
  typedef std::array<real_type, BlockElementCount> Buffer;

  /// \brief Create a new buffer
  GeneratedBuffer();

  /// \brief Format a dataspace to read the data from this buffer
  ///   The dataspace has one dimension and is sized to fit all elements in this object.
  H5::DataSpace createDataSpace();

  /// \brief Find the revision for this block - increased each time someone locks for writing.
  /// \note Use LockedData::ConstOnly to not increase the revision on use.
  size_t revision() const { return m_revisionCount; }

  /// \brief Block until m_revisionCount to change (via another process).
  void waitForChange(LockedData *lock);

private:
  Buffer m_elements;
  std::atomic<size_t> m_revisionCount;
  boost::interprocess::interprocess_mutex m_mutex;
  boost::interprocess::interprocess_condition m_condition;

  friend class LockedData;
  };

/// \brief A lock used to secure access to the data, access
class LockedData : public boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex>
  {
public:
  enum Mode
    {
    ConstOnly,
    Writable,
    };

  /// \brief Create a locked data instance - locking the buffer.
  LockedData(GeneratedBuffer *buffer, Mode constLock = Writable);
  ~LockedData();

  /// \brief Obtain a pointer to the data itself
  /// \note When the data is unlocked listeners are informed of a change
  GeneratedBuffer::Buffer *data();

  /// \brief Obtain a pointer to the data itself, this data should not be changed!
  const GeneratedBuffer::Buffer *constData() const;

private:
  GeneratedBuffer *m_buffer;
  bool m_constLock;
  };
