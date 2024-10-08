termios
=======

These tests are brought to you by the letter `q'.

When you start the test, you should see:

  You have the following choices:
    1 - Reset the struct termios
    2 - Look at the current termios setting
    3 - Change the line characteristics
    4 - Test canonical input
    5 - Test raw input
    9 - Exit
  Enter your choice (1 to 5 or 9, followed by a carriage return): 

The individual tests are briefly described below:


1. Reset the struct termios.

Included just in case you get into trouble. More than likely, if you are in
trouble, neither input nor output are likely to work and this won't help. But
hey, it should give you some warm fuzzy feeling that its there...


2. Look at the current termios setting

Dumps the current state of the termios settings in hex and with symbolic flag
names.


3. Change the line characteristics

Allows you to change the line speed, parity, number of data bits and number of
stop bits. You must supply a delay before the change takes effect. This gives
you time to switch your terminal settings to continue with the test.

WARNING: Minicom under Linux gets extremely unhappy (as does the /dev/ttyS?
underlying devices) if you change the line characteristics and do not make the
corresponding change in the terminal emulator.


4. Test canonical input

Simple test of canonical or cooked input mode. Try typing some tabs and/or control characters and make sure that you can backspace over them properly.


5. Test raw input

The line is placed into raw mode and four separate test are done:

VMIN=0, VTIME=0
        Each letter you type should produce a line of output.
        The `count' should be quite large, since (as you correctly
        pointed out) the read is non-blocking.  The time should be
        the interval between typing characters.
        Type a `q' to finish the test.
VMIN=0, VTIME=20
        Again, each letter should produce a line of output.  The
        `count' should be much smaller -- the read is non-blocking
        but has a timeout of 2 seconds, so the count should be about
        half the `interval'.
        Type a `q' to finish the test.
VMIN=5, VTIME=0
        A line should be produced for every 5 characters typed.  The
        count should be 1.  This is a blocking read.
        Type a `q' as the first character of a group of 5 to finish
        the test.
VMIN=5, VTIME=20
        Type a character.  Two seconds later a line should be printed.
        Count should be 1.  Type a character, and another within 2 seconds.
        Two seconds after last character (or right after the 5th character)
        a line should be printed.
        Type a `q' as the first character of a group to finish the test.


9. Exit

Gets you out of the test.


Clear???


---
Eric Norum
eric@norum.ca

Charles-Antoine Gauthier
Software Engineering Group
Institute for Information Technology
National Research Council of Canada
charles.gauthier@nrc.ca
