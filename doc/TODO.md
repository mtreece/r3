[//]: # (Copyright (C) 2016 Michael T. Reece.)
[//]: # ( )
[//]: # (This program is free software; you can redistribute it and/or modify)
[//]: # (it under the terms of the GNU General Public License version 2 as)
[//]: # (published by the Free Software Foundation.)

# TODOs and r3 Roadmap

## Administrative

* Create an "/AUTHORS" file.

* Establish a "/HACKING" file & guideline.


## Roadmap

### Python Bindings

* Investigate creating a, e.g., "${REPO}/lib/libpyr3/", which provides Python
  bindings (python3) to call into libr3. Note, "Rework Memory Management" may
  be a prereq for Python bindings. Need to investigate...

* Investigate if any tests can be improved (or better tests added) that can be
  powered by the Python bindings.


## Misc / Noteworthy TODOs

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
