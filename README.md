
# ps1-4git

#### A git-aware section for your PS1 prompt

## What is it?

I wanted a git-aware prompt and was not able to get other things working
the way I wanted (my fault, definitely). On the other hand I wanted to
remind myself C coding (didn't do that since ~ 1993) so I decided to
combine these two into one -- code in C a super simple and lightweight
git-aware prompt add-on for my bash ``PS1``! ;)


## How does it look like?

(Working on providing screenshots, stay tuned.)


## Prerequisites

To be able to build the ``ps1-4git`` binary, you need a C compiler as well as
``make`` installed on your system.

The following will do that for you on Debian/Ubuntu:

```bash
$ sudo apt-get install build-essential make
```

## Build and install

To make use of this utility -- download, build and install:

```bash
$ git clone --depth=1 https://github.com/yassen-itlabs/git-goodies.git
$ cd git-goodies/ps1
$ make && sudo make install
$ source ~/.bashrc
```

**What happened during the install?**

The ``make install`` step should have added to your ``~/.bashrc`` a line
that appends ``ps1-4git`` output to your ``PS1`` prompt, similar to what is shown
below:

```bash
PS1="$PS1\$(git status 2>&1 | ps1-4git -n)"
```

The ``ps1-4git`` binary outputs such a line when called with ``--4bashrc``, like:

```bash
ps1-4git --4bashrc >> ~/.bashrc"  # make sure you do not add this multiple times
```

**What other command line arguments are supported?**

See ``ps1-4git --help``.


## Uninstall

When you decide you want to get rid of ``ps1-4git``, just go to the
``git-goodies/ps1`` directory (or clone it as shown in the _Build
and Install_ section) and execute ``sudo make uninstall``:

```bash
$ cd git-goodies/ps1
$ sudo make uninstall
```

After that, you can safely remove the repo directory itself like:

```bash
cd ~/parent-where-git-goodies-is/
rm -rf git-goodies
```


## Tests

You probably don't need to do this (unless you want to make changes to the code
and possibly submit a pull request). Still, if you need or want to, read further.

To be able to build and run the tests, you need ``CUnit`` development package
installed on your system.

On Debian/Ubuntu this can be done with:

```bash
$ sudo apt-get install libcunit1-dev
```

(tested and working on Ubuntu 18.04).

As soon as you have that, going to the ``ps1/`` directory and running ``make test``
should work:

```bash
$ cd git-goodies/ps1
$ make test
```

Expected output:

```
user@host:~/some-dir/git-goodies/ps1$ make test
gcc  -O2 -march=native ps1-4git.c ps1-4git-main.c tests/test-ps1-4git.c -Wall -Wfloat-equal -Wextra  -lcunit -o tests/test-ps1-4git
./tests/run-test.sh

All tests passed - OK
```

## License
Released under the **MIT** license.

Copyright (c) 2019, Yassen Damyanov

This program is free software: you can redistribute it and/or modify
it under the terms of the MIT License.

You should have received a copy of the MIT License along with this program
(see LICENSE text file).

If not, see <https://opensource.org/licenses/MIT>.


## Author

Primary author/maintainer (for now):

Yassen Damyanov <``yassen.damyanov.bg -AT- gmail.com``>
