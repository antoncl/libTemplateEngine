# Introduction {#introduction}
libTemplateEngine is a simple unicode aware templating engine.

In the simplest form, the engine will use a dictionary to expand named values contained in a template. E.g.

~~~.html
<form>
    {{FIRSTNAME}}:<br>
    <input type="text" name="firstname">
    <br>
    {{LASTNAME}}:<br>
    <input type="text" name="lastname">
</form>
~~~
can be converted into:
~~~.html
<form>
  First name:<br>
  <input type="text" name="firstname">
  <br>
  Last name:<br>
  <input type="text" name="lastname">
</form> 
~~~

# Motivation {#motivation}
I started writing C code back in the Kernigan and Richie days. I continued to write C and C++ code, although professionally more focus was given to C# with the advent of the .Net framework. Now, I find myself with a bit of extra spare time, and have decided to brush off my C++ skills.

In order to do that I decided on a small cross platform server project. One of the first tasks was cross platform capability of reporting performance counters. Remember Knuth's **premature optimization is the root of all evil**. Surprisingly I didn't find any readily available open source solutions, and decided to roll my own.

That library uses a simple DSL (domain specific language) to hide away the platform specific code. It became obvious to me that a the code generation could be simplified tremendously by using a simple template engine.

There are a lot of templating engines available. But none really matched my requirements, either they were huge, and required a lot of effort compared to what I needed done or they had severe issues working across platforms.

So, I decided to roll my own (is there a pattern here?). In retrospect it might have been quicker to go with a different code generation scheme, but as I started out say, I find myself with a bit of spare time on my hands.

# Goals {#goals}
I wanted a lightweight, simple, cross platform templating engine, with full unicode support.

Those requirements are quite fluffy, e.g. what does *lightweight* mean?

For my purposes lightweight means, no external dependencies! I've had to compromise a bit on this goal. The unit tests depends on [boost](http://www.boost.org/ "link to the boost library homepage"), the build environment depends on [cmake](https://cmake.org/ "link to the cmake library") and this documentation depends on [doxygen](http://www.stack.nl/~dimitri/doxygen/ "link to the doxygen application"). The core library can be build without boost and doxygen.

Another aspect of lightweight is that is builds as a static link library, a .a or .lib file, rather than a .so or .dll file. I doubt that this library will gain such popularity that the hazzles of yet another install time component is worth the effort.

Cross platform in this context means that it can be built and run under both most Linuxes and Windows. It might build on other platforms, BSD and OSX springs to mind, but I have not verified this.

Full unicode support means just that. The actual text being expanded by the engine should support all 17 planes, and the engine should correctly expand all unicode based templates regardless of the glyphs being displayed. If I want hieroglyphics or klingon text, I should be able to get that.

