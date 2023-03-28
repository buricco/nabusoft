# The NabuSoft Project

## What is this?

This aims to be a collection of wrappers around SG-1000 (and in the future
perhaps also MSX, ColecoVision) video games so that they will run on the Nabu
Personal Computer.

## Why do this?

The Nabu was well ahead of its time conceptually, but like many obscure
computer platforms has a dearth of software.  The similarity of the far more
popular MSX, ColecoVision and SG-1000 systems means that the effort needed to
bring games to the Nabu is far less than that needed to bring games to other
systems which are less similar to ones with larger game libraries.

## Is this code open-source?

No.  See below.

Exception: the "Injector" tool is licensed under the NetBSD license.

## I see source code; why do you say it's not open-source?

Open-source code does not simply mean it's *open* for you to look at: it must
also be *open* for you to use, modify and share (in any combination).  While
the wrappers can most likely be published (any "black code" falling under some
fair use exemption), the binaries in their complete states cannot.

This is the reason I have included an "Injector" utility; it allows the
publication of the wrapper code *without* requiring me to also publish ROM
dumps of the games in question.  You supply the ROM, and the program will fit
it into the pre-compiled binary.

## What sort of weird Z80 assembler dialect is this?

[z80asm](https://savannah.nongnu.org/projects/z80asm)

## How do I use these wrappers?

Example command to compile Injector: `cc -o injector injector.c`

To build each .a80 file: ```z80asm -o filename.bin filename.a80
./injector filename.bin filename.rom filename.com```

Nabu's native operating system loads applications other than the menu system
at 0x0100, the same as CP/M, so the resulting binaries are meant to be used in
both modes.  Yes, I know that that is not the load or execute address of the
menu system (I wrote a cable IPL for the Nabu after all); these binaries are
not meant to be used that way.  My Testing is generally done with a fake
loader or with CP/M 3.1.

## it no wrk.

The reason I have uploaded this code is not necessarily because these games
work, but often because they have serious bugs and, as the saying goes, "many
eyes make all bugs shallow."  This is not where I release finished game ports,
but where I release unfinished ports in need of assistance, and finished ports
will be archived for the historical record.
