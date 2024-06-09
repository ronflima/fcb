/* -*-c-*-
   The MIT License (MIT)

   Copyright (c) 2024 - Ronaldo Faria Lima

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

   Created: 2024-06-08 by Ronaldo Faria Lima

   This file purpose: FCB structure declarations
*/

#ifndef _FCB_H_
#define _FCB_H_

#include <stdio.h>
#include "fcb_common.h"

FCB_BEGINDECLS

#define FCB_SIGNATURE 0x00FCB0

/**
 * Defines the overall file FCB
 */
typedef struct fcb_t
{
  unsigned int signature;       /* File signature */
  size_t records_qty;           /* Quantity of records in this file */
} fcb_t;

/**
 * Describes a record within the file
 */
typedef struct record_t
{
  size_t record_size;           /* Record size in bytes */
  struct record_t *next;        /* Next record or NULL if this is the last one */
  char record[0];               /* Record entry point */
} record_t;

/**
 * Describes an FCB file
 */
typedef struct fcb_file_t
{
  FILE *fp;                   /* File descriptor */
  fcb_t header;               /* File header */
  record_t *records;          /* List of records read from the file */
  record_t *last_record;      /* Last record at the list */
} fcb_file_t;


/**
 * Opens a FCB file for reading/writing
 *
 * Parameters:
 * - fname: File name
 * - mode: Mode. (see fopen for valid modes)
 *
 * Returns:
 * - fcb file descriptor, if successful.
 * - NULL if something went wrong
 */
fcb_t *(fcb_open)(const char *fname, const char *mode);

/**
 * Closes a previously opened FCB file
 *
 * Parameters:
 * - fcb: An FCB file descriptor
 *
 * Returns:
 * - See fclose return values
 */
int (fcb_close)(const *fcb_t fcb);

/**
 * Reads all available records in an FCB file into memory.
 *
 * Parameters:
 * - fcb: A valid FCB file descriptor.
 *
 * Returns:
 * - The number of read records on success.
 * - EOF if we could not read from the file
 * - ENOMEM if we failed to get more memory
 */
int (fcb_read)(const fcb_t *fcb);


FCB_ENDDECLS

#endif
