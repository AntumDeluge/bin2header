## Binary to Header


### **Description:**

Converter for binary files to C/C++ headers.

***bin2header*** takes any file as an argument and converts its binary data into a source header file for use in C/C++
applications. The data is stored as a character array.


### **Licensing:**

[MIT](LICENSE.txt)


### **Building:**

#### 'configure' script (currently recommended):

<blockquote style="padding-left:2em; font-family:monospace;">
<b>USAGE:</b> ./configure [--help] [--type=TYPE] [--static]<br><br>
Options:<br>
<span style="padding-left:2em; font-weight:bold; font-style:italic;">--help</span> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Show this help text<br>
<span style="padding-left:2em; font-weight:bold; font-style:italic;">--type=TYPE</span> &nbsp; Set to "b" (default) for binary executable "s" for Python script<br>
<span style="padding-left:2em; font-weight:bold; font-style:italic;">--static</span> &nbsp;&nbsp;&nbsp;&nbsp; Create statically linked executable (binary only)
</blockquote>

- To build native binary, run: `./configure && make`
- To build static native binary, run: `./configure --static && make`
- To build python version, run: `./configure --type=s && make`

#### CMake (native binary only):

Build in source root directory:

```sh
cmake ./ && make
```

Build in separate directory:

```sh
mkdir build
cd build
cmake ../ && make
```
