/* Script2 (TM) @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /wall.h
@author  Cale McCollough <https://cale-mccollough.github.io>
@license Copyright (C) 2015-9 Kabuki Starship (TM) <kabukistarship.com>.
This Source Code Form is subject to the terms of the Mozilla Public License,
v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain
one at <https://mozilla.org/MPL/2.0/>. */

#pragma once
#include <_config.h>
#if SEAM >= SCRIPT2_DIC
#ifndef SCRIPT2_WALL_HEADER
#define SCRIPT2_WALL_HEADER

#if SEAM == SCRIPT2_ROOM
#include "_release.inl"
#else
#include "_release.inl"
#endif

#include "door.h"
#include "op.h"

namespace _ {

/* A group of slots that all go to the same Room.
Only one single wall is required for a Chinese Room, but when more memory is
needed a new Wall may be created and destroyed dynamically.
@code
+--------------+
|  Terminals   |
|      v       |
|vvvvvvvvvvvvvv|
|    Buffer    |
|^^^^^^^^^^^^^^|
|      ^       |
|  TSTack of   |
|    Doors     |
|   Offsets    |
|--------------|
|    Header    |
+--------------+
@endcode */
class Wall : public Operand {
 public:
  enum {
    cMinSizeBytes = 512,  //< Min functional Wall size.
  };

  virtual ~Wall() {
    if (is_dynamic_) {
      CHA* socket = reinterpret_cast<CHA*>(doors_);
      delete[] socket;
    }
  }

  Wall(TMap<Door*>* doors);

  /* Constructs a wall from the given socket. */
  Wall(ISW size_bytes = cMinSizeBytes) : is_dynamic_(true) {
    size_bytes = size_bytes < cMinSizeBytes ? (ISC)cMinSizeBytes : size_bytes;
    size_bytes = TAlignUpUnsigned<ISD, ISW>(size_bytes);
    ISW size_words = (size_bytes >> sizeof(void*)) + 3;
    UIW *socket = new UIW[size_words],
        *aligned_buffer = AlignUpPointer8<UIW>(socket);
    //< Shift 3 to divide by 8. The extra 3 elements are for aligning memory
    //< on 16 and 32-bit systems.
    size_bytes -= sizeof(UIW) * (aligned_buffer - socket);
    origin = socket;
    doors_ = reinterpret_cast<TMatrix<Door*>*>(aligned_buffer);
    TStackInit(socket, size_bytes >> sizeof(UIW));
  }

  /* Constructs a wall from the given socket. */
  Wall(UIW* socket, ISW size_bytes) {
    // CHA* ptr     = reinterpret_cast<CHA*> (socket);//,
    //    * new_ptr = ptr + AlignOffset<IUD> (ptr),
    //    * end_ptr = ptr + size_bytes;
    enum {
      cBitsShift = sizeof(UIW) == 2 ? 1 : sizeof(UIW) == 2 ? 2 : 3,
    };
    // ISC size_words = (size_bytes >> kBitsShift) + 3;
    //< Computer engineering voodoo for aligning to 64-bit boundary.

    UIW* aligned_buffer = AlignUpPointer8<UIW>(socket);
    //< Shift 3 to divide by 8. The extra 3 elements are for aligning memory
    //< on 16 and 32-bit systems.
    size_bytes -= sizeof(UIW) * (aligned_buffer - socket);
    origin = socket;
    doors_ = reinterpret_cast<TMatrix<Door*>*>(aligned_buffer);
    TStackInit(socket, size_bytes >> sizeof(UIW));
  }

  /* Gets the size of the wall in bytes. */
  ISW GetSizeBytes() {
    return size_bytes_;

    /* Gets a pointer to the array of pointers to Door(). */
    TMatrix<Door*>* Doors();

    /* Gets the Door from the Door at the given index. */
    Door* GetDoor(ISN index) { return 0; }

    /* Adds a Door to the slot.
    @return Returns nil if the Door is full and a pointer to the Door in the
            socket upon success. */
    ISC OpenDoor(Door * door) { return 0; }

    /* Deletes the Door from the Door at the given index. */
    BOL CloseDoor(ISN index) { return false; }

    /* Prints the given Door to the stream. */
    template <typename Printer>
    Printer& PrintTo(Printer & o) {
      return o;
    }

   private:
    BOL is_dynamic_;        //< Flag for if using dynamic memory.
    ISW size_bytes_;        //< Size of the Wall in bytes.
    UIW* origin;            //< The Wall's socket.
    TSTack<Door*>* doors_;  //< The doors in the room.
  };

}  // namespace _
#endif
#endif