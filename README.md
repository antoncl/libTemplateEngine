<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Introduction 
libTemplateEngine is a simple unicode awawre templating engine.

In the simplest form, the engine will use a dictionary to expand named values contained in a template. E.g.

```.html
<form>
    {{FIRSTNAME}}:<br>
    <input type="text" name="firstname">
    <br>
    {{LASTNAME}}:<br>
    <input type="text" name="lastname">
</form>
```
can be converted into:
```.html
<form>
  First name:<br>
  <input type="text" name="firstname">
  <br>
  Last name:<br>
  <input type="text" name="lastname">
</form> 
```

# Motivation 
I started writing C code back in the Kernigan and Richie days. I continued to write C and C++ code, although professionally more focus was given to C# with the advent of the .Net framework. Now, I find myself with a bit of extra spare time, and have decided to brush off my C++ skills.

In order to do that I decided on a small cross platform server project. One of the first tasks was cross platform capability of reporting performance counters. Remember Knuth's **premature optimization is the root of all evil**. Surprisingly I didn't find any readily available open source solutions, and decided to roll my own.

That library uses a simple DSL (domain specific language) to hide away the platform specific code. It became obvious to me that a the code generation could be simplified tremendously by using a simple template engine.

There are a lot of templating engines available. But none really matched my requirements, either they were huge, and required a lot of effort compared to what I needed done or they had severe issues working across platforms.

So, I decided to roll my own (is there a pattern here?). In retrospect it might have been quicker to go with a different code generation scheme, but as I started out say, I find myself with a bit of spare time on my hands.

# Goals 
I wanted a lightweight, simple, cross platform templating engine, with full unicode support.

Those requirements are quite fluffy, e.g. what does *lightweight* mean?

For my purposes lightweight means, no external dependencies! I've had to compromise a bit on this goal. The unit tests depends on [boost][Boost], the build environment depends on [cmake][CMake] and this documentation depends on [doxygen][doxygen]. The core library can be build without boost and doxygen.

Another aspect of lightweight is that is builds as a static link library, a .a or .lib file, rather than a .so or .dll file. I doubt that this library will gain such popularity that the hazzles of yet another install time component is worth the effort.

Cross platform in this context means that it can be built and run under both most Linuxes and Windows. It might build on other platforms, BSD and OSX springs to mind, but I have not verified this.

Full unicode support means just that. The actual text being expanded by the engine should support all 17 planes, and the engine should correctly expand all unicode based templates regardless of the glyphs being displayed. If I want hieroglyphics or klingon text, I should be able to get that.

# Licensing 
The code is available under the [GNU LGPL license][GnuLgpl]. Which basically means you are free to link to the binary code, but any changes to the code should be made publicly available. If you do modify the code, fix a bug or add an interesting feature I'd appreciate a pull request.

Licenses of dependencies

Application | License | Comment
------------|---------|----------
CMake       | BSD     | Required
Boost       | BSL     | Optional
Doxygen     | GPL     | Optional

All libraries - including libTemplateEngine - have no ownership of the output of the application. Who ever has the copyright of the input, also holds the copyright of the output.

# Building 
If you're an old hand at this, you probably only need to know that libTemplateEngine uses cmake and that it doesn't accept in-source builds.

The root CMakeLists.txt is located in the `src`folder.

During configuration it will try to locate boost and doxygen. If it finds boost it will enable building of the unit test framework, if it finds doxygen it will enable building of the documentation.

## Build script 
CMake is a great tool, but unless you perform an install, it leaves the build results in some - in my opinion - odd places. I doubt anyone would want to make a system wide install of libTemplateEngine, so I've created a small build script which performs the build and then installs things in a safe local location.

`./lib`                 - will contain the static libraries (.a/.lib).

`./libTemplateEngine`   - will contain the header files.

`./bin`                 - will contain the unit test executable, if it was built.

`./doc`                 - will contain the documentation, if it was built.

I know `libTemplateEngine` is a little unconventional, but this way - with proper specification of include folders - it is possibly to write something like this in your code:

```.cpp
#include <libTemplateEngine/TemplateEngine.hpp> 
```

you have a namespace for your include files! This way it is easy to see which library provides the header file.

### Windows 
The easiest way will be to use the `wibuild.cmd`file. 

1. Open a Visual Studio 2015 command line prompt
2. CD to the root of libTemplateEngine
3. Type `wibuild` and press enter.

The the script will use Visual Studio solutions and projects to build the binaries. The build will performed four times, once for: debug x86, release x86, debug x64 and finally release x64. When the build has completed the solution and project files can be found in the `build` directory.

### Linux 
The easiest way will be to use the `libuild`script. 

1. Open a shell/terminal window
2. CD to the root of libTemplateEngine
3. Type `libuild` and press enter.

# Concept 
libTemplateEngine operates in a two step process.

![Image depicting flow of data][ConceptSvg]

1. Compile the input stream into a list of template elements
2. Pull in a dictionary and render the compiled text.

Any text enclosed between **`{{`** and **`}}`**, *start-tag* and *end-tag* respectively, is considered to be a processing instruction. Any text found outside these tags is considered to be plain text, and is copied verbatim. Any text inside tags are parsed separately and is considered to be some kind of instruction.

Most of the following will focus on processing instructions since there is very little processing involved in handling the plain text components. Any plain text is copied from the input to the output almost verbatim, except for escape sequences.

# Escape sequences 
This concept of tagging - of course - begs the question of escaping, or how to have strings like "{{" passed through to the output stream without any processing. libTemplateEngine handles this the usual way, via the backslash **'\'** character. Although maybe not quite so usual. 


Source| .. | Destination
:----:|:--:|:----------:
\\{{  | -> |{{
\\}}  | -> |}}

Anything else is ignored. `\\{a}` will be passed on as plain text `\\{a}` and no escaping is done, only the character sequences `\\{{`and `\\}}` are treated specially. 


# Processing instructions 
The format of a template, including instructions, can be seen in this syntax diagram:

![TemplateSvg]

Where **PlainText** is defined as:

![PlainTextSvg]

*Note!* I know that from a formalistic viewpoint the above diagram doesn't make sense. What I wanted to show was that any unicode character can be used as plain text, except two left braces in a row and two right braces in a row, and how to escape those. I couldn't find a simple way of showing this without breaking a few formalistic rules. If you have a better solution, I'd love to hear it.

Only creativity sets a limit to what kinds of processing instructions could be implemented. libTemplateEngine only has three kinds of instructions (remember one of the goals was simplicity).

**Format of instructions**
![InstructionSvg]

The first is a simple dictionary lookup [expansion](#expansion).

The second [repeats](#repeat) the embedded template a number of times.

The third is a [comment](#comment) which can be used to clarify the tempate instructions.

## Expansion 
**Syntax:**

<b>{{</b>\<name\><b>}}</b>

**Purpose:**

Replace the given name with a value from the associated dictionary.

**Method:**

An expansion simply performs a lookup in the context using the embedded name and outputs the contents of the entry found.

If the name cannot be found an error is reported.

## Repeat 
**Syntax:**

<b>{{\#repeat</b> \<label\><b>}}</b>\<template\><b>{{/repeat}}</b>

**Purpose:**

A repeat duplicates/copies the embedded template zero or more times, as determined by the context.

**Method:**

A repeat instruction carries a label, assigned to it in the \<label\> part of the instruction. When the repeat is processed the context is queried for a value matching the label of the instruction. That value is expected to be a list of dictionaries, if not an error is reported.

The details and intricacies of [dictionary lists][RefDictionaryList] and the context is documented [else where](#context), but basic principle is that a dictionary entry may either be a simple text value or a list of dictionaries. 


The embedded template is copied once for each sub-dictionary in the dictionary list. If the dictionary list is empty, no copying is done.

## Comment 
**Syntax:**

<b>{{-</b>\<text\><b>}}</b>

**Purpose:**

Help document and clarify the template usage.

**Method:**
The text within the instruction is discarded.

# Context and dictionaries 
A [dictionary][RefDictionary] is a data-structure which associates a given name with a given value.

Name  | Value    
:-----|:---------------
A     | \<value for A\>
B     | \<value for B\>

A value can either be a simple UTF-16 string or a list of dictionaries.

Name  | Value    
:-----|:-----------------------
Name  | Rudolph Abel
Block | <table><tr><td>Dict-1</td><td>Dict-2</td><td>...</td><td>Dict-n</td></tr></table>

A [context][RefContext] can conceptually be thought of as a stack of dictionaries (Note! the stack is only implicitly expressed in the code). When the context is created it creates an internal dictionary containing some default values. The globally available values are defined [here][RefContextCTOR].

Prior to calling [render][RefTemplateRender] on the template a dictionary should be attached to the context using the [setDictionary][RefContextSetDictionary] method.

When the rendering is started the context pushes first the it's global dictionary onto the stack, then by the dictionary supplied via [setDictionary][RefContextSetDictionary] is pushed onto the stack.

Stack | Comment
:-----|:-------------------------
Main  |Assigned via [setDictionary][RefContextSetDictionary]
Global|System supplied values

Dictionary lookup is performed by walking the stack from top to bottom. First the topmost dictionary is queried, if the entry isn't found in that dictionary, then the next dictionary on the stack is queried, until either the value is found or the stack has been exhausted.


In the simple case, a template without any repeats, this is all that needs to be done. 

Repeat instructions use additional dictionaries. When a repeat instruction is rendered, the context if first queried for a value with the name of the repeat instruction. The value must exist and the value must be a dictionary list, otherwise an error is reported, and the rendering is aborted.

A dictionary list is a dictionary in it's own right, and may contain any number of values scoped to be shared with each sub dictionary in the list.

Once the dictionary list has been found, the list's internal dictionary is pushed onto the stack, then the list of sub dictionaries is iterated. for each iteration, the sub-dictionary is pushed onto the stack, and the embedded template is rendered using that context.

Stack | Comment
:-----|:-------------------------
Dict-i|The i'th sub-dictionary of the list
List  |The internal dictionary of the list
Main  |Assigned via [setDictionary][RefContextSetDictionary]
Global|System supplied values

# Error reporting 
All errors are reported by throwing an [exception][RefTemplateException]. In order to keep things simple only one kind of exception is defined. The exception inherits from the STL exception, and the actual error message can be queried via `what()`.

The reasoning behind using exceptions rather than returning booleans from various method calls is that - once the application has been debugged - errors should be a rare occurrence.

# Unicode and character sets 
As stated previously, unicode correctness is one of the design goals of this project. for this reason some deliberation has gone into the decision about the format of the internal string buffers.

Internally libTemplateEngine uses UTF-16 strings and characters. This was choice was made in order to strike a balance between memory footprint and the risk of polluting the internal structures with non-unicode strings.

Note! A dictionary has a method for [adding][RefAddStdString] a std::string to the dictionary. It is important that the string is guaranteed to contain a valid UTF-8 string, otherwise the dictionary will be polluted, and the resulting output may not be unicode correct.

# Scanner 
libTemplateEngine is doing the easy part, the real work in being unicode correct lies in the input and output mechanisms.

The task of managing the input is delegated to the [scanner][RefScanner]. The purpose of the scanner is to take the input template, and convert it to an UTF-16 stream of UTF-16 code units.

# UTF-8 and UTF-16 conversion 
On windows libTemplateEngine has a small [utility][RefConverter], called te_converter, which can convert between UTF-8 and UTF-16. To the best of my knowledge that conversion utility is correct. Although the c++11 standard has methods for converting between UTF-32 and its smaller counterparts I have been unable to link that code under windows. There appears to be a bug reported to MS about this, and although it is more than six months old it hasn't been patched yet. Consequently the library has no means of converting between UTF-32 and UTF-16.

# Examples 
I have included two example applications, which might help provide some insights into how everything fits together

The [first][RefExample1] is very simple and builds upon the example shown in the [introduction][Introduction].

The [second][RefExample2] shows how to use the [repeat][Repeat] instruction and [dictionary lists][RefDictionaryList].

[ConceptSvg]: https://rawgit.com/antoncl/libTemplateEngine/master/src/doc/concept.svg
[TemplateSvg]: https://rawgit.com/antoncl/libTemplateEngine/master/src/doc/template.svg
[InstructionSvg]: https://rawgit.com/antoncl/libTemplateEngine/master/src/doc/instruction.svg
[Boost]: http://www.boost.org/
[CMake]: https://cmake.org/
[doxygen]: http://www.stack.nl/~dimitri/doxygen/
[GnuLgpl]: http://www.gnu.org/licenses/lgpl-3.0-standalone.html
[Introduction]: #introduction
[Repeat]: #repeat

[RefDictionaryList]:./src/TemplateEngine/include/DictionaryList.hpp
[RefDictionary]: ./src/TemplateEngine/include/Dictionary.hpp
[RefContext]:./src/TemplateEngine/include/Context.hpp
[RefContextCTOR]:./src/TemplateEngine/include/Context.hpp
[RefTemplateRender]:./src/TemplateEngine/include/Template.hpp
[RefContextSetDictionary]:./src/TemplateEngine/include/Context.hpp
[RefTemplateException]:./src/TemplateEngine/include/TemplateException.hpp
[RefAddStdString]: ./src/TemplateEngine/include/Dictionary.hpp
[RefScanner]: ./src/TemplateEngine/include/Scanner.hpp
[RefConverter]: ./src/TemplateEngine/include/Types.hpp
[RefExample1]: ./src/examples/Simple/main.cpp
[RefExample2]: ./src/examples/List/main.cpp
