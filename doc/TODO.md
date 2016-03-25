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

* Establish a "/HACKING" file & guideline.

* Create a friendly "/README" to describe what's going on here.


## Roadmap

Long-term ideas for advancing r3 or adding unique features.

### Python Bindings

* Investigate creating a, e.g., "${REPO}/lib/libpyr3/", which provides Python
  bindings (python3) to call into libr3. Note, "Rework Memory Management" may
  be a prereq for Python bindings. Need to investigate...

* Investigate if any tests can be improved (or better tests added) that can be
  powered by the Python bindings.


## Misc / Noteworthy TODOs

Miscellaneous ideas that could have a large-impact on the codebase.

* "Rework Memory Management". Currently, users of the API (usually) need to
  have an open definition of struct r3cube in order to declare an instance and
  then pass a pointer to that instance to the API (init + operations). This is
  somewhat problematic, as it makes it harder to change the internals of the
  structs without breaking some sort of dependency. I'm reluctant to modify r3
  to instead make calls to malloc in a "init" / "close" pattern (where "init"
  calls into a "well-known" malloc/calloc-like function, and "close" calls into
  a free-like function), since that's outside the scope of what this library
  should do. Furthermore, I don't want to limit r3 from running in limited,
  embedded environments which may not have malloc-like calls. The "solution" to
  this will be to either 1) allow the API to be queried for the struct r3cube
  size (so the user has to manage memory), or 2) allow the API to be passed a
  pair of "malloc" and "free" functions that are then called in "init" and a
  new "free" API function. The former would be easier, but the latter would be
  more versatile and allow for future-thinking shutdown hooks.

* See if it's possible to refactor the algorithm to either remove synclinks,
  allow it to optionally be compiled in, allow it to optionally be used (at
  runtime), or optionally link against a dictionary API. Why have the optional
  configurations? Depending on the primary use-case of r3, it may help reduce
  (via tradeoff) move operations vs lookup operations.
