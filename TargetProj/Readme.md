## Recognition Demo on Raspberry Pi
This source code can be built on Raspberry Pi with cmake

### Requirements
* OpenCV 2.4 / 3.1
* WiringPi
* g++ 4.8
* cmake 3.0+

### Build on Windows
These files are not tested building on windows system. If you want to test the algorithms. Please use the [Test projects] (https://github.com/zhuchen115/wvo-locator/tree/master/testproject) 

### Build method
If you want to build the source code, you can use CMake.
It is suggested to build the source code in the folder other than the source code.

1. Make a directory to build the source code.
```bash
mkdir build 
cd build
```
**Use Default Value for configuration**
2. Configure the project 
```shell
cmake ../
```
**Custom the Configuration Value**
2. Configure the project
```shell
ccmake ../
```
*You must installed ccmake before use it*

3. Build the source code
``` bash
make 
``` 

4. Install the binaries
```bash
make install
```
Normally, it will be installed to /usr/local/

### License

GPL v3+ 

### Software Configuration
The configuration is not put into the CMake options, you need to edit the config.h in src/ directory.

You can find Chinese document [Here](https://www.greatqq.com/2017/01/wvo-recognize-confiure) 

