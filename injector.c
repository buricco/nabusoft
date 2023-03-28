/*
 * (C) Copyright 2023 S. V. Nickolas.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.
 *
 * IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Injects a ROM file into a NabuSoft application.
 * 
 * For copyright reasons, the NabuSoft wrappers hosted publicly do not include
 * the original code (apart, perhaps, from short routines that have been
 * modified to run on the Nabu).  A tool is needed to place the ROM into its
 * expected place (since it is neither at the immediate beginning, nor the
 * immediate end, of the file).
 * 
 * Arguments:  injector filename.bin filename.rom filename.com
 *   filename.bin   The output of z80asm (see below).
 *   filename.rom   A dump of the original ROM code.
 *   filename.com   This will be the generated binary for the Nabu.
 * 
 * The wrapper is assumed to be written in the following way:
 *   * File is a CP/M ".COM" binary, which means that it has a load/exec
 *     address of 0x0100 and no header.
 *   * File starts with a JP instruction and a number of 0x00 bytes equal to
 *     the size of the ROM.
 *   * The target of the JP, which may not necessarily immediately follow the
 *     ROM image, must be a DI instruction.
 * If the above is not true, this program will abort.
 * 
 * Note: This program is mostly written in generic C89, but does use the C99
 *       stdint.h to define uint8_t (8-bit unsigned) and uint16_t (16-bit
 *       unsigned) types.  Replace this with manual declarations if your
 *       ancient C compiler complains.
 */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *progname;

void xperror (char *filename)
{
 fprintf (stderr, "%s: %s: %s\n", progname, filename, strerror(errno));
}

int main (int argc, char **argv)
{
 int c, r;
 uint8_t *buf;
 FILE *file;
 size_t t, l1, l2, l3;
 uint16_t la;
 
 /* Program name for diagnostic messages */
 progname=strrchr(argv[0], '/');
 if (progname) progname++; else progname=argv[0];
 
 /* Invalid number of parameters */
 if (argc!=4)
 {
  fprintf (stderr, "%s: usage: %s filename.bin filename.rom filename.com\n",
           progname, progname);
  return 1;
 }
 
 /* Default return code; mark if warning */
 r=0;
 
 /* Read original COM file (i.e., without the ROM) */
 file=fopen(argv[1], "rb");
 if (!file)
 {
  xperror(argv[1]);
  return 1;
 }
 fseek(file, 0, SEEK_END);
 l1=ftell(file);
 fseek(file, 0, SEEK_SET);
 if (l1>0xFEFF)
 {
  fclose(file);
  fprintf (stderr, "%s: %s: file too large\n", progname, argv[1]);
  return 1;
 }
 buf=malloc(l1);
 if (!buf)
 {
  fclose(file);
  fprintf (stderr, "%s: insufficient memory\n", progname);
  return 1;
 }
 if (fread(buf, 1, l1, file)<l1)
 {
  r=1;
  fprintf (stderr, "%s: %s: warning: short read\n", progname, argv[1]);
 }
 fclose(file);
 
 c=(*buf==0xC3);
 
 /* Do we begin with a Z80 JP? */
 if (c)
 {
  /* The target of the jump should be a Z80 DI. */
  la=buf[2];
  la<<=8;
  la|=buf[1];
  if (buf[la-0x0100]!=0xF3) c=0;
 }

 /* Find out how long the gap is. */
 if (c)
 {
  c=0;
  
  for (t=0; t<(l1-3); t++)
  {
   if (buf[3+t]) {l2=t; c=1; break;}
  }
 }
 
 /* Not our file. */
 if (!c)
 {
  fprintf (stderr, "%s: %s: invalid format\n", progname, argv[1]);
  free(buf);
  return 1;
 }
 
 /* Read the ROM over the gap. */
 file=fopen(argv[2], "rb");
 if (!file)
 {
  free(buf);
  xperror(argv[2]);
  return 1;
 }
 fseek(file, 0, SEEK_END);
 l3=ftell(file);
 fseek(file, 0, SEEK_SET);
 if (l2!=l3) /* File won't slide nicely into the gap. */
 {
  fclose(file);
  free(buf);
  fprintf (stderr, "%s: %s: was expecting %u bytes, found %u\n", 
           progname, argv[2], l2, l3);
  return 1;
 }
 if (fread(&(buf[3]), 1, l2, file)<l2)
 {
  r=1;
  fprintf (stderr, "%s: %s: warning: short read\n", progname, argv[2]);
 }
 fclose(file);
 
 file=fopen(argv[3], "wb");
 if (!file)
 {
  free(buf);
  xperror(argv[3]);
  return 1;
 }
 if (fwrite(buf, 1, l1, file)<l1)
 {
  r=1;
  fprintf (stderr, "%s: %s: warning: short write\n", progname, argv[3]);
 }
 fclose(file);
 free(buf);
 return r;
}
