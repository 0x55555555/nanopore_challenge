#include <iostream>
#include "Generator.h"
#include "SharedBuffer.h"
#include "GeneratedBufferArray.h"
#include "ConstantParameters.h"
#include <unistd.h>

int generateRandomData(const char *name)
  {
  // Wipe down the memory in the generator - we are about to recreate it and fill.
  SharedBuffer::remove(name);

  // Create a shared buffer called [sharedMemoryName], large enough to hold [count] * [bufferCount] real_type's.
  TypedSharedBuffer<GeneratedBufferArray> buffer(name, boost::interprocess::open_or_create);
  buffer.initialise();

  // The generator will generate random numbers between -1000 and 1000.
  Generator<real_type> gen(-1000, 1000);

  // Loop infinitely, generating data into [buffer].
  size_t i = 0;
  for (;;)
    {
    // record the current time so we can ensure the cycle takes 1s in total.
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    const size_t bufferIdx = i;
    std::cout << "Starting generation of " << BlockElementCount << " elements for area " << bufferIdx << std::endl;

    {
      // Find the buffer to generate into
      GeneratedBuffer *generatedBuffer = buffer.data()->at(bufferIdx);

      // Lock the data inside the buffer, exposing the floats for generation
      LockedData lockedData(generatedBuffer);
      auto data = lockedData.data();

      // Generate into [count] elements into [data].
      gen.generate(*data);
    }

    ++i;
    if (i == 10)
      {
      i = 0;
      }

    // Find how long the generation took.
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> consumedTime = end - start;

    // We should wait the remainder of [Delay]
    double waitForSeconds = Delay - consumedTime.count();
    std::cout << "Finished generation, wait for " << waitForSeconds << "s" << std::endl;

    if (waitForSeconds > 0)
      {
      int sleepFor = waitForSeconds * MicrosecondsInSecond;
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
