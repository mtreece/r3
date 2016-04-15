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

* finally, like Wikipedia, "ignore all rules" if it's appropriate to do so.
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

### How to Commit

### Tests

## Misc Concerns
