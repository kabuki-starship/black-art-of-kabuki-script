/* Script^2 @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /floor.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= _0_0_0__14
#ifndef SCRIPT2_FLOOR
#define SCRIPT2_FLOOR
#include "config.h"

namespace _ {

/* Cache-aligned group of global system variables.
The Floor is the Floor in the Chinese Room. In Kabuki Crabs, the floor is
designed to store variables without having to include the classes the variable
controls. For this reason only strings and POD types are in the Floor.
*/
struct SDK Floor {
  SI2 epoch;
};

/* Returns the the global Floor. */
SDK inline Floor* Global();

}  //< namespace _

#endif  // #if SCRIPT2_FLOOR
#endif  // #if SEAM >= _0_0_0__14
