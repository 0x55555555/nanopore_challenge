#pragma once

const size_t count = 1000000; ///< Random numbers to generate in one phase
const size_t bufferCount = 10; ///< How many [count]'s should the buffer contain
const int delay = 1; ///< Wait period after each genration step - seconds
typedef float real_type; ///< The type of data to generate

const int microsecondsInSecond = 1000 * 1000;
