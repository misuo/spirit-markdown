This is a fork of [Spirit Markdown](https://launchpad.net/spirit-markdown) - a [Markdown][] to [HTML][] converter written by Alastair Rankine in C++ using [Boost.Spirit][Spirit] as parser/generator engine.

Original source files retrieved from
* https://launchpad.net/spirit-markdown
* http://bazaar.launchpad.net/~arankine/spirit-markdown/trunk/files/94/

# Spirit Markdown

Originally written by Alastair Rankine <alastair@girtby.net>, 2010, with the following comments:

> Spirit Markdown is an implementation of a [Markdown][] syntax parser using the [Spirit][] parser library. It is a single pass parser for Markdown, and is written in C++, so it should be extremely fast. It is written to parse the input to an internal document object representation, which allows flexibility in output generation. Also, a complete suite of unit tests are provided, to ensure against functional regressions in the future.
>
> Spirit Markdown is currently quite immature and so performance may not yet be comparable with some other Markdown implementations. There are many optimizations to be made, both in the parser rules and in the code itself. For now, the emphasis is on correctness, specifically satisfying the complete requirements of the Markdown language.

The intention with this fork is to bring it up to level with other CommonMark parsers/generators. Additionally it might be worth to explore adding extensions, e.g. such as tables. 

## How to build

To build use `CMake`. 

> :bulb: As of this writing the **Debug** version is always built. To remove this, outcomment the following line in `CMakeLists.txt`:
> ~~~cmake
> ...
> #set(CMAKE_BUILD_TYPE Debug)
> ...
> ~~~

1. Get the source.

2. Create build directory:

    ~~~~
    $ mkdir build
    $ cd build
    ~~~~

3. Initialize/configure `CMake`:

   ~~~
   $ cmake ..
   ~~~

4. Build it:

   ~~~~
   $ make
   ~~~~

# To run tests

There is about 44 tests included - made using Boost.Test unit tests. Note that I haven't (yet) checked whether theses matches the standard test suites available from respectively John Grubers and/or CommonMark.

The `CMakeLists.txt` contains actually two ways to run tests. One integrated with `CTest` and one using Alistair's method

1. CTest unit test output

     ~~~~
     $ make test
     ~~~~

2. Boost unit test output

     ~~~~
     $ make testx
     ~~~~

   A successful test run should output:

     ~~~~
     Running 44 test cases...

     *** No errors detected
     ~~~~

# To-do

* [ ] Verify/examine that all unit tests are running. Resolve those which fails.
* [ ] Ensure that unit tests matching the John Grubers standard unit tests are part of unit tests. Missing ones should be added.
* [ ] Ensure that unit tests matching the CommonMark unit tests are part of unit tests. Missing ones should be added.
* [ ] *... other?*

# Ideas

* [ ] Add performance measurements to compare with other implementations.

# License

Open Source - [Boost Software License](http://www.boost.org/LICENSE_1_0.txt).

[Markdown]: http://daringfireball.net/projects/markdown/
[Spirit]: http://boost-spirit.com/home/
[HTML]: https://en.wikipedia.org/wiki/HTML