ZETA
====

ZEBRA is a [VG-RAM Weightless Neural Network](https://github.com/xperroni/Yamabiko/tree/master/2013-2) system for estimating depth maps. It is developed in C++ 03 on top of [boost](http://www.boost.org/), [OpenCV](http://opencv.org/) and [Clarus](https://github.com/xperroni/Clarus).

ZEBRA is an experimental project, and currently serves no practical purpose other than demonstrating the use of the Clarus library, particularly its VG-RAM API.

ZEBRA is developed inside the [Code::Blocks](http://www.codeblocks.org/) IDE, and currently this is the only supported build environment. I intend to provide a set of [autotools](http://www.gnu.org/savannah-checkouts/gnu/automake/manual/html_node/index.html#Top) files for cross-platform building in the future.

TO DO
-----

* Provide a set of autotools files for building the library;
* Document the source files;
* Move the code base to C++ 11, when support is widespread enough.
