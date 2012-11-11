# ARGLE
## Automatic Requirements for GL Extensions

There are many useful libraries to make handling portable OpenGL loading easier. [GLEW](http://glew.sourceforge.net) and [The OpenGL Loader Generator](https://bitbucket.org/alfonse/glloadgen/wiki/Home) both do this job admirably. However, in order to check for requirements or build a custom loader, you must first know what you are trying to load.

Many "argles" have been bargled in this quest, and ARGLE hopes to help.

ARGLE is a code scanner that takes C or C++ source, extracts all things that look like calls to OpenGL functions, cross-references them with the relevant extension or core version of OpenGL, and produces a summary report.

It does not, at present, treat specially extensions of OpenGL that are "guaranteed" to be part of later versions. It also does not attempt to deduce the versions of GLSL (if any) used. This means that, for instance, the first few tutorials in [Learning Modern 3D Graphics Programming](http://arcsynthesis.org/gltut) will be reported as requiring OpenGL 2.0 and GL_ARB_vertex_array_object, instead of OpenGL 3.3. In practice, this should actually serve to minimize the amount of the API you import, restricting it to the things you are actually using.
