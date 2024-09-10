# Created by TuanTNT19
## Introdution

The program is designed to facilitate communication between the PC and Nvidia Xavier NX at a frequency of 800Hz. At any given time, the PC will transmit one of the following two messages:


Message 1: Coordinates x, y, z

    {
      int x;
      int y;
      int z;
    }


Message 2: Heartbeat

    {
      float panAngle;
      float tiltAngle;
      uint8_t errorCode[6];
    }

On the Nvidia Xavier NX, each field value of the message is multiplied by 2, written to separate files and then sent back to the PC. The communication technology used is Protobuf 3.13.

## Protobuf Installation and Cross-Compilation Guide

### 1. Installing Protobuf on Your PC

To install Protobuf on your PC, follow these steps:

#### 1.1. Install Dependencies

Install the necessary packages for building Protobuf:

`sudo apt-get update`

`sudo apt-get install autoconf automake libtool curl make g++ unzip `

#### 1.2. Download Protobuf Source Code

Download the Protobuf source code for version 3.13.0:

`wget https://github.com/protocolbuffers/protobuf/releases/download/v3.13.0/protobuf-all-3.13.0.tar.gz ` 

#### 1.3. Extract Source Code

Extract the downloaded source code:

`tar -xzvf protobuf-all-3.13.0.tar.gz`

`cd protobuf-3.13.0 ` 


#### 1.4. Configure and Build

Run the following commands to configure and build Protobuf:

`./autogen.sh`

`./configure`

`make`

`sudo make install`

`sudo ldconfig`
 ` 
#### 1.5. Verify Installation

Check the installed version of Protobuf to ensure it is correctly installed:

`protoc --version ` 

### 2. Cross-Compile Protobuf for Nvidia Xavier NX

To cross-compile Protobuf for Nvidia Xavier NX, follow these steps:
#### 2.1. Install Cross-Compilation Tools

Install the cross-compilation tools for aarch64:

`sudo apt-get update`

`sudo apt-get install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu ` 

#### 2.2. Download Protobuf Source Code

Download the Protobuf source code for version 3.13.0:

`wget https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.13.0.zip`

`unzip v3.13.0.zip`

`cd protobuf-3.13.0 ` 

#### 2.3. Install Cross-Compilation Dependencies

Install the necessary tools and libraries for cross-compilation:

`sudo apt-get install autoconf automake libtool curl make g++ unzip`


#### 2.4. Configure Cross-Compilation

Set environment variables for cross-compiling for aarch64:

`export CC=aarch64-linux-gnu-gcc`

`export CXX=aarch64-linux-gnu-g++ ` 

#### 2.5. Configure and Build Protobuf

Configure and build Protobuf for aarch64:

`git submodule update --init --recursive`

`./autogen.sh`

`./configure --host=aarch64-linux-gnu --prefix=/path/to/install`

For example, if your path is /home/tuantnt/Desktop/protobuf_arm/protobuf_set/

`make`

`make install `
