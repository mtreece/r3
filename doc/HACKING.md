# HACKING on r3

The purpose of this file is to try to detail some of the necessary and helpful
pieces of information for hacking on r3. Contributors should treat this as the
"contributor-friendly" README and authoritative guide for submitting patches
upstream.


## Administrivia

## Code Style

Generally speaking, the "Linux kernel coding style" has worthy guidelines and
is an entertaining few minutes of reading material. This is true as of the time
of this commit. You should be able to find the latest version of the "Linux
kernel coding style" guide here:

    https://www.kernel.org/doc/Documentation/CodingStyle

Notable exceptions deviating from this coding style are briefly outlined here:

* tabs have many uses; r3 isn't one of them. Any existing tabs (where possible)
  are slated to be converted.

* generally speaking, braces should be used _even_ around a single-statement
  conditional. It's great knowing that the language makes this optional, but in
  my experience, you will almost always need the flexibility of inserting extra
  statements in that conditional (e.g. temporary printf). It's a pain having to
  add and remove braces in those cases, so just leave them in. You'll thank me.

## How to Contribute

### How to Commit

### Tests

## Misc Concerns
