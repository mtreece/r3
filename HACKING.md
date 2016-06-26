Copyright (C) 2016 Michael T. Reece.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

# HACKING on r3

The purpose of this file is to collectively detail some of the necessary and
helpful pieces of information for hacking on r3. Contributors should treat this
as the "contributor-friendly" README addendum and authoritative guide for
submitting patches upstream.

Note, this document may change often as lessons-learned are accumulated; be
sure to check back often to ensure you are familiar with any new guidance.

## Administrivia

* Changes should be carried out on branches. When merged upstream, they will be
  merged with the `--no-ff` option in order to always checkpoint the merger;
  this allows contributions to be captured in a cohesive unit.

  * "Mainline" development may routinely break this rule while trying to pull
    everything together; however, the long-term goal is to only advance the
    baseline with branch merges.

* Branch names should be formatted `${type}-${description}`, where `${type}`
  is one of ...

  * "rf" = "refactor"
  * "ft" = "feature"
  * "bf" = "bugfix"

  and `${description}` is a brief alphanumeric description string, using "\_"
  (underscore) in place of spaces.

## Code Style

Generally speaking, the "Linux kernel coding style" has worthy guidelines and
is an entertaining few minutes of reading material. This is true as of the time
of this commit. You should be able to find the latest version of the document
linked [here](https://www.kernel.org/doc/Documentation/CodingStyle).

Notable exceptions deviating from this coding style are briefly outlined here:

* Tabs have many uses; r3 isn't one of them. Any existing tabs (where possible)
  are slated to be converted. Makefiles are, of course, the exception where
  required.

* Braces should be used _even_ around a single-statement conditional. It's
  great knowing that the language makes this optional, but in my experience,
  you will almost always need the flexibility of inserting extra statements in
  that conditional (e.g. temporary printf). It's a pain having to add and then
  later remove braces when that happens, so leave them in. You'll thank me.

Additional guidelines:

* Files should be "self contained" regarding header includes. In other words,
  if a header references a particular type, it should (itself) `#include` any
  needed headers to define those types.

* All source files should `#include <config.h>` so as to appropriately
  configure a build. This should be the _first_ `#include` of a source file.
  The next block of includes should be any needed system header files, followed
  by a block of project/public (i.e. `#include <something.h>`) headers, finally
  followed by private/local (i.e. `#include "something.h"`) headers.

* Within logical groups, `#include` blocks should be sorted lexicographically.

* Finally, like Wikipedia, "ignore all rules" if it's appropriate to do so.
  Rules are only a (strong) guideline: if whatever you're trying to convey
  isn't properly demonstrated using the "rules" here, please express it in any
  way which is most effective. Just be sure you are capable of justifying your
  style (in the commit message, etc.) ;-).

## How to Contribute

If you are interested in contributing to r3, feel free to submit patches either
via the GitHub pull request feature or through email. Please make sure your
changes are tested in both "developer mode" as well as non-"developer mode",
and in any other configuration as appropriate. Ensure that the changes do not
break any tests (or if they do, please update existing tests as necessary). Any
new features, where possible, should also have tests written for them.

Please make sure each of your commits are "small", cohesive, and meaningful.
Commits should avoid introducing whitespace errors (see `git-diff --check`).
As much as possible, try to avoid messy commit history (e.g. messy merge
lineages). Make sure everything is spell-checked (commit messages included).
Finally, make sure your changes will merge cleanly with the latest upstream
master branch.

## Misc Concerns

If you have changes that you want to contribute that are significantly large,
you should consider reaching out first before implementing them, just to ensure
the changes fit nicely in the short and long-term roadmap plans.

Also note, if you peruse the git history of this repo, you may see commits that
aren't always "complete" or at that point in time compileable. This is usually
caused by a combination of 1) doing incremental commits, and 2) trying to build
a GitHub streak; however, these non-functional commits should only ever appear
on branches that are eventually merged --no-ff to master once complete.
