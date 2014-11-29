#pragma once

#include "ConstantParameters.h"
#include <array>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

class GeneratedBuffer
  {
public:
  /// \brief a fixed size buffer of real_types for generating into
  /// \note access using the [LockedData] class.
  typedef std::array<real_type, BlockElementCount> Buffer;

  /// \brief Create a new buffer
  GeneratedBuffer();

private:
  Buffer m_elements;
  size_t m_revisionCount;
  boost::interprocess::interprocess_mutex m_mutex;
  boost::interprocess::interprocess_condition m_condition;

  friend class LockedData;
  };

/// \brief A lock used to secure access to the data.
class LockedData : private boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex>
  {
public:
  /// \brief Create a locked data instance - locking the buffer.
  LockedData(GeneratedBuffer *buffer);
  ~LockedData();

  /// \brief Obtain a pointer to the data itself
  /// \note When the data is unlocked listeners are informed of a change
  GeneratedBuffer::Buffer *data();

  /// \brief Obtain a pointer to the data itself, this data should not be changed!
  const GeneratedBuffer::Buffer *constData() const;

private:
  GeneratedBuffer *m_buffer;
  };
