/**
 * Furnace Tracker - multi-system chiptune tracker
 * Copyright (C) 2021-2022 tildearrow and contributors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _FMSHARED_BASE_H
#define _FMSHARED_BASE_H

#include "../dispatch.h"
#include <deque>

#define rWrite(a,v) if (!skipRegisterWrites) {pendingWrites[a]=v;}
#define immWrite(a,v) if (!skipRegisterWrites) {writes.push_back(QueuedWrite(a,v)); if (dumpWrites) {addWrite(a,v);} }
#define urgentWrite(a,v) if (!skipRegisterWrites) { \
  if (writes.empty()) { \
    writes.push_back(QueuedWrite(a,v)); \
  } else if (writes.size()>16 || writes.front().addrOrVal) { \
    writes.push_back(QueuedWrite(a,v)); \
  } else { \
    writes.push_front(QueuedWrite(a,v)); \
  } \
  if (dumpWrites) { \
    addWrite(a,v); \
  } \
}

class DivPlatformFMBase: public DivDispatch {
  protected:
    const bool isOutput[8][4]={
      // 1     3     2    4
      {false,false,false,true},
      {false,false,false,true},
      {false,false,false,true},
      {false,false,false,true},
      {false,false,true ,true},
      {false,true ,true ,true},
      {false,true ,true ,true},
      {true ,true ,true ,true},
    };
    const unsigned char dtTable[8]={
      7,6,5,0,1,2,3,4
    };

    const int orderedOps[4]={
      0,2,1,3
    };

    struct QueuedWrite {
      unsigned short addr;
      unsigned char val;
      bool addrOrVal;
      QueuedWrite(unsigned short a, unsigned char v): addr(a), val(v), addrOrVal(false) {}
    };
    std::deque<QueuedWrite> writes;

    unsigned char lastBusy;
    int delay;

    unsigned char regPool[512];
    short oldWrites[512];
    short pendingWrites[512];

    DivPlatformFMBase():
    lastBusy(0),
    delay(0) {}
};

#endif
