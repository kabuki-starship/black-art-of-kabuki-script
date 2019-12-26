/* Script2 (TM) @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /_seams/13.table.h
@author  Cale McCollough <https://cale-mccollough.github.io>
@license Copyright (C) 2015-9 Kabuki Starship (TM) <kabukistarship.com>.
This Source Code Form is subject to the terms of the Mozilla Public License,
v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain
one at <https://mozilla.org/MPL/2.0/>. */

#if SEAM >= SCRIPT2_TABLE
#include "../table.hpp"
using namespace _;
#if SEAM == SCRIPT2_TABLE
#include "../_debug.inl"
#else
#include "../_release.inl"
#endif

namespace script2 {

template <typename ISZ, typename IUZ, typename CHT>
void TestTable() {
  D_COUT(Linef("\n\n\n\n\n\n+---\nTesting ATable<IS")
         << CH('@' + sizeof(ISZ)) << ",IU" << CHT('0' + sizeof(IUZ)) << ",CH"
         << CH('@' + sizeof(CHT)) << ">\n"
         << Linef("+---\n\n"));

  ATable<ISZ, IUZ, CHT> table;

  static const CHT a[] = {'A', '\0'}, b[] = {'B', '\0'}, c[] = {'C', '\0'},
                   d[] = {'D', '\0'}, abc[] = {'a', 'b', 'c', '\0'},
                   bac[] = {'b', 'a', 'c', '\0'}, cba[] = {'c', 'b', 'a', '\0'},
                   cab[] = {'c', 'a', 'b', '\0'},
                   test[] = {'t', 'e', 's', 't', '\0'};

  A_AVOW((ISZ)0, table.Add(a));

  A_AVOW((ISZ)0, table.Find(a));

  A_AVOW((ISZ)1, table.Add(b));
  A_AVOW((ISZ)0, table.Find(a));
  A_AVOW((ISZ)1, table.Find(b));

  A_AVOW((ISZ)2, table.Add(c));
  A_AVOW((ISZ)0, table.Find(a));
  A_AVOW((ISZ)1, table.Find(b));
  A_AVOW((ISZ)2, table.Find(c));

  A_AVOW((ISZ)3, table.Add(d));
  A_AVOW((ISZ)0, table.Find(a));
  A_AVOW((ISZ)1, table.Find(b));
  A_AVOW((ISZ)2, table.Find(c));
  A_AVOW((ISZ)3, table.Find(d));

  A_AVOW(CInvalidIndex<ISZ>(), table.Add(a));
  A_AVOW(CInvalidIndex<ISZ>(), table.Add(b));
  A_AVOW(CInvalidIndex<ISZ>(), table.Add(c));
  A_AVOW(CInvalidIndex<ISZ>(), table.Add(d));

  A_AVOW((ISZ)4, table.Add(abc));
  A_AVOW((ISZ)4, table.Find(abc));

  A_AVOW((ISZ)5, table.Add(bac));
  A_AVOW((ISZ)4, table.Find(abc));
  A_AVOW((ISZ)5, table.Find(bac));

  A_AVOW((ISZ)6, table.Add(cba));
  A_AVOW((ISZ)4, table.Find(abc));
  A_AVOW((ISZ)5, table.Find(bac));
  A_AVOW((ISZ)6, table.Find(cba));

  A_AVOW((ISZ)7, table.Add(cab));
  A_AVOW((ISZ)4, table.Find(abc));
  A_AVOW((ISZ)5, table.Find(bac));
  A_AVOW((ISZ)6, table.Find(cba));
  A_AVOW((ISZ)7, table.Find(cab));

  D_COUT_OBJ(table);
  A_AVOW(CInvalidIndex<ISZ>(), table.Find(test));

  // D_PAUSE("");
}
}  // namespace script2
#endif

namespace script2 {
static const CHA* Table(const CHA* args) {
#if SEAM >= SCRIPT2_TABLE
  A_TEST_BEGIN;

  TestTable<ISB, IUB, CHA>();
  TestTable<ISC, IUC, CHA>();
#if USING_UTF16 == YES_0
  TestTable<ISB, IUB, CHB>();
  TestTable<ISC, IUC, CHB>();
#endif
#if USING_UTF32 == YES_0
  TestTable<ISB, IUB, CHC>();
  TestTable<ISC, IUC, CHC>();
#endif
#endif
  return nullptr;
}
}  // namespace script2
