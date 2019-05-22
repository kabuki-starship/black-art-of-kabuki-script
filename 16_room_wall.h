/* Script^2 @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /script2/16_room.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <cale@astartup.net>;
All right reserved (R). This Source Code Form is subject to the terms of the 
Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with 
this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once
#include <pch.h>

#if SEAM == SCRIPT2_15
#include "module_debug.inl"
#else
#include "module_release.inl"
#endif

#include "c_room.h"

using namespace _;

namespace script2 {
static const CH1* _16_Room_Wall(CH1* seam_log, CH1* seam_end,
                                const CH1* args) {
#if SEAM == SCRIPT2_16
  TEST_BEGIN;
  Room room_a, room_b;
#endif
  return nullptr;
}
} //< namespace script2
