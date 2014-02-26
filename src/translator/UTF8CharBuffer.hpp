/*
  UTF8CharBuffer.hpp

  Copyright (C) 2008-2014  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  CharBuffer for antlr that uses a libxml input routines.
  This provides for any necessary encoding conversion to UTF8,
  so using libxml for XML output is direct.

  libxml stores data internally in UTF*
*/

#ifndef INCLUDE_UTF8CHARBUFFER_HPP
#define INCLUDE_UTF8CHARBUFFER_HPP

#include <antlr/CharBuffer.hpp>
#include <cstring>

#include <iconv.h>

class UTF8FileError {};


class UTF8CharBuffer : public antlr::CharBuffer {
public:

    // size of the original character buffer
    static const size_t SRCBUFSIZE = 1024;

    // Create a character buffer
    UTF8CharBuffer(const char* ifilename, const char * encoding);
    UTF8CharBuffer(const char* c_buffer, size_t buffer_size, const char * encoding);
    UTF8CharBuffer(FILE * file, const char * encoding);
    UTF8CharBuffer(int fd, const char * encoding);

    // Get the next character from the stream
    int getChar();

    ~UTF8CharBuffer();

private:

    int growBuffer();
    void processEncoding(const char * encoding);

    FILE * input;
    int pos;
    int size;
    int total_size;
    bool lastcr;
    bool need_close;
    char * raw_buffer;
    unsigned char * input_buffer;
    iconv_t cd;
    char buffer[SRCBUFSIZE];
    char iconv_buffer[4 * SRCBUFSIZE];

};
#endif
