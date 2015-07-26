# The CowPack archive format
WIP version  
Author: Loukas Agorgianitis (TheArtist)

## Table of Contents
  1. [Introduction](#intro)
  2. [The Format](#theformat)
     1. [Conventions](#conventions)
     2. [General archive layout](#generallayout)
     3. [The Header](#header)
     4. [The Filetable](#filetable)
     5. [The Filedata](#filedata)
  3. [Sample source code](#samplesource)
     1. [Fetching files](#fetchingfiles)
     2. ...
  4. [Revisions](#revisions)
  5. [Licensing](#licensing)

## <a name="intro"/> 1. Intro
The CowPack format is a binary archive format that hasn't yet been used anywhere.
Its main purpose is to be used as a game resource packing format
so its design will be optimized for random read access of its data.

## <a name="theformat"/> 2. The format

### <a name="conventions"/> 2.1 Conventions
* All numbers in the CowPack format are in little endian byte order; signed numbers use the two's complement system.  
* All sizes and offsets are in bytes, unless specified otherwise.  
* All paths and filenames are stored as ASCII strings, so there is no Unicode support and neither will be for simplicity's sake.  
* All paths are stored using forward slashes, and filenames should obay the users' system rules.  

### <a name="generallayout"/> 2.2 General archive layout
The general format of the archive's layout is illustrated at the following diagram:

<pre>
.----------------------.
|  CowPack File Header | [1]
'----------------------'
|                      |
|      File Table      | [2]
|                      |
'----------------------'
|                      |
|                      |
|       File Data      | [3]
|                      |
|                      |
'----------------------'
</pre>

\[1\]: The header contains generic information about the archive, such as the version of the format, the size of the whole archive etc.  
\[2\]: The filetable section contains information related to the files and the directories stored in the archive,
such as the file/directory names and the file data offsets in the filedata section.  
\[3\]: The filedata section contains the concatenated contents of the files contained in the archive.

### <a name="header"/> 2.3 The Header
The header consists of the following:
<pre>
.---------------------------------------------.
| Signature | Version | Size of total archive |
'---------------------------------------------'
     [1]        [2]             [3]
</pre>

Where:

\[1\]: The Singature is composed of 4 bytes: the hexadecimal value of DA and the ascii value of the word "COW". 
The resulting value should be the bytes: DA 43 4F 57.

\[2\]: The Version is a two byte number holding the current version of the format.

\[3\]: The Size of the total archive is a four byte number that holds the sum of the header, filetable and filedata sections' size.

After that additional 2 FF bytes follow. This totals the size of the header into 12 bytes.

### <a name="filetable"/> 2.4 The Filetable
TODO

### <a name="filedata"/> 2.5 The Filedata
TODO

## <a name="samplesource"/> 3. Sample source code
TODO

### <a name="fetchingfiles"/> 3.1 Fetching files
TODO

## <a name="revisions"/> 4. Revisions
TODO

## <a name="licensing"/> 5. Licensing
The MIT License (MIT)

Copyright (C) 2015 Agorgianitis Loukas <agorglouk@gmail.com> 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
