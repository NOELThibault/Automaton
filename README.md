# The project
A library implementing operations on deterministic and non-deterministic automata along with its testing.

This was a college project for my language theory class.

Every basic operation is implemented and tested. More complex operations, such as creating minimal or deterministic automata may contain bugs (they do).

The tests are made by myself and may miss some edge cases (they do).

The project lacks documentation, though you may be able to generate it quickly as there are Doxygen comments for every attribute and method implemented. 

# Build
Requires CMake 3.10 or newer. It uses GoogleTest but it's already setup in the repo.

mkdir build<br>
cd build<br>
cmake ..<br>
make
