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

   Created: 2024-06-09 by Ronaldo Faria Lima

   This file purpose: Reads an FCB file records into memory
*/

#include <stdio.h>
#include <stdlib.h>

#include "fcb.h"

int
fcb_read(const fcb_t *fcb)
{
  int rc = 0x0;
  record_t *prev = NULL;
  register int i;
  size_t record_size;

  for (i = 0x0; i < fcb->records_qty; ++i)
    {
      if (prev)
        {
          prev->next = curr;
          prev = curr;
        }
      rc = fread(&record_size, sizeof(size_t), 1, fcb->fp);
      if (rc != 1)
        {
          return EOF;
        }
      /* Dirty trick: allocates memory for the record + the record size */
      record_t *curr = (record_t *)malloc(sizeof(record_t) + record_size);
      if (curr == NULL)
        {
          return ENOMEM;
        }

      memset(curr, 0x0, sizeof(record_t));
      curr->record_size = record_size;
      rc = fread(curr->record, record_size, 1, fcb->fp);
      if (rc != 1)
        {
          free(curr);
          return EOF;
        }

      /*
       * We want records in the order they were read. So, we need to
       * keep track of the last record inserted, so we can create a
       * sane list easily.
       */
      if (fcb->last_record)
        {
          fcb->last_record->next = curr;
          fcb->last_record = curr;
        }
      else
        {
          /*
           * If this is the first record, it will be, also, the last
           * one read from disk.
           */
          fcb->records = curr;
          fcb->last_record = curr;
        }
    }

  return rc;
}
