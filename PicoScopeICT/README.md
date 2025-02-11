# picosdk-c-examples

*picosdk-c-examples* is a set of C/C++ examples for PicoScope<sup>®</sup> oscilloscopes and PicoLog<sup>®</sup> data loggers.

Examples are provided as Microsoft Visual Studio Express 2015 for Windows Desktop solutions and are grouped by driver name. Each driver folder has a `linux-build-files` sub-folder for building applications on Linux and macOS platforms from a terminal window.

## Getting started

### Prerequisites

#### Microsoft Windows

* Microsoft Visual Studio 2015 (including Express and Community editions) or later. 

#### Linux

* A suitable IDE such as [Eclipse](https://www.eclipse.org/downloads/?)
* Alternatively, ensure that the `autoconf` and `libtool` packages are installed for your operating system if building applications from the terminal.

#### macOS

* A suitable IDE such as [XCode](https://developer.apple.com/xcode/)

### Installing drivers

Drivers are available for the following platforms. Refer to the subsections below for further information.

#### Microsoft Windows

* Download the PicoSDK (32-bit or 64-bit) driver package installer from our [Downloads page](https://www.picotech.com/downloads).

#### Linux

* Follow the instructions from our [Linux Software & Drivers for Oscilloscopes and Data Loggers page](https://www.picotech.com/downloads/linux) to install the required driver packages for your product.

#### macOS

* Visit our [Downloads page](https://www.picotech.com/downloads) and download the *PicoScope Beta for macOS* application.

### Programmer's Guides

You can download Programmer's Guides providing a description of the API functions for the relevant PicoScope or PicoLog driver from our [Documentation page](https://www.picotech.com/library/documentation).

### Building example applications

#### Microsoft Windows

* Open the solution .sln file for the required driver in Visual Studio
* Select the required configuration and solution platform
* Build the solution

#### Linux and macOS

* Copy the required source code C file (e.g. ps3000aCon.c) into the corresponding `linux-build-files` sub-folder for the driver or copy the files to another folder 
* Open a terminal window
* Run the following commands:
  * `./autogen.sh`
  * `make`

## Obtaining support

Please visit our [Support page](https://www.picotech.com/tech-support) to contact us directly or visit our [Test and Measurement Forum](https://www.picotech.com/support/forum19.html) to post questions.

## Contributing

Contributions are welcome. Please refer to our [guidelines for contributing](.github/CONTRIBUTING.md) for further information.

## Copyright and licensing

See [LICENSE.md](LICENSE.md) for license terms. 

*PicoScope* and *PicoLog* are registered trademarks of Pico Technology Ltd. 

*Windows* and *Visual Studio* are registered trademarks of Microsoft Corporation. 

*macOS* is a registered trademark of Apple Inc. 

*Linux* is the registered trademark of Linus Torvalds in the U.S. and other countries.

Copyright © 2004-2019 Pico Technology Ltd. All rights reserved. 

