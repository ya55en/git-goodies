
# ps1-4git

**A git-aware addition to your PS1 prompt**

## Build and install

### Prerequisites

TO be able to build the ``ps1-4git`` binary, you need a C compiler as well as
``make`` installed on your system. The follwoing will do that for you on
Debian/Ubuntu:

```bash
$ sudo apt-get install build-essential make
```

### Build and install

To make use of this module, download, build and install ():
```bash
$ git clone --depth=1 https://gitlab.itlabs.bg/open-source/git-goodies.git
$ cd git-goodies/ps1
$ make && sudo make install
```

The ``make install`` step should have added to your ``~/.bashrc`` a line that appends ``ps1-4git`` output to your PS1, similar to what is shown below:

```bash
PS1="$PS1\$(git status 2>&1 | ps1-4git -n)"
```

The ``ps1-4git`` binary outputs such a line when called with ``--4bashrc``, like:

```bash
ps1-4git --4bashrc >> ~/.bashrc"  # make sure you do not add this multiple times
```

Check also ``ps1-4git --help``.


## Tests

To be able to build and run the tests, you need ``CUnit`` development package installed on your system.

On Debian/Ubuntu this can be done with:

```bash
$ sudo apt-get install libcunit1-dev
```

(tested and working on Ubuntu 18.04).

As soon as you have that. going to the ``ps1/`` directory and running ``make test``
should work:

```bash
$ cd git-goodies/ps1
$ make test
```

Expected output:

```
user@host:~/some-things/git-goodies/ps1$ make test
gcc  -O2 -march=native ps1-4git.c tests/test-ps1-4git.c -Wall -Wfloat-equal -Wextra  -lcunit -o tests/test-ps1-4git
./tests/run-test.sh

All tests passed - OK
```


## Author

Primary (and only one for now) author is Yassen Damyanov <yassen.damyanov.bg@gmail.com>
