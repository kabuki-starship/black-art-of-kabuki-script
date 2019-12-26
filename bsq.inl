/* Script2 (TM) @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /bsq.inl
@author  Cale McCollough <https://cale-mccollough.github.io>
@license Copyright (C) 2015-9 Kabuki Starship (TM) <kabukistarship.com>.
This Source Code Form is subject to the terms of the Mozilla Public License,
v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain
one at <https://mozilla.org/MPL/2.0/>. */

#include <_config.h>
#if SEAM >= SCRIPT2_DIC
#include "bsq.h"
#include "test.h"

namespace _ {

ISN BsqParamNumber(const ISN* params, ISN param_number) {
  if (!params) return 0;
  ISN num_params = *params++;
  if (param_number > num_params) return cNIL;
  ISN i;
  for (i = 0; i < param_number; ++i) {
    ISN value = params[i];
    if (value == kSTR)
      ++param_number;
    else if (value > 31) {  // It's an array!
      value = value >> 5;
      if (value < 4) {  // It's a single dimension!
        param_number += 2;
        break;
      } else if (value > 7) {  // Gratuitous explanation points!
        // D_COUT ("\nError");
        return cNIL;
      } else {
        param_number += params[i] + 1;
      }
    }
  }
  return params[i];
}

template <typename Printer>
Printer& PrintBsq(Printer& o, const ISN* params) {
  ISN num_params = *params++, i, type, value = 0;

  o << "Param<";
  if (num_params > cParamsMax) {
    o << "\nInvalid num_params: " << num_params;
    return o;
  }
  o << num_params << ": ";
  for (i = 1; i < num_params; ++i) {
    value = *params++;
    type = value & 0x1f;  //< Mask off type.
    value = value >> 5;   //< Shift over array type.
    o << STRType((ISN)value) << ", ";
    if (type >= kSTR) {
      if (value) {
        o << "\nError: arrays may only be created from POD "
             "types.";
        return o;
      }
      // Print out the max length of the .
      ++i;
      value = *params++;
      o << value;
    } else if (value > 31) {
      if (value > 127) {  //< It's a multi-dimensional array.
        o << "Multi-dimensional Array:" << value << ", ";
      }
      // Then it's an array.
      ++i;
      switch (value) {  //< Print out the Array type.
        case 0: {
          break;
        }
        case 1: {
          value = *params++;
          o << "c_UIA:" << value << ", ";
          break;
        }
        case 2: {
          value = *params++;
          o << "c_UI2:" << value << ", ";
          break;
        }
        case 3: {
          value = *params++;
          o << "c_UI4:" << value << ", ";
          break;
        }
        case 4: {
          value = *params++;
          o << "c_UI8:" << value << ", ";
          break;
        }
        case 5: {
          value = *params++;
          if (value == 0) {
            o << "c_UIA:[0]";
            break;
          }
          o << "c_UIA:[" << value << ": ";
          for (ISN i = value; i != 0; --i) {
            value = *params++;
            o << value << ", ";
          }
          value = *params++;
          o << value << "]";
          break;
        }
        case 6: {
          value = *params++;
          if (value == 0) {
            o << "c_UI2:[0]";
            break;
          }
          o << "c_UI2:[" << value << ": ";
          for (ISN i = value; i != 0; --i) {
            value = *params++;
            o << value << ", ";
          }
          value = *params++;
          o << value << "]";
          break;
        }
        case 7: {
          value = *params++;
          if (value == 0) {
            o << "c_UI4:[0]";
            break;
          }
          o << "c_UI4:[" << value << ": ";
          for (ISN i = value; i != 0; --i) {
            value = *params++;
            o << value << ", ";
          }
          value = *params++;
          o << value << "]";
          break;
        }
      }
    }
  }
  // Do the last set without a comma.
  value = *params++;
  o << STRType(value) << ", ";
  if (value == kSTR) {
    ++i;
    value = *params++;
    o << value;
  } else if (value > 31) {
    // Then it's an array.
    type = value & 0x1f;  //< Mask off type.
    value = value >> 5;   //< Shift over array type.
    ++i;
    switch (value) {
      case 0: {
        break;
      }
      case 1: {
        value = *params++;
        o << "c_UIA:" << value << ", ";
        break;
      }
      case 2: {
        value = *params++;
        o << "c_UI2:" << value << ", ";
        break;
      }
      case 3: {
        value = *params++;
        o << "c_UI4:" << value << ", ";
        break;
      }
      case 4: {
        value = *params++;
        o << "UI5:" << value << ", ";
        break;
      }
      case 5: {
        value = *params++;
        if (value == 0) {
          o << "c_UIA:[0]";
          break;
        }
        o << "c_UIA:[" << value << ": ";
        for (ISN i = value; i != 0; --i) {
          value = *params++;
          o << value << ", ";
        }
        value = *params++;
        o << value << "]";
        break;
      }
      case 6: {
        value = *params++;
        if (value == 0) {
          o << "c_UI2:[0]";
          break;
        }
        o << "c_UI2:[" << value << ": ";
        for (ISN i = value; i != 0; --i) {
          value = *params++;
          o << value << ", ";
        }
        value = *params++;
        o << value << "]";
        break;
      }
      case 7: {
        value = *params++;
        if (value == 0) {
          o << "c_UI4:[0]";
          break;
        }
        o << "c_UI4:[" << value << ": ";
        for (ISN i = value; i != 0; --i) {
          value = *params++;
          o << value << ", ";
        }
        value = *params++;
        o << value << "]";
        break;
      }
    }
  }
  o << '>';
  return o;
}

}  // namespace _

#endif