#include "SharedBuffer.h"

SharedBuffer::SharedBuffer(const char *name, size_t size)
    : m_sharedMemory(
        boost::interprocess::open_or_create,
        name,
        boost::interprocess::read_write),
      m_mutex(
        boost::interprocess::open_or_create,
        (std::string(name) + "_mutex").data())
  {
  // Resize the array to [size]
  m_sharedMemory.truncate(size);

  // Setup [m_region] to reference the shared object with rw access.
  m_region = boost::interprocess::mapped_region(m_sharedMemory, boost::interprocess::read_write);
  assert(m_region.get_address());
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
