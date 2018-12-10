# 42sh

CI: [![Build Status](https://travis-ci.org/jjaniec/42sh.svg?branch=develop)](https://travis-ci.org/jjaniec/42sh)

Code reviews: [![Codacy Badge](https://api.codacy.com/project/badge/Grade/a4c61474f5c2439086efc8455b0817c4)](https://www.codacy.com/app/jjaniec/42sh?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=jjaniec/42sh&amp;utm_campaign=Badge_Grade) [![CodeFactor](https://www.codefactor.io/repository/github/jjaniec/42sh/badge)](https://www.codefactor.io/repository/github/jjaniec/42sh) [![Sonarcloud](https://sonarcloud.io/api/project_badges/measure?project=jjaniec_42sh&metric=alert_status)](https://sonarcloud.io/dashboard?id=jjaniec_42sh) [![Sonarcloud2](https://sonarcloud.io/api/project_badges/measure?project=jjaniec_42sh&metric=ncloc)](https://sonarcloud.io/dashboard?id=jjaniec_42sh)

Coverage: [![codecov](https://codecov.io/gh/jjaniec/42sh/branch/develop/graph/badge.svg)](https://codecov.io/gh/jjaniec/42sh)

[![Open in Cloud Shell][shell_img]][shell_link]

[shell_img]: http://gstatic.com/cloudssh/images/open-btn.png
[shell_link]: https://console.cloud.google.com/cloudshell/open?git_repo=https://github.com/jjaniec/42sh&page=editor&open_in_editor=./README.md

---

## Installation & makefile rules

- Compiling the project

```shell=
git clone https://github.com/jjaniec/42sh
make
```

- Execute unit tests:

```shell=
make tests
./42sh_tests
```
###### Get our testing library: [zorgnax/libtab](https://github.com/zorgnax/libtap.git)

-  Make coverage reports

```shell=
make fclean coverage
./42sh_tests
```

- Enable permanent verbose mode (Mainly used for debugging)

```shell=
make fclean verbose
./42sh
```

###### Get our logging library: [rxi/log.c](https://github.com/rxi/log.c)

---

## Usage & options

```shell=
./42sh --help
Usage:       42sh [-hcGv] [-c "command"] file
  -h, --help      Print help and exit
  -c              Non-interactive mode: Execute command lines passed as parameters
  -G              Disable syntax highlighting
  -v, --verbose   Enable (verbose/debug) mode
  --le-debug      Enable line edition debugging in /dev/ttys000 (value of TTY_DEBUG in line_edition.h)
```
- Pass files as arguments without non-interactive mode enabled to interprete commands in those files

# Features

## Command operators

#### Separators

- Logical operators (`&&`, `||`) and `;`

#### Pipes

- `|` or `|&` (ca c'est pas encore fait mais on va voir pask ca peut se faire (tres) vite)

#### Redirections: 

- `>`, `2>`, or other file descriptors: `2>&`

- `>>` Append content of filedesc to a file

- File descriptor closing: `>&-` / `<&-`, `2>&-` will close `2` while `2>&1-` will close `1`

- Less commonly used redirections: `<>`
 (like `2 <> file` to point fd `2` to `file`)

##### Heredocs

- File heredocs: `command < file`

- Multi-line heredocs: `command <<EOF`

- One-line heredocs: `command <<<SOME_RANDOM_TEXT_TO_STDIN`

#### Temporary env variables assignements before program name

- `FOO=BAR VAR=VAR1 env`

#### Local variables assignations

- Without program specified, env variables assignations will create local variables

#### Inhibitors

- Simple quotes: `'`

- Double quotes: `"`

- Backslashes (Escape character): `\`

#### Shell scripting

- `if`, `elif`, `else`, `then`, `fi`

- `while`, `do`, `done`

- `break`, `continue`

###### More details for shell scripting [here](#ShellScripting)

#### `!` token


- `!!` Becomes the last command executed
-  `!n` Becomes the command number `n` in historydevient la commande numero n de l'historique
-  `!-n` Becomes the command number `n` in history, starting at the end
-  `!str` Becomes the last command executed starting with `str`


## Line Edition

- Multi-line line edition with syntax highlighting

- Subprompts with commands ending with ``&&``  ``||``  ``\``  ``|``  ``""``  ``''``  ``<<`` 

#### Shortcut list

```
arrow right : cursor moves right
arrow left : cursor moves left
arrow up : history previous
arrow down : history !previous
ctrl + a : cursor goes to cmd first character
home : same
ctrl + e : cursor goes to cmd last character
end : same
alt + arrow right : cursor moves to the next word
alt + arrow left : cursor moves to the previous word
backspace : delete the previous character
delete : delete the currently pointed character
ctrl + arrow up : cursor moves one line up
ctrl + arrow down : cursor moves one line down
ctrl + b : copy cmd, cursor to start
ctrl + f : copy cmd, cursor to end
ctrl + r : copy all cmd
ctrl + p : past cmd copied
ctrl + u : delete currently written cmd
ctrl + dash : cut all cmd
ctrl + [ : cut cmd, cursor to start
ctrl + ] : cut cmd, cursor to end
ctrl + d : EOF / exit
ctrl + l : clear screen
tabulation : launch autocompletion
```

## Syntax Highlighting

[![SyntaxHighlighting](http://i.imgur.com/UvZLsCi.png)](i.imgur.com/UvZLsCi.png)

- Can be disabled at start with the `-G` option or with the `syntax` builtin command
```shell=
syntax [on|off]
```

#### Default formating list w/ corresponding defines in `syntax_highlighting.h`

```
Program found in hashtable:
   "\e[1;32m" (COL_PROG_NAME_FOUND)
Program not found in hashtable:
   "\e[1;31m" (COL_PROG_NAME_NOT_FOUND)
Word beggining by a '-' like program options:
   "\e[0;36m" (COL_PROG_OPT)
Program argument corresponding to a file:
   "\e[2;49;32m" (COL_PROG_ARG_FILE)
Program argument corresponding to a directory:
   "\e[2;49;33m" (COL_PROG_ARG_DIR)
Or else:
   "\e[0;49;31m" (COL_PROG_ARG_NOT_FOUND)
Program is an alias:
   "\e[36m" (COL_ALIAS)
Quoted strings:
   "\e[0;33m" (COL_QUOTED_ARG)
Operators:
   "\e[0;96m" (COL_OPERATORS)
Redirections:
   "\e[2;49;39m" (COL_REDIRS) 
Environnement/Local variable expansion:
   "\e[2;49;39m" (COL_ENV_VAR_EXPANSION)
Reserved/Shell script token:
   "\e[1;2m" (COL_SCRIPT)
```

## Zsh like auto-completion

<a href="https://asciinema.org/a/a9dNz7LOjWMecmT9RbcFXKvel" target="_blank"><img src="https://asciinema.org/a/a9dNz7LOjWMecmT9RbcFXKvel.svg" /></a>

- Key-bindings: 
    - <tab>: start auto-completion / select next item
    - <arrow-keys>: navigate in menu
    - <return>: validate selected item

- Standard auto-completion when only one item match
- Auto-completion with menu shown when multiple items match

## History

```shell=
42sh: Usage : history [n | -d n | --clear | --save]
```

- History file loaded at startup 

- By default the history file is placed at `$HOME/.42sh_history`

## Hash-table

- Load all binary found in $PATH
- Hash-table will be updated:
    - If `$PATH`'s value changes
    - If the content of a directory referenced in $PATH is changed

## [Aliases](#Aliases)


```shell=
42sh: usage: alias [-d] key value | --save
```
- Aliases cannot expand recursively.

- By default the aliases file is placed at `$HOME/.42sh_aliases`

- Create/delete aliases with the `alias` builtin

- `unalias key1 key2 key3 ...`


## Expansions

- Expansions of environnement / Local / Internal variables

##### Default Internal variables list:

```shell=
$$: Shell pid
$!: jse pu c koi lol
$IFS: Internal Field Separators
$UID: uid of current user
$SHELL
$42SH_VERSION
$SHLVL
```

## <a name="ShellScripting">Shell scripting</a>

##### Template of condition construction
```shell=
IF [condition] ; 
    THEN [command-line] ; 
ELIF [condition] ; 
    THEN [command-line] ; 
ELSE 
    [command-line] ;
FI
```
     
##### Template of loop construction
```shell=
WHILE [condition] ; 
    DO [command-line] ; 
DONE
```
- Particularities:
    - A `[condition]` block must only begin with a command.
    - A `[condition]` block can be as complex as a `[command-line]`.
    - All script tokens are case-insensitive.
    - `THEN` and `DO` tokens must not be followed by a separator.
        - `.... THEN ls && pwd; ..... # GOOD`
        - `.... THEN ; ls && pwd; ..... # WRONG`
    

- The keyword `continue` stop the execution of the end of `[command-line]` in the loop, and start again checking the `[condition]` block.
- The keyword `break` stop the execution of `[command-line]` in the loop, go to the outer corresponding token `DONE` and start executing what is after the loop.
- You can integrate and add logic tokens for a whole script construction:
```shell=
ls &&  IF [condition]; THEN [command-line]; FI  || pwd # GOOD
```
- But you cannot integrate a whole script construction with redirections:
```shell=
ls >  IF [condition]; THEN [command-line]; FI  2>1 | cat -e # BAD
```
- You can nest whole shell script construction inside other shell script constructions, in `[condition]` or `[command-line]` blocks.
- The return value of a shell script construction is the return value of the last command executed.
## Detailled (POSIX) builtin list

#### Env / Setenv / Unsetenv

- env:
    ```shell=
    [-h], [-help], Print help and exit
	[-i], Invoke utility with exactly the environment specified by the arguments; the inherited environment will be ignored completely.
    ```
- setenv (aliased to `export`):
    ```shell=
    usage setenv VAR1=VALUE1 VAR2=VALUE2 ...
    ```
- unsetenv (aliased to `unset`):
    ```shell=
    usage unsetenv VAR1NAME VAR2NAME ...
    ```


#### Echo

```
echo [string ...]
\a
Write an <alert>.
\b
Write a <backspace>.
\c
Suppress the <newline> that otherwise follows the final argument in the output. All characters following the '\c' in the arguments shall be ignored.
\f
Write a <form-feed>.
\n
Write a <newline>.
\r
Write a <carriage-return>.
\t
Write a <tab>.
\v
Write a <vertical-tab>.
\\
Write a backslash character
```

#### Cd

```shell=
[-h], [-help], Print help and exit
[-P], Handle the operand dot-dot physically; symbolic link components shall be resolved before dot-dot components are processed.
[-L], Handle the operand dot-dot logically; symbolic link components shall not be resolved before dot-dot components are processed
```

#### Exit

- When a valid numeric argument is specified, exit with passed value

- If multiple arguments are specified, an `too many arguments` error will be printed

- If a non-numeric value is specified as argument, return code will be set to `255` and an `invalid arguments` error will be printed

## Other (not POSIX compliant) builtin list

#### Test aka `[`

```shell=
Usage:   42sh: test, [ --
                test expression
                [ expression ]
                ! expression
                If expression is an integer, it becames the value returned.
        -h      -h:             Print this help with all the options listed.
        -b      -b file:        True if file exists and is a block special file.
        -c      -c file:        True if file exists and is a character special file.
        -d      -d file:        True if file exists and is a directory.
        -e      -e file:        True if file exists (regardless of type).
        -f      -f file:        True if file exists and is a regular file.
        -k      -k file:        True if file exists and its sticky bit is set.
        -p      -p file:        True if file is a named pipe (FIFO).
        -r      -r file:        True if file exists and is readable.
        -s      -s file:        True if file exists and has a size greater than zero.
        -u      -u file:        True if file exists and its set user ID flag is set.
        -w      -w file:        True if file exists and is writable.
                                True indicates only that the write flag is on.
                                The file is not writable on a read-only file system even if this test indicates true.
        -x      -x file:        True if file exists and is executable.
                                True indicates only that the execute flag is on.
                                The file is not executable on a read-only file system even if this test indicates true.
        -L      -L file:        True if file exists and is a symbolic link.
        -S      -S file:        True if file exists and is a socket.
        ---------------------------
        =       s1 = s2:        True if the strings s1 and s2 are identical.
        !=      s1 != s2:       True if the strings s1 and s2 are not identical.
        ---------------------------
        -eq     n1 -eq n2:      True if the intengers n1 and n2 are algebraically equal.
        -ne     n1 -ne n2:      True if the intengers n1 and n2 are not algebraically equal.
        -gt     n1 -gt n2:      True if the intengers n1 is algebraically greater than the integer n2.
        -ge     n1 -ge n2:      True if the intengers n1 is algebraically greater than or equal to the integer n2.
        -lt     n1 -lt n2:      True if the intengers n1 is algebraically less than the integer n2.
        -le     n1 -le n2:      True if the intengers n1 is algebraically less than or equal to the integer n2.
```

- When test is executed without argument : `[ ]`, return false.
- When test is executed with one argument : `[ foo ]`, return true.


#### Read

```shell=
42sh: read: usage read [-d 'x'] [[-n | -N] x] -p -s -- VAR1 VAR2 ...
	-d x : change the delimiter ('\n' by default) to x.
	-n x : read stops if receiving the delimiter character or if it
	    has read x characters. (not compatible with option -N).
	-N x : ignore the delimiter character, read stops if it has read
	    x character or encountering EOF. (not compatible with option -n).
	-p : a prompt is displayed before reading.
	-s : the input is not displayed.
```
- Wait for the user's input, then split the input into words using IFS, then the words are stored in the shell variables given as arguments.

- If there is more words than variables to store them, the last variable will have all the remaining words.

- If there is no variable given as argument, the REPLY variable is used.
