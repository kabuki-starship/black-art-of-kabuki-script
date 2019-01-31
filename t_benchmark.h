/* Script^2 @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /script2/t_benchmark.h
@author  Cale McCollough <cale@astartup.net>
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

#if SEAM >= SCRIPT2_13
#ifndef INCLUDED_SCRIPTTBENCHMARK
#define INCLUDED_SCRIPTTBENCHMARK

#include "benchmark.h"

namespace _ {

template <typename UI>
const CH1* BenchmarkCaseLoop(CH1* cursor, CH1* stop, const CH1* args) {
  PrintHeading((sizeof(UI) == 8) ? "Testing 64-bit use case"
                                 : "Testing 32-bit use case");
}

}  // namespace _

#endif  //< INCLUDED_SCRIPTTBENCHMARK
#endif  //< #if SEAM >= SCRIPT2_13