# dewalls
A C++/Qt parser for the Walls .srv cave survey data format

I'm on a crusade to liberate survey data from the Walls format so it can be used in modern 3D cave survey viewers like [Cavewhere](https://github.com/Cavewhere/cavewhere) and [Breakout](https://github.com/jedwards1211/breakout).
The Walls format is far more complicated than any other cave survey data format I know of, which is probably why I
haven't known of any other programs that can read or write Walls data.  But that will soon change thanks to this library.

Right now this is just a parsing library to be embedded in other programs, but I'll probably make a command-line exporter for some other formats soon.

I've been careful to write very clean and precise code for this parser, but the documentation is...pretty much nonexistent :)  If you're interested in using this for your own program, let me know and I'll write up documentation and help you get started.

## Building

Just open `dewalls.qbs` in Qt and hit run.
