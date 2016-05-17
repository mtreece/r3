Copyright (C) 2016 Michael T. Reece.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

# TODOs and r3 Roadmap

The intent of this file is to capture noteworthy TODOs and an overall roadmap
for r3. As ideas are implemented, they should be _deleted_ from this file and
the deletion performed in the branch which implements the change. There's no
point in keeping history to this file: that's what git is for.


## Administrative

The intent of this section is to capture miscellaneous TODOs that affect the
overall health of the project.

* Create an "/AUTHORS" file.
    * Determine format: does including email address increase spam activity to
      the mailbox?

* Create a friendly "/README" to describe what's going on here.

* Better document the algorithm.

* Consider bumping required version of libcheck to support ck_assert_int_eq;
  see commit 8e2d7e1071005feaf230fe8867095c89c366fa79 for more details.


## Roadmap

Long-term ideas for advancing r3 or adding unique features.

### Python Bindings

* Investigate creating a, e.g., "${REPO}/lib/libpyr3/", which provides Python
  bindings (python3) to call into libr3. Note, "Rework Memory Management" may
  be a prereq for Python bindings. Need to investigate...

* Investigate if any tests can be improved (or better tests added) that can be
  powered by the Python bindings.

### Graphics

* Investigate the creation of a library for graphically drawing the cubes
  created with r3. (It may be better to "outsource" this, depending on what's
  available.)


## Misc / Noteworthy TODOs

### General

Miscellaneous ideas that could have a large-impact on the codebase.

* See if it's possible to refactor the algorithm to either remove synclinks,
  allow it to optionally be compiled in, allow it to optionally be used (at
  runtime), or optionally link against a dictionary API. Why have the optional
  configurations? Depending on the primary use-case of r3, it may help reduce
  (via tradeoff) move operations vs lookup operations.

* Consider breaking out make-check tests into separate executables. The problem
  currently is that the "Testsuite Summary" only displays statistics for the
  one test executable that is created -- either a single PASS or FAIL, which is
  annoying.

* Compiling with `--enable-developer-mode` appears to break `make dist-*`. Need
  to investigate and fix. (It could just be a "don't do this" type of thing?)


### Aesthetics & Misc Cleanup

TODOs which can apply in multiple places, codebase-wide, that affect overall
aesthetics and aesthetics-related health of the project.

* Add missing [in], [out], [in,out] arguments to doxygen.
