# GLA

GLA is an OpenGL Loading Library.

> An OpenGL Loading Library is a library that loads pointers to OpenGL functions at runtime, core as well as extensions.
> This is required to access functions from OpenGL versions above 1.1 on most platforms. Extension loading libraries
> also
> abstracts away the difference between the loading mechanisms on different platforms.

- [OpenGL Loading Library - khronos.org](https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library)

GLA parses the [Khronos](https://www.khronos.org) created [GL/glcorearb.h](include/GL/glcorearb.h)
, [GL/glext.h](include/GL/glext.h) and [KHR/khrplatform.h](include/KHR/khrplatform.h) header files and produces a
library that loads the functions defined in these headers.

The tool that produces the library can be passed several options that will determine the shape of the library generated.
The library can be a ["header only"](https://en.wikipedia.org/wiki/Header-only) for the sake of simplicity or can
include both a normal `gla.c` and `gla.h` files that can be used to create a dynamic library or static library as
desired. A minimum and maximum profile version can be specified and a set of extensions that should be part of the
library. If a profile or extension is not part of the generated library then the types, defines and functions associated
with the profile or extension will not be present in the generated library.

## Running the CLI tool

The library is generated using the python script [gla_generator.py](gla_generator.py) with a CLI API:

```
$ python3 gla_generator.py -h
usage: gla_generator.py [-h] [--quiet] [--verbose] [--header_only] [--input_dir D] [--output_directory D] [--minimum_profile V] [--maximum_profile V] [--extension E]

gla generator script

optional arguments:
  -h, --help            show this help message and exit
  --quiet               quiet output
  --verbose             verbose output
  --header_only         generate a header-only library guarded by GLA_IMPLEMENTATION
  --input_dir D         the input directory
  --output_directory D  the output directory
  --minimum_profile V   the lowest OpenGL profile that the generated code will support
  --maximum_profile V   the highest OpenGL profile that the generated code will support
  --extension E         an extension that the generated code will support
```

## Bazel Integration

To integrate with the Bazel build tool add the following to your `WORKSPACE.bazel` file:

```python
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

_GLA_VERSION = "1.0"
_GLA_SHA256 = "b7c418a2a01150d044dfa2721241814595285b57935008e9f9e645c49325d575"

http_archive(
    name="gla",
    sha256=_GLA_SHA256,
    strip_prefix="gla-%s" % _GLA_VERSION,
    url="https://github.com/realityforge/gla/archive/refs/tags/v%s.tar.gz" % _GLA_VERSION,
)
```

And then add the following into the `BUILD.bazel` file for the package that will define the `gla` library:

```python
load("@gla//:rules.bzl", "gla_lib")

# Generate a non-header-only cc_library named "gla" that requires OpenGL profile 3.0 but will
# optionally load functions up to OpenGL profile 3.2. The generated library also supports the
# specified extensions
gla_lib("gla", False, "3.0", "3.2", [
    "GL_ARB_framebuffer_object",
    "GL_ARB_depth_clamp",
    "GL_ARB_seamless_cube_map",
    "GL_NVX_gpu_memory_info",
    "GL_ATI_meminfo",
    "GL_ARB_texture_compression_rgtc",
    "GL_ARB_texture_compression_bptc",
    "GL_EXT_texture_compression_s3tc",
    "GL_EXT_texture_filter_anisotropic",
    "GL_EXT_texture_sRGB",
    "GL_S3_s3tc",
])
```

## API Reference

The GLA API consists of the following functions:

* `int glaInit(void)`:  Initializes the library. Should be called once after an OpenGL context has
  been created. Returns `GLA_OK` when the library was initialized successfully and another value if there was an error.
  See the generated header for full documentation. It should be noted that this function will fail if the minimum
  profile requirement is not met by the loaded library.
* `int glaDispose(void)`:  Unloads the library. Should be called when the library is no longer required.
  Returns `GLA_OK` when the library was disposed successfully and another value if there was an error. (See the
  generated header for full documentation)
* `const char* glaError(void)`:  Return a string describing last error that occurred or NULL if no such error.
* `const char* glaErrorCodeToMessage(GLenum error_code)`:  Return a string describing the error code or NULL if unable
  to determine an appropriate error code. The `error_code` value is expected to be the value returned from a call to the
  OpenGL function `glGetError()`.

The library also generates defines of the form `GLA_VERSION_[Major]_[Minor]` that will evaluate to `true` if the profile
version `[Major].[Minor]` if the loaded library supports that profile. The defines will only be generated if the profile
is above the minimum version specified for the tool and below or equal to the maximum version.

The library also generates defines of the form `GLA_ARB_texture_compression_rgtc` if the loaded library declares that it
supports the `"GL_ARB_texture_compression_rgtc"` extension and the gla library was built with support for
the `GL_ARB_texture_compression_rgtc` extension.

# Contributing

GLA was released as open source so others could benefit from the project. We are thankful for any
contributions from the community. A [Code of Conduct](CODE_OF_CONDUCT.md) has been put in place and
a [Contributing](CONTRIBUTING.md) document is under development.

# License

GLA is licensed under [Apache License, Version 2.0](LICENSE).

# Credit

* This toolkit began as a patched variant of [gl3w](https://github.com/skaslev/gl3w) that was modified to make the build
  process hermetic and add a few additional features. It was gradually evolved over time until little remained of the
  original library. Credit goes to the [gl3w contributors](https://github.com/skaslev/gl3w#credits) for the idea and
  initial work.

# Alternatives

There are plenty of alternatives that should probably be considered before adopting this library.
The [OpenGL Loading Library](https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library) lists them.
