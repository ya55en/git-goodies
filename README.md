
# ps1-4git
**A git-aware addition to your PS1 prompt**

To make use of this module, download, build and install (you need a C compiler and ``make`` installed on your machine):
```
$ git clone --depth=1 https://gitlab.itlabs.bg/open-source/git-goodies.git
$ cd git-goodies/ps1
$ make && sudo make install
```

The ``make install`` step should have added to your ``~/.bashrc`` a line that appends ``ps1-4git`` output to your PS1, similar to what is shown below:

```
PS1="$PS1\$(git status 2>&1 | ps1-4git)"
```

The ``ps1-4git`` binary outputs such a line when called with ``--4bashrc``, like:

```
ps1-4git --4bashrc >> ~/.bashrc"  # make sure you do not add this multiple times
```

Check also ``ps1-4git --help``.
