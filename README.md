## bin2header


### **Description:**

Converter for binary files to C/C++ headers.

***bin2header*** takes any file as an argument and converts its binary data into a source header file for use in C/C++
applications. The data is stored as a character array.


### **Licensing:**

[MIT](LICENSE.txt)


### **Building:**

#### 'configure' script:

To build native binary, run: `./configure && make && sudo make install`

To build python version, run: `./configure --type=s && make && sudo make install`

#### CMake (native binary only):

Build in source root directory:

```sh
cmake ./ && make && sudo make install
```

Build in separate directory:

```sh
mkdir build
cd build
cmake ../
make && sudo make install
```
