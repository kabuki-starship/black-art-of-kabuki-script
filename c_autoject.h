/* SCRIPT Script @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /script2/c_array.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <cale@astartup.net>;
All right reserved (R). This Source Code Form is subject to the terms of the
Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with
this file, You can obtain one at <https://mozilla.org/MPL/2.0/>. */

#pragma once
#include <pch.h>

#if SEAM >= SCRIPT2_STACK
#ifndef SCRIPT2_AUTOJECT_HEADER
#define SCRIPT2_AUTOJECT_HEADER

namespace _ {
struct Autoject;
}

/* RamFactory manages memory for ASCII Objects.
@return A word-aligned buffer, rounding up if unaligned.
@param obj  A block of word-aligned heap memory.
@param size The size of the buffer to create in bytes. */
typedef UIW* (*RamFactory)(UIW* obj, SIW size);

namespace _ {

/* ASCII OBJ and RamFactory. */
struct Autoject {
  RamFactory ram_factory;  //< Autoject Factory function pointer.
  UIW* begin;              //< Pointer to the Autoject.
};

enum AsciiFactoryFunction {
  kFactoryDelete = 0,  //< Factory function deletes an OBJ.
  kFactoryNew = 1,     //< Factory function checks if the size can double.
  kFactoryGrow = 2,    //< Factory function double OBJ size.
  kFactoryClone = 3,   //< Factory function clones the OBJ.
  kFactoryName = 4,    //< Factory function gets the info string.
  kFactoryFunctionCount = 5,  //< Factory function count.
};

enum AsciiFactoryError {
  kFactorySuccess = 0,      //< Factory operation completed successfully error.
  kFactoryNil = 1,          //< Factory missing error.
  kFactoryNilOBJ = 2,       //< Factory found nil obj.begin pointer error.
  kFactoryNilArg = 3,       //< Factory arg nil error.
  kFactoryCantGrow = 4,     //< Factory can't grow.
  kFactorySizeInvalid = 5,  //< Factory size invalid.
  kFactoryErrorCount = 6,   //< Factory function count.
};

/* Creates or destroys a block of heap memory. */
LIB_MEMBER UIW* RamFactoryHeap(UIW* obj, SIW size);

/* Creates a block of heap memory. */
LIB_MEMBER UIW* RamFactoryStack(UIW* ptr, SIW size);

LIB_INLINE UIW* AutojectBeginSet(Autoject& obj, void* buffer);

/* Deletes the given obj using the obj.factory. */
LIB_MEMBER void Delete(Autoject& obj);

/* Overwrites the memory with fill_char; functionally identical to memset. */
LIB_MEMBER CH1* ArrayFill(void* begin, void* end, CH1 fill_char = 0);

/* Overwrites the memory with fill_char; functionally identical to memset. */
LIB_MEMBER CH1* ArrayFill(void* begin, SIW size, CH1 fill_char = 0);

/* Overwrites the memory with fill_char; functionally identical to memset. */
LIB_MEMBER CH1* ArrayWipe(void* begin, void* end);

/* Overwrites the memory with fill_char; functionally identical to memset. */
LIB_MEMBER CH1* ArrayWipe(void* begin, SIW size);

/* Copies the source to the target functionally identical to memcpy.
@param  begin     The start of the write socket.
@param  size      The stop of the write socket.
@param  begin     The begin of the read socket.
@param  read_size Number of bytes to copy.
@return Pointer to the last UI1 written or nil upon failure. */
LIB_MEMBER CH1* ArrayCopy(void* begin, SIW size, const void* read,
                          SIW read_size);

/* Copies the source to the target functionally identical to memcpy.
@param  begin The start of the write socket.
@param  stop  The stop of the write socket.
@param  begin The begin of the read socket.
@param  size  Number of bytes to copy.
@return Pointer to the last UI1 written or nil upon failure. */
LIB_MEMBER CH1* ArrayCopy(void* start, void* stop, const void* begin,
                          SIW read_size);

/* Copies the source to the target functionally identical to memcpy.
@param  start The start of the write socket.
@param  stop  The stop of the write socket.
@param  begin The begin of the read socket.
@param  end   The end of the read socket.
@return Pointer to the last UI1 written or nil upon failure. */
LIB_INLINE CH1* ArrayCopy(void* start, void* stop, const void* begin,
                          const void* end);

/* Compares the two memory sockets.
@param  start  The start of Socket A.
@param  stop   The stop of Socket A.
@param  begin  The begin of Socket B.
@param  end    The end of Socket B.
@return True if they are the same and false if they are not. */
LIB_MEMBER BOL ArrayCompare(const void* start, const void* stop,
                            const void* begin, const void* end);

/* Compares the two memory sockets.
@param  start The start of Socket A.
@param  stop  The stop of Socket A.
@param  begin The begin of Socket B.
@param  size  The size of Socket B.
@return True if they are the same and false if they are not. */
LIB_MEMBER BOL ArrayCompare(const void* start, void* stop, const void* begin,
                            SIW size);

/* Compares the two memory sockets.
@param  start The start of socket a.
@param  size_a The size of Socket A .
@param  start  The begin of socket b.
@param  size_b The size of Socket B.
@return True if they are the same and false if they are not. */
LIB_MEMBER BOL ArrayCompare(const void* start, SIW size_a, const void* begin,
                            SIW size_b);

/* Shifts the memory up by the given count in bytes.
@return 0 upon failure and count upon success.
@param  begin       The begin UI1.
@param  end         The end UI1.
@param  count_bytes The UI1 count to shift up. */
LIB_MEMBER SIW ArrayShiftUp(void* begin, void* end, SIW count_bytes);

/* Shifts the memory down by the given bytes_count.
@return 0 upon failure and count upon success.
@param  begin       The start UI1.
@param  end         The end UI1.
@param  count_bytes The UI1 count to shift up. */
LIB_MEMBER SIW ArrayShiftDown(void* begin, void* end, SIW bytes_count);

}  // namespace _
#endif
#endif
