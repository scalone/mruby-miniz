mruby-miniz
============

"mruby-miniz" mrbgem provides interface to zip and unzip files.
Implemented using [miniz](https://code.google.com/p/miniz/), Single C source file Deflate/Inflate compression library with zlib-compatible API, ZIP archive reading/writing, PNG writing.

## Example
```sh
% vim zip.rb
Miniz.zip("test.zip", "test.txt", "test2.txt", "test3.txt")


% vim unzip.rb
Miniz.unzip("test.zip")
...
```

## Requirement
- [iij/mruby-io](https://github.com/iij/mruby-io) mrbgem
- [iij/mruby-dir](https://github.com/iij/mruby-dir) mrgbem
- and some POSIX API...

## TODO
- Support zip directories.
- Remove IO require, configurable.
- Remove malloc require, configurable.
- add missing methods.
- write tests.
- Add exception instead of return false if some problem happens.


## License
under the MIT License:

* http://www.opensource.org/licenses/mit-license.php