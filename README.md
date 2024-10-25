A Clone Hero Patterns Generator which is (will be) able to generate notes.chart files with certain patterns for YARG/Clone Hero.
(Note that I am not experienced with C++ there might and will be odd code)

Building:
- cmake is required
- I am using gcc (I think?) so if other cxx compilers complain that's probably why
Steps:
1. create `build` directory in the top folder (The same folder that contains CMakeLists.txt):
`mkdir build && cd build`
2. run: 
`cmake ..`
3. run: 
`cmake --build .`
Repeat only step 3 (inside the `build` directory) for subsequent builds

Rough sketch of TODOs:
- a lot
- figure out why it is segfaulting sometimes
 
