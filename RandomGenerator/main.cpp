#include <iostream>
#include "Generator.h"
#include "SharedBuffer.h"
#include <unistd.h>

const size_t count = 1000000; // Random numbers to generate in one phase
const size_t bufferCount = 10;
const int microsecondsInSecond = 1000 * 1000;
const int delay = 1; // Wait period - seconds
typedef float real_type;

int generateRandomData(const char *name)
  {
  boost::interprocess::shared_memory_object::remove(name);
  boost::interprocess::named_mutex::remove((std::string(name) + "_mutex").data());

  // Create a shared buffer called [sharedMemoryName], large enough to hold [count] * [bufferCount] real_type's.
  SharedBuffer buffer(
        name,
        bufferCount * count * sizeof(real_type));

  // The generator will generate random numbers between -1000 and 1000.
  Generator<real_type> gen(-1000, 1000);

  // Loop infinitely, generating data into [buffer].
  size_t i = 0;
  for (;;)
    {
    // record the current time so we can ensure the cycle takes 1s in total.
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    const size_t bufferIdx = i % 10;
    std::cout << "Starting generation of " << count << " elements for area " << bufferIdx << std::endl;

    // Find the right segment of the buffer.
    const size_t offset = bufferIdx * count;
    {
      TypedLockedData<real_type> lockedData(&buffer);
      auto data = lockedData.data(offset);

      // Generate into [count] elements into [data].
      gen.generate(data, count);
    }

    ++i;

    // Find how long the generation took.
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> consumedTime = end - start;

    // We should wait the remainder of [delay]
    double waitForSeconds = delay - consumedTime.count();
    std::cout << "Finished generation, wait for " << waitForSeconds << "s" << std::endl;

    if (waitForSeconds > 0)
      {
      int sleepFor = waitForSeconds * microsecondsInSecond;
      usleep(sleepFor);
      }
    }

  return 1;
  }



int main(int argc, char *argv[])
  {
  if (argc != 2)
    {
    std::cerr << "Incorrect number of arguments passed - expected 2, got " << argc;
    return 0;
    }

  generateRandomData(argv[1]);
  return 1;
  }
