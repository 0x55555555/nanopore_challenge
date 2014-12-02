Threading / HDF5 example
========================

Building
--------

### Using Qt Creator UI

* Open All.pro using QtCreator
* Setup a kit for the project
* Choose Build >> Build All

### Using qmake

* Run `qmake All.pro` in a terminal
* run `make`

Running
-------

There are 4 executables built by the All.pro project.

### Random Generator

Generates random data into a shared memory buffer specified on run:

`RandomGenerator SharedBufferName`
>> Continually writes to SharedBufferName every 1s.

### Simple Consumer

Consumes data stored in a shared memory buffer and places it in a fixed size Hdf5 file.

`SimpleConsumer SharedBufferName myfile.h5`
>> Creates myfile.h5

### VariableSizedConsumer

Consume random amounts of the shared memory buffer passed, and stores these buffers as files on disk.

`VariableSizedConsumer SharedBufferName myfile.h5`
>> Continually creates 0_myfile.h5, 1_myfile.h5, 2_myfile.h5...

### Transformer

Transform data in a shared memory buffer to between the value 0 -> 1, then store it in another memory buffer. (Note, not the same buffer!)

`Transformer SharedBufferName SharedBufferTransformed`
>> Continually writes to SharedBufferTransformed whilst reading and transforming data.
