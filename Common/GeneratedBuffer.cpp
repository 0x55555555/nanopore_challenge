#include "GeneratedBuffer.h"

GeneratedBuffer::GeneratedBuffer()
    : m_revisionCount(0)
  {
  }

LockedData::LockedData(GeneratedBuffer *buf)
    : scoped_lock(buf->m_mutex),
      m_buffer(buf)
  {
  }

LockedData::~LockedData()
  {
  // Increase the revision count so users of the buffer know its changed.
  ++m_buffer->m_revisionCount;

  // The data has changed, signal any people awaiting this.
  m_buffer->m_condition.notify_all();
  }

GeneratedBuffer::Buffer *LockedData::data()
  {
  return &m_buffer->m_elements;
  }

const GeneratedBuffer::Buffer *LockedData::constData() const
  {
  return &m_buffer->m_elements;
  }
