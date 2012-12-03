# ARGLE
## Automatic Requirements for GL Extensions

There are many useful libraries to make handling portable OpenGL loading easier. [GLEW](http://glew.sourceforge.net) and [The OpenGL Loader Generator](https://bitbucket.org/alfonse/glloadgen/wiki/Home) both do this job admirably. However, in order to check for requirements or build a custom loader, you must first know what you are trying to load.

Many "argles" have been bargled in this quest, and ARGLE hopes to help.

ARGLE is a code scanner that takes C or C++ source, extracts all things that look like calls to OpenGL functions, cross-references them with the relevant extension or core version of OpenGL, and produces a summary report.

If an extension of OpenGL is *guaranteed* to be part of later versions, it will also note the highest revision of OpenGL that contains such extensions, and lists any that remain past that point. This can be useful for retrofitting easy-to-communicate requirements to existing code. For example, the first few tutorials in [Learning Modern 3D Graphics Programming](http://arcsynthesis.org/gltut) will be reported as requiring OpenGL 2.0 and GL_ARB_vertex_array_object, but it will then note that all such calls are contained in OpenGL 3.0. (These programs actually require OpenGL 3.3 to *run*, but this is because of run-time configuration and the shaders that it loads from external files. ARGLE cares only about functions that you directly compile against.)

## Syntax

ARGLE's syntax is pretty simple:

```
argle [-v] [filenames]
```

Using the `-v` option sets "verbose" mode where it justifies the requirements by listing each function name it found and the extension it corresponds to. The filenames are the filenames scanned. If you set `-` as a filename, it will read from standard input. That means that you can check the minimum version of an unfamiliar token in someone else's code with a command like:

```
echo glBindBufferBase | argle -
```

If a token doesn't correspond to anything that ARGLE recognizes, it will report a minimum OpenGL requirement of 1.1. This doesn't necessarily mean that OpenGL 1.1 features are in use; however, it is the minimum requirement for all extant OpenGL loader programs.
