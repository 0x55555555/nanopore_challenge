#include "SharedBuffer.h"

SharedBuffer::SharedBuffer(const std::string &name, size_t size, boost::interprocess::open_or_create_t)
    : m_sharedMemory(
        boost::interprocess::open_or_create,
        name.data(),
        boost::interprocess::read_write)
  {
  // Resize the array to [size]
  m_sharedMemory.truncate(size);

  // Setup [m_region] to reference the shared object with rw access.
  m_region = boost::interprocess::mapped_region(m_sharedMemory, boost::interprocess::read_write);
  assert(m_region.get_address());
  }

SharedBuffer::SharedBuffer(const std::string &name, boost::interprocess::open_only_t)
    : m_sharedMemory(
        boost::interprocess::open_only,
        name.data(),
        boost::interprocess::read_write)
  {
  // Setup [m_region] to reference the shared object with rw access.
  m_region = boost::interprocess::mapped_region(m_sharedMemory, boost::interprocess::read_write);
  assert(m_region.get_address());
  }

void *SharedBuffer::data()
  {
  return m_region.get_address();
  }

void SharedBuffer::remove(const std::string &name)
  {
  boost::interprocess::shared_memory_object::remove(name.data());
  }