# Licensing {#licensing}
The code is available under the [GNU LGPL license](http://www.gnu.org/licenses/lgpl-3.0-standalone.html "link to license text"). Which basically means you are free to link to the binary code, but any changes to the code should be made publicly available. If you do modify the code, fix a bug or add an interesting feature I'd appreciate a pull request.

Licenses of dependencies

Application | License | Comment
------------|---------|----------
CMake       | BSD     | Required
Boost       | BSL     | Optional
Doxygen     | GPL     | Optional

All libraries - including libTemplateEngine - have no ownership of the output of the application. Who ever has the copyright of the input, also holds the copyright of the output.

# Concept {#concept}
libTemplateEngine operates in a two step process.

![Image depicting flow of data][ConceptSvg]

@diafile ./concept.dia "" width=6cm

1. Compile the input stream into a list of template elements
2. Pull in a dictionary and render the compiled text.

Any text enclosed between **`{{`** and **`}}`**, *start-tag* and *end-tag* respectively, is considered to be a processing instruction. Any text found outside these tags is considered to be plain text, and is copied verbatim. Any text inside tags are parsed separately and is considered to be some kind of instruction.

Most of the following will focus on processing instructions since there is very little processing involved in handling the plain text components. Any plain text is copied from the input to the output almost verbatim, except for escape sequences.

This concept of tagging - of course - begs the question of escaping, or how to have strings like "{{" passed through to the output stream without any processing. libTemplateEngine handles this the usual way, via the backslash **'\'** character.

Source| .. | Destination
:----:|:--:|:----------:
\\{   | -> |{
\\}   | -> |}
\\\\  | -> |\\


A stand alone backslash '\' or a backslash in any other combination will trigger an error during parsing.


# Processing instructions {#processingInstruction}
The format of a template, including instructions, can be seen in this syntax diagram:

\image html template.svg "Copy anything which isn't an instruction to the output"

Only creativity sets a limit to what kinds of processing instructions could be implemented. libTemplateEngine only has three kinds of instructions (remember one of the goals was simplicity).

\image html instruction.svg "Format of instructions"

The first is a simple dictionary lookup [expansion](#PI_Expansion).

The second [repeats](#PI_Repeat) the embedded template a number of times.

The third is a [comment](#PI_Comment) which can be used to clarify the tempate instructions.

## Expansion {#PI_Expansion}
**Syntax:**

<b>{{</b>\<name\><b>}}</b>

**Purpose:**

Replace the given name with a value from the associated dictionary.

**Method:**

An expansion simply performs a lookup in the context using the embedded name and outputs the contents of the entry found.

If the name cannot be found an error is reported.

## Repeat {#PI_Repeat}
**Syntax:**

<b>{{\#repeat</b> \<label\><b>}}</b>\<template\><b>{{/repeat}}</b>

**Purpose:**

A repeat duplicates/copies the embedded template zero or more times, as determined by the context.

**Method:**

A repeat instruction carries a label, assigned to it in the \<label\> part of the instruction. When the repeat is processed the context is queried for a value matching the label of the instruction. That value is expected to be a list of dictionaries, if not an error is reported.

The details and intricacies of [dictionary lists](@ref template_engine::DictionaryList) and the context is documented [else where](#Context), but basic principle is that a dictionary entry may either be a simple text value or a list of dictionaries. 


The embedded template is copied once for each sub-dictionary in the dictionary list. If the dictionary list is empty, no copying is done.

## Comment {#PI_Comment}
**Syntax:**

<b>{{-</b>\<text\><b>}}</b>

**Purpose:**

Help document and clarify the template usage.

**Method:**
The text within the instruction is discarded.

# Context and dictionaries {#Context}
A [dictionary](@ref template_engine::Dictionary) is a data-structure which associates a given name with a given value.

Name  | Value    
:-----|:---------------
A     | \<value for A\>
B     | \<value for B\>

A value can either be a simple UTF-16 string or a list of dictionaries.

Name  | Value    
:-----|:-----------------------
Name  | Rudolph Abel
Block | <table><tr><td>Dict-1</td><td>Dict-2</td><td>...</td><td>Dict-n</td></tr></table>

A [context](@ref template_engine::Context) can conceptually be thought of as a stack of dictionaries (Note! the stack is only implicitly expressed in the code). When the context is created it creates an internal dictionary containing some default values. The globally available values are defined [here](@ref template_engine::Context::Context).

Prior to calling [render](@ref template_engine::Template::render) on the template a dictionary should be attached to the context using the [setDictionary](@ref template_engine::Context::setDictionary) method.

When the rendering is started the context pushes first the it's global dictionary onto the stack, then by the dictionary supplied via [setDictionary](@ref template_engine::Context::setDictionary) is pushed onto the stack.

Stack | Comment
:-----|:-------------------------
Main  |Assigned via [setDictionary](@ref template_engine::Context::setDictionary)
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
Main  |Assigned via [setDictionary](@ref template_engine::Context::setDictionary)
Global|System supplied values

# Error reporting {#error_reporting}
All errors are reported by throwing an [exception](@ref template_engine::TemplateException). In order to keep things simple only one kind of exception is defined. The exception inherits from the STL exception, and the actual error message can be queried via `what()`.

The reasoning behind using exceptions rather than returning booleans from various method calls is that - once the application has been debugged - errors should be a rare occurrence.

# Unicode and character sets {#Unicode}
As stated previously, unicode correctness is one of the design goals of this project. for this reason some deliberation has gone into the decision about the format of the internal string buffers.

Internally libTemplateEngine uses UTF-16 strings and characters. This was choice was made in order to strike a balance between memory footprint and the risk of polluting the internal structures with non-unicode strings.

Note! A dictionary has a method for [adding][AddStdString] a std::string to the dictionary. It is important that the string is guaranteed to contain a valid UTF-8 string, otherwise the dictionary will be polluted, and the resulting output may not be unicode correct.

# Scanner {#Scanner}
libTemplateEngine is doing the easy part, the real work in being unicode correct lies in the input and output mechanisms.

The task of managing the input is delegated to the [scanner][Scanner]. The purpose of the scanner is to take the input template, and convert it to an UTF-16 stream of UTF-16 code units.

# UTF-8 and UTF-16 conversion {#char_conversion}
On windows libTemplateEngine has a small [utility][Converter] which can convert between UTF-8 and UTF-16. To the best of my knowledge that conversion utility is correct. Although the c++11 standard has methods for converting between UTF-32 and its smaller counterparts I have been unable to link that code under windows. There appears to be a bug reported to MS about this, and although it is more than six months old it hasn't been patched yet.

Consequently the library has no means of converting between UTF-32 and UTF-16.

[ConceptSvg]:https://rawgithub.com/potherca/StackOverflow/gh-pages/question.13808020.include-an-svg-hosted-on-github-in-markdown/controllers_brief.svg

[AddStdString]: @ref template_engine::Dictionary::add(const template_engine::te_string, const std::string&)
[Scanner]: @ref template_engine::Scanner
[Converter]: @ref template_engine::te_converter
