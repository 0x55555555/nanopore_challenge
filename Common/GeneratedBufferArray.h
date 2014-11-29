#pragma once
#include <array>
#include "GeneratedBuffer.h"
#include "ConstantParameters.h"

class GeneratedBufferArray
  {
public:
  GeneratedBuffer *at(size_t i)
    {
    return &m_elements.at(i);
    }

private:
  std::array<GeneratedBuffer, BufferCount> m_elements;
  };
