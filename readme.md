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
it into the pre-compiled binary.  (This was required for earlier revisions of
Zoom 909; it is no longer needed since the wrapper sources the ROM from a
separate file at compile time.)

## What sort of weird Z80 assembler dialect is this?

[z80asm](https://savannah.nongnu.org/projects/z80asm)

## How do I use these wrappers?

You'll need the ROM file in the current directory, if the wrapper references
it.  At any rate you'll need to start with the assembler:
`z80asm -o filename.bin filename.a80`

### SG-1000 (with `DS` gap)

Example command to compile Injector: `cc -o injector injector.c`

Then Injector is used as follows:
`./injector filename.bin filename.rom filename.com`

### MSX (with ROM space at the end)

Build the .a80 as above.

On Windows, `copy /b filename.bin+filename.rom filename.com`

On anything remotely resembling Unix,
`cat filename.bin filename.rom > filename.com`

### File that uses `INCBIN` to automatically use the ROM

Just make sure the file is in the current directory with the required name.

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

As for the "it no wrk in MAME" problem, I have no idea which emulator is doing
The Right Thing®™©, and I have no way to test any of this on metal.  Marduk is
following my best understanding of the available documentation, but MAME tries
to be accurate while Marduk takes a lot of shortcuts.

## Games included here?

`frogger`: Frogger
`pooyan`: Pooyan
`warpwarp`: Galaxian, Tank Battalion, Warp & Warp (same stub for all 3)
`zaxxon`: Zaxxon
`zoom909`: Zoom 909 (*a.k.a.* Buck Rogers: Planet of Zoom)

## Games that *work?*

Zoom 909 works in Marduk and apparently works on metal, apart from sound
issues related to the on-the-fly SN76489-to-AY8910 translation.

## Controls?

As with the official Nabu games, these games expect a joystick, and will
support the Pause key.

Also, Zoom 909 supports the Ctrl-R key to restart.

## Other stuff included here?

`deadwait`: Fake ROM that copies itself to RAM and then waits for an external
source to DMA a CP/M command into place.  Quicker than loading a complete copy
of [OpenNabu IPL](https://github.com/buricco/opennabuipl).  (More recently
obviated because I added a *special switch* `-x` to Marduk.)

## Third-party games?

Why not?
