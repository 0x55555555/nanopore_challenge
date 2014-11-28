#pragma once
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

/// \brief A ring buffer shared in on the local machine
class SharedBuffer
  {
public:
  /// \brief Create a buffer called [name]
  SharedBuffer(const char *name, size_t size);

private:
  boost::interprocess::shared_memory_object m_sharedMemory;
  boost::interprocess::mapped_region m_region;
  boost::interprocess::named_mutex m_mutex;

  friend class LockedData;
  };

/// \brief A lock used to secure access to the data.
class LockedData : private boost::interprocess::scoped_lock<boost::interprocess::named_mutex>
  {
public:
  /// \brief Create a locked data instance - locking the buffer.
  LockedData(SharedBuffer *buffer);

  /// \brief Obtain a pointer to the data itself.
  void *data();

private:
  SharedBuffer *buffer;
  };

/// \brief A small wrapped used to type the data which is locked
template <typename T> class TypedLockedData : LockedData
  {
public:
  /// \brief Create a typed view on [buffer], securing access to the data inside.
  TypedLockedData(SharedBuffer *buffer) : LockedData(buffer)
    {
    }

  /// \brief Obtain a pointer to the typed data
  T *data(size_t offset=0)
    {
    T *root = reinterpret_cast<T *>(LockedData::data());

    return root + offset;
    }
  };
