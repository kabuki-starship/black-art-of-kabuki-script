/* Script^2 @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /script2/c_asciidata.h
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

#include "c_ascii.h"

namespace _ {
/* List of the 32 ASCII Data Types.
Types are organized with types that are allowed in text B-Sequences first,
followed by objects which get created in text using Script Operations. */
typedef enum AsciiTypes {
  kNIL = 0,  //< 0.  kNIL/nil/void type.
  kSI1,      //< 1.  8-bit signed integer.
  kUI1,      //< 2.  8-bit unsigned integer.
  kSI2,      //< 3.  16-bit signed integer.
  kUI2,      //< 4.  16-bit unsigned integer.
  kHLF,      //< 5.  16-bit floating-point number.
  kBOL,      //< 6.  32-bit non-zero true boolean as signed integer.
  kint,      //< 7.  32-bit signed integer.
  kUI4,      //< 8.  32-bit unsigned integer.
  kFLT,      //< 9.  32-bit floating-point number.
  kTM4,      //< 10. 32-bit second since epoch timestamp.
  kTME,      //< 11. 64-bit sub-second timestamp with kTM4 and an kUI4 tick.
  kTM8,      //< 12. 64-bit second since epoch timestamp.
  kSI8,      //< 13. 64-bit signed integer.
  kUI8,      //< 14. 64-bit unsigned integer.
  kDBL,      //< 15. 64-bit floating-point number.
  kSIH,      //< 16. 128-bit (Hexadeca-UI1) signed integer.
  kUIH,      //< 17. 128-bit (Hexadeca-UI1) unsigned integer.
  kDEC,      //< 18. 128-bit (Hexadeca-UI1) floating-point number.
  kUIX,      //< 19. 2^(6+X)-bit unsigned integer, where 0 <= X <= 7.
  kOBJ,      //< 20. N-UI1 object.
  kADR,      //< 21. UTF-8 Operand stack address.
  kSTR,      //< 22. A UTF-8 .
  kTKN,      //< 23. A UTF-8  token without whitespace.
  kBSQ,      //< 24. B-Sequence.
  kLOM,      //< 25. A loom of UTF-8, UTF-16, or UTF-32 strings.
  kTBL,      //< 26. A hash table.
  kEXP,      //< 27. Script^2 Expression.
  kLST,      //< 28. Set or multiset of Type-Value tuples.
  kMAP,      //< 29. One-to-one map of Integer-{Type-Value} records.
  kBOK,      //< 30. Many-to-one multimap of Key-{Type-Value} records.
  kDIC,      //< 31. One-to-one map of Key-{Type-Value} records.
} AsciiType;
}  // namespace _

#if SEAM >= SCRIPT2_13
#ifndef SCRIPT2_CASCIIDATA
#define SCRIPT2_CASCIIDATA

#include "c_socket.h"
#include "t_strand.h"

namespace _ {
/* A type-value tuple. */
struct SDK TypeValue {
  SI4 type;           //< ASCII Type.
  const void* value;  //< Pointer to the value data.

  /* Stores the type and value. */
  TypeValue(SI4 type, const void* value = nullptr);
};

/* Checks if the given type is valid.
    @return False if the given type is an 8-bit kLST, kMAP, kBOK, or kDIC. */
inline BOL TypeIsValid(SI4 type);

/* Aligns the given pointer to the correct word boundary for the type. */
SDK void* TypeAlign(SI4 type, void* value);

enum {
  kTypeCount = 32,  //< The starting index of invalid types.
};

/* Returns a pointer to an array of pointers to the type names.*/
SDK const CH1** TypeStrands();

/* Returns the name of the given type. */
SDK inline const CH1* TypeStrand(SI4 type);

/* Returns the name of the given type. */
SDK inline const CH1* TypeStrand(SI4 type);

/* Masks off the lower 5-LSb to get the type. */
SDK inline UI1 TypeMask(UI1 value);

/* Returns true if the given type is an Array type. */
SDK inline BOL TypeIsArray(SI4 type);

/* Returns true if the given type is a kBOK type. */
SDK inline BOL TypeIsSet(SI4 type);

/* Returns the size or max size of the given type. */
SDK SI4 TypeFixedSize(SI4 type);

/* Gets the next address that a data type may be stored at. */
SDK void* TypeAlign(SI4 type, void* value);

/* Writes the given value to the socket. */
SDK CH1* Write(CH1* begin, CH1* stop, SI4 type, const void* source);

/* Returns true if the given type is an ASCII CObject. */
SDK inline BOL TypeIsObj(SI4 type);

/* Returns true if the given type is a  type. */
SDK inline BOL TypeIsStrand(SI4 type);

/* Checks if the given type is UTF-16.
    @param  type The type to check.
    @return True if the given type is UTF-16. */
SDK inline BOL TypeIsUTF16(SI4 type);

SDK inline SI4 TypeSizeWidthCode(SI4 type);
}  // namespace _

#if USING_UTF8 == YES
namespace _ {
/* Prints th given type or type-value.
@return Returns a pointer to the next CH1 after the stop of the read number or
nil upon failure.
@param utf The utf to utf to.
@param type    The type to utf.
@param value   The value to utf or nil. */
SDK CH1* Print(CH1* begin, CH1* stop, SI4 type, const void* value);
}  // namespace _

/* Writes the given value to the utf justified right.
@return The utf.
@param  utf The utf.
@param  item The item to utf. */
SDK ::_::UTF1& operator<<(::_::UTF1& utf, const ::_::TypeValue& type_value);
#endif

#if USING_UTF16 == YES
namespace _ {
/* Prints th given type or type-value.
@return Returns a pointer to the next CH1 after the stop of the read number or
nil upon failure.
@param utf The utf to utf to.
@param type    The type to utf.
@param value   The value to utf or nil. */
SDK CH2* Print(CH2* begin, CH2* stop, SI4 type,
                    const void* value);
}  // namespace _
/* Writes the given value to the utf justified right.
@return The utf.
@param  utf The utf.
@param  item The item to utf. */
SDK ::_::UTF2& operator<<(::_::UTF2& utf, const ::_::TypeValue& type_value);
#endif
#if USING_UTF32 == YES

namespace _ {
/* Prints th given type or type-value.
@return Returns a pointer to the next CH1 after the stop
of the read number or nil upon failure.
@param printer The printer to utf to.
@param type    The type to utf.
@param value   The value to utf or nil. */
SDK CH2* Print(CH2* begin, CH2* stop, SI4 type,
                    const void* value);
}  // namespace _
/* Writes the given value to the utf justified right.
@return The utf.
@param  utf The utf.
@param  item The item to utf. */
SDK ::_::UTF4& operator<<(::_::UTF4& utf, const ::_::TypeValue& type_value);
#endif

#endif  //< SCRIPT2_CASCIIDATA
#endif  //< #if SEAM >= SCRIPT2_13