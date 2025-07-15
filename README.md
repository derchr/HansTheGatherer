# General
This is a template for creating software for wueHans. The template comes with a Desktop backend, which allows to use your software also on Windows/Linux/Mac (see further down).

# Setup
## Disclaimer
We only aim to have this work on Windows, Linux, and Mac. If it works on something else, then this is pure coincidence.


## The easy stuff
You need:
- [CMake](https://cmake.org/download/)
- [Ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages)
- A code editor

## Download Clang
#### Windows
The clang binaries for windows can be downloaded from this shady google drive link (size ca. 7 GB): [https://drive.google.com/file/d/1f136QhEjiOVqIvz3E0IVTMM5NRc88Kyc/view?usp=drivesdk](https://drive.google.com/file/d/1f136QhEjiOVqIvz3E0IVTMM5NRc88Kyc/view?usp=drivesdk). Just kidding, this is *my* shady google drive so you should be safe. Anyway, Unpack the zip file and add clangBinaries\Release\bin to your path. You should now be able to execute the following in your command line:

```bash
clang
```
which should return:
```bash
clang: error: no input files
```

#### Linux
@Jonathan, bitte nochmal durchlesen

Clang can be downloaded with your preferred package manager (example shown with ???). After download, make sure that the binaries are within your path.

## Download the Software Template
Go to this Github repo and press the "Use this template" button to create a new repository. This creates a repository on your account that has all the content of the [Hans 2 Software Template](https://github.com/Byter64/Hans2SoftwareTemplate/tree/main) repo in it. Now, clone the repository to your computer. Remember to adjust the commands in the next section to your repositories name.

## Setup the Template
Open a command line and execute the following:
```bash
cd blaaa/YourRepository/build/Desktop
cmake ../.. -G Ninja -DDESKTOPBUILD=ON
cd ../wueHans
cmake ../.. -G Ninja
```
This initialises the build folder, in which cmake and ninja will throw in the build files. Desktop contains the desktop version which should run on your PC, while wueHans contains the wueHans build.
> 💡 **_Good To Know_**: Windows users, who want to use Visual Studio, can skip the "-G Ninja" for the Desktop build. The wueHans build **NEEDS** Ninja
> 
> 💡 **_Good To Know_**: You can build both the desktop and wueHans version by calling "Ninja" from Hans2SoftwareTemplate/build

# Important information
## Using Visual Studio Code
(muss noch den Matthias fragen)

## Distinguish between Desktop and wuehans
If the have code, that should only be included in either the desktop build or the wueHans build, you can use the precprocessor macros #ifdef ... #endif with the symbols **WUEHANS** and **DESKTOP**. The desktop build will only have the **DESKTOP**, while the wueHans build will only have **WUEHANS** symbol defined.

## Supported file formats
Halib only supports .bmp for images and .wav for audio. You can always include libraries that support other file formats, but keep in mind, that uncompressing data takes time, and time is a limited resource for wueHans.

# Template Structure
#### build/
This directory will contain your built project
#### cmake/
This directory contains helper files for cmake. You usually don't need to touch it
#### code/
This is where all your code, assets, and any file in general should go. This folder is yours
#### lib/
This directory contains system libraries, that are needed for building your project. You usually don't need to touch it
#### system-include/
This directory contains system headers, that are needed for building your project. You usually don't need to touch it
#### wuehans/
This directory contains files that are wueHans specific but are needed by every project. You usually don't need to touch it

# Adding Code to the Project
If you already know cmake, you can skip this section.

A target is a file, that is built by clang. The template lets clang generate exactly one target by default, that is your executable software. In order to add a new .c/.cpp to this target, you need to edit code/CMakeLists.txt. Open the file and add your new file under the "# Add your .cpp files here" comment.

# Building the Project
If you want to build the projectv open a command line and execute the following:
```bash
cd blaaa/YourRepository/build
ninja
```
If you have only added new source files in CMake, this should build the project and result in an executable file called build/code/game.elf.
