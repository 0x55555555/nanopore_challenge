#include "GeneratedBuffer.h"
#include "H5Cpp.h"

GeneratedBuffer::GeneratedBuffer()
    : m_revisionCount(0)
  {
  }

H5::DataSpace GeneratedBuffer::createDataSpace()
  {
  // Create a dataspace representing a this object as a single "row" of data.
  hsize_t readdim[] = { BlockElementCount };
  H5::DataSpace readspace( 1, readdim );

  return readspace;
  }

void GeneratedBuffer::waitForChange(LockedData *lock)
  {
  size_t rev = m_revisionCount;

  // Wait will unlock [lock], and loop checking the predicate passed
  m_condition.wait(
    *lock,
    [this, rev](){ return rev != m_revisionCount; }
    );
  }

LockedData::LockedData(GeneratedBuffer *buf, Mode mode)
    : scoped_lock(buf->m_mutex),
      m_buffer(buf),
      m_constLock(mode == ConstOnly)
  {
  }

LockedData::~LockedData()
  {
  // Only inform of changes if we were changing the data.
  if (!m_constLock)
    {
    // Increase the revision count so users of the buffer know its changed.
    ++m_buffer->m_revisionCount;

    // The data has changed, signal any people awaiting this.
    m_buffer->m_condition.notify_all();
    }
  }

GeneratedBuffer::Buffer *LockedData::data()
  {
  assert(!m_constLock);
  return &m_buffer->m_elements;
  }

const GeneratedBuffer::Buffer *LockedData::constData() const
  {
  return &m_buffer->m_elements;
  }
