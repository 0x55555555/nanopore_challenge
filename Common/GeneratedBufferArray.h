#pragma once
#include <array>
#include "GeneratedBuffer.h"
#include "ConstantParameters.h"

/// \brief A utility containing [BufferCount] generated arrays,
/// used as a convinience for constructing into a shared memory object
class GeneratedBufferArray
  {
public:
  /// \brief Get the data element at [i].
  GeneratedBuffer &at(size_t i)
    {
    return m_elements.at(i);
    }

  /// \brief Find the size of the element array.
  size_t size() const
    {
    return m_elements.size();
    }

private:
  std::array<GeneratedBuffer, BufferCount> m_elements;
  };
