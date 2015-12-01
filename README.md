# dewalls
A C++/Qt parser for Walls .wpj and .srv cave survey data format

I'm on a crusade to liberate survey data from the Walls format so it can be used in modern 3D cave survey viewers like [Cavewhere](https://github.com/Cavewhere/cavewhere) and [Breakout](https://github.com/jedwards1211/breakout).

Right now this is just a parsing library to be embedded in other programs.  Maybe someday I'll use it to make command-line converters from Walls to other formats.

I've been careful to write very clean and precise code for this parser, but the documentation is...pretty much nonexistent :)  If you're interested in using this for your own program, let me know and I'll write up documentation and help you get started.

## Building

Just open `dewalls.qbs` in Qt and hit run.
