#pragma once
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

/// \brief A ring buffer shared in on the local machine
class SharedBuffer
  {
public:
  /// \brief Create a buffer called [name]
  SharedBuffer(const std::string &name, size_t size, boost::interprocess::open_or_create_t);

  /// \brief Open a buffer called [name]
  SharedBuffer(const std::string &name, boost::interprocess::open_only_t);

  /// \brief Get the data in the shared object
  void *data();

  /// \brief Delete the shared memory object from disk
  static void remove(const std::string &name);

private:
  boost::interprocess::shared_memory_object m_sharedMemory;
  boost::interprocess::mapped_region m_region;
  };


/// \brief Typed wrapper around raw shared memory.
template <typename T> class TypedSharedBuffer : public SharedBuffer
  {
public:
  TypedSharedBuffer(const std::string &name, boost::interprocess::open_or_create_t c)
      : SharedBuffer(name, sizeof(T), c)
    {
    }

  TypedSharedBuffer(const std::string &name, boost::interprocess::open_only_t o)
      : SharedBuffer(name, o)
    {
    }

  /// \brief Construct the object into the shared memory.
  void initialise()
    {
    new(data()) T();
    }

  /// \brief Get access to the shared object.
  T *data()
    {
    return reinterpret_cast<T *>(SharedBuffer::data());
    }
  };
