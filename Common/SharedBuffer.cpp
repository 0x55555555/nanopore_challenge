#include "SharedBuffer.h"

SharedBuffer::SharedBuffer(const std::string &name, size_t size)
    : m_sharedMemory(
        boost::interprocess::open_or_create,
        name.data(),
        boost::interprocess::read_write),
      m_mutex(
        boost::interprocess::open_or_create,
        (name + "_mutex").data())
  {
  // Resize the array to [size]
  m_sharedMemory.truncate(size);

  // Setup [m_region] to reference the shared object with rw access.
  m_region = boost::interprocess::mapped_region(m_sharedMemory, boost::interprocess::read_write);
  assert(m_region.get_address());
  }

SharedBuffer::SharedBuffer(const std::string &name)
    : m_sharedMemory(
        boost::interprocess::open_only,
        name.data(),
        boost::interprocess::read_write),
      m_mutex(
        boost::interprocess::open_only,
        (name + "_mutex").data())
  {
  // Setup [m_region] to reference the shared object with rw access.
  m_region = boost::interprocess::mapped_region(m_sharedMemory, boost::interprocess::read_write);
  assert(m_region.get_address());
  }

void SharedBuffer::remove(const std::string &name)
  {
  boost::interprocess::shared_memory_object::remove(name.data());
  boost::interprocess::named_mutex::remove((name + "_mutex").data());
  }

LockedData::LockedData(SharedBuffer *buf)
    : scoped_lock(buf->m_mutex),
      buffer(buf)
  {
  }

void *LockedData::data()
  {
  return buffer->m_region.get_address();
  }
