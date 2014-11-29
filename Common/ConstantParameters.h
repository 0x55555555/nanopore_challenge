#pragma once
#include <cstddef>

const size_t BlockElementCount = 1000000; ///< Random numbers to generate in one phase
const size_t BufferCount = 10; ///< How many [count]'s should the buffer contain
const int Delay = 1; ///< Wait period after each genration step - seconds
typedef float real_type; ///< The type of data to generate
const size_t BufferSize = BufferCount * BlockElementCount * sizeof(real_type);

const size_t FileCount = 5; ///< How many different files should be created and written to

const int MicrosecondsInSecond = 1000 * 1000;
