/* Script^2 @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /script2/06_array.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <cale@astartup.net>;
All right reserved (R). This Source Code Form is subject to the terms of the
Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with
this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once
#include <pch.h>

#include "t_stack.h"

#if SEAM == SCRIPT2_SEAM_STACK
#include "module_debug.inl"
#else
#include "module_release.inl"
#endif

using namespace _;

namespace script2 {

template <typename SIZ>
void TestStack(CH1* seam_log, CH1* seam_end, const CH1* args) {
  PRINT("\n\nTesting AStack<SI");
  PRINT(sizeof(SIZ));
  PRINT(">...\n\nPrinting empty stack...\n");

  AStack<SI8, SIZ, TSocket<8, SI8, TStack<SIZ>>> stack;
#if DEBUG_SEAM
  stack.COut();
#endif

  PRINT("\n\nPushing items on to the Stack...\n");

  SI8 test_count = 32, count_init = 0;
  for (SI8 i = count_init; i <= test_count; ++i) stack.Push(i);

  PRINT_OBJ(stack);

  PRINT("\nPopping items off the Stack...\n");

  for (SI8 i = test_count; i >= count_init; --i) AVOW_INDEX(i, stack.Pop(), i);
#if DEBUG_SEAM
  stack.COut();
#endif
  ASSERT(stack.Count() == 0);
  PRINT_OBJ(stack);
}

static const CH1* _07_Stack(CH1* seam_log, CH1* seam_end, const CH1* args) {
#if SEAM >= SCRIPT2_SEAM_STACK
  TEST_BEGIN;

  const CH1* result = 0;
  TestStack<SI2>(seam_log, seam_end, args);
  TestStack<SI4>(seam_log, seam_end, args);
  TestStack<SI8>(seam_log, seam_end, args);
#endif
  return nullptr;
}
}  // namespace script2