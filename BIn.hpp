/* Script2 (TM) @version 0.x
@link    https://github.com/KabukiStarship/Script2.git
@file    /BIn.hpp
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright (C) 2015-21 Kabuki Starship (TM) <kabukistarship.com>.
This Source Code Form is subject to the terms of the Mozilla Public License,
v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain
one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM >= SCRIPT2_CRABS
#ifndef SCRIPT2_BIN_DECIMPL
#include "BIn.h"
#define SCRIPT2_BIN_DECIMPL
namespace _ {

/* Print the BIn to the printer. */
template <typename Printer>
Printer& TBInPrint(Printer& printer, BIn* bin) {
  ISC size = bin->size;
  return printer << Linef('_', 80) << " size:" << bin->size
                 << " start:" << bin->origin << " stop:" << bin->stop
                 << " read:" << bin->read;
  printer << Hexf(BInBegin(bin), size + sizeof(BIn));
}

inline CHA* BInBegin(BIn* bin) { return TPtr<CHA>(bin) + sizeof(BIn); }

inline const Op* BOutRead(BOut* bout, const ISC* params, void** args) {
  return BInRead(reinterpret_cast<BIn*>(bout), params, args);
}

}  // namespace _

#endif
#endif
