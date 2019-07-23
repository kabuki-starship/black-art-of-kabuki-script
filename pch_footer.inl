/* SCRIPT Script @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    \pch_footer.inl.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <cale@astartup.net>;
All right reserved (R). This Source Code Form is subject to the terms of the
Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with
this file, You can obtain one at <https://mozilla.org/MPL/2.0/>. */

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// The below macros can be override in the pch.h file.

#if SCRIPT2_MAX_PARAMS < 0
#error MAX_ERRORS must be greater than 0
#endif

#ifndef SCRIPT2_MAX_WALLS
#define SCRIPT2_MAX_WALLS 65535
#endif
#ifndef SCRIPT2_MAX_ERRORS
#define SCRIPT2_MAX_ERRORS 65535
#endif
#ifndef SCRIPT2_MAX_PARAMS
#define SCRIPT2_MAX_PARAMS 65535
#endif
#ifndef SCRIPT2_COM_TIMEOUT_TICKS
#define SCRIPT2_COM_TIMEOUT_TICKS 65535
#else
#if SCRIPT2_COM_TIMEOUT_TICKS < 0
#error MAX_ERRORS must be greater than 0
#endif
#endif
#ifndef SCRIPT2_MAX_ADDRESS_LENGTH
#define SCRIPT2_MAX_ADDRESS_LENGTH 65535
#endif
#ifndef SCRIPT2_OP_MAX_NAME_LENGTH
#define SCRIPT2_OP_MAX_NAME_LENGTH 65535
#endif
#ifndef SCRIPT2_OP_MAX_DECRABSION_LENGTH
#define SCRIPT2_OP_MAX_DECRABSION_LENGTH 65535
#endif
#ifndef SCRIPT2_TEXT_LENGTH
#define SCRIPT2_TEXT_LENGTH 65535
#endif
#ifndef SCRIPT2_BUFFER_SIZE_RX
#define SCRIPT2_BUFFER_SIZE_RX 65535
#endif
#ifndef SCRIPT2_BUFFER_SIZE_TX
#define SCRIPT2_BUFFER_SIZE_TX 65535
#endif

#ifndef SCRIPT2_WINDOW_SIZE_MIN
#define SCRIPT2_WINDOW_SIZE_MIN 512
#endif
#ifndef SCRIPT2_BUFFER_SIZE_DEFAULT
#define SCRIPT2_BUFFER_SIZE_DEFAULT 64 * 1024
#endif
#ifndef SCRIPT2_STACK_COUNT_MAX_DEFAULT
#define SCRIPT2_STACK_COUNT_MAX_DEFAULT 32
#endif
#ifndef SCRIPT2_FLOOR_SIZE
#define SCRIPT2_FLOOR_SIZE 1024
#endif
#ifndef SCRIPT2_STACK_COUNT_MAX_DEFAULT
#define SCRIPT2_STACK_COUNT_MAX_DEFAULT 16
#endif
#ifndef SCRIPT2_OBJ_SIZE_DEFAULT
#define SCRIPT2_OBJ_SIZE_DEFAULT 256
#endif
#ifndef SCRIPT2_TOKEN_COUNT
#define SCRIPT2_TOKEN_COUNT 32
#endif
#ifndef SCRIPT2_CPU_CACHE_LINE_SIZE
#define SCRIPT2_CPU_CACHE_LINE_SIZE 64
#endif
#ifndef SCRIPT2_STRAND_COUNT_DEFAULT
#define SCRIPT2_STRAND_COUNT_DEFAULT 28
#endif
#ifndef SCRIPT2_CIN_BUFFER_SIZE
#define SCRIPT2_CIN_BUFFER_SIZE 81
#endif
#ifndef SCRIPT2_UNICODE_VERSION_MAJOR
#define SCRIPT2_UNICODE_VERSION_MAJOR 12
#endif
#ifndef SCRIPT2_UNICODE_VERSION_MINOR
#define SCRIPT2_UNICODE_VERSION_MINOR 1
#endif
#ifndef SCRIPT2_UNICODE_ASSIGNED_CODE_POINTS
#define SCRIPT2_UNICODE_ASSIGNED_CODE_POINTS 277576
#endif
#ifndef SCRIPT2_CONSOLE_WIDTH
#define SCRIPT2_CONSOLE_WIDTH 80
#endif

// End overridable `pch.h` parameters.
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#ifdef ASSEMBLE_DYNAMIC_LIB
#ifdef LIB_EXPORT
#define LIB_MEMBER __declspec(dllexport)
#else
#define LIB_MEMBER __declspec(dllimport)
#endif
#else
#define LIB_MEMBER
#define LIB_INLINE /*< inline There is a design flaw from when I didn't know \
                       how c++ inlines work with DLLs. The inline code       \
                       has to go into the t_template.h files.                \
                       I would ideally like to make a templating engine to   \
                       make C++ single-file headers similar to how Nuklear's \
                    */
#endif

#if SCRIPT2_PLATFORM == OS_WINDOWS
#define APP_EXIT_SUCCESS 0
#define APP_EXIT_FAILURE 1
#else
#define APP_EXIT_SUCCESS 0
#define APP_EXIT_FAILURE 1
#endif

#if USING_128_BIT_POD_TYPES == YES_0
typedef int128_t SIH;
typedef uint128_t UIH;
#endif

#if SEAM < SEAM_SCRIPT2_UTF
#undef USING_UTF16
#undef USING_UTF32
#define USING_UTF16 NO_0
#define USING_UTF32 NO_0
#endif

#if SEAM < SEAM_SCRIPT2_FTOS
#undef USING_FP4
#undef USING_FP8
#define USING_FP4 NO_0
#define USING_FP8 NO_0
#endif

#if USING_FP == 4
typedef FP4 FPW;
#elif USING_FP == 8
typedef FP8 FPW;
#endif

#if CPU_WORD_SIZE == CPU_16_BIT
#define CPU_IS_LESS_THAN_32_BIT 1
#define CPU_IS_LESS_THAN_64_BIT 1
#define CPU_IS_GREATER_THAN_16_BIT 0
#define CPU_IS_GREATER_THAN_32_BIT 0
#elif CPU_WORD_SIZE == CPU_32_BIT
#define CPU_IS_LESS_THAN_32_BIT 0
#define CPU_IS_LESS_THAN_64_BIT 1
#define CPU_IS_GREATER_THAN_16_BIT 1
#define CPU_IS_GREATER_THAN_32_BIT 0
#elif CPU_WORD_SIZE == CPU_64_BIT
#define CPU_IS_LESS_THAN_32_BIT 0
#define CPU_IS_LESS_THAN_64_BIT 0
#define CPU_IS_GREATER_THAN_16_BIT 1
#define CPU_IS_GREATER_THAN_32_BIT 1
#endif

// Default character type.
#if USING_STR == STRING_TYPE_2
typedef CH2 CHR;
#elif USING_STR == STRING_TYPE_3
typedef CH4 CHR;
#else  // ASCII or UTF-8.
typedef CH1 CHR;
#endif

//< The Largest char.
#if LARGEST_CHAR == STRING_TYPE_2
typedef CH2 CHA;
#elif LARGEST_CHAR == STRING_TYPE_3
typedef CH4 CHA;
#else
typedef CH1 CHA;
#endif

#if CPU_WORD_SIZE == CPU_64_BIT
#if COMPILER_SUPPORTS_128_BIT_INTEGERS
#define LARGEST_POD_TYPE 16
#else
#define LARGEST_POD_TYPE 8
#endif
#elif CPU_WORD_SIZE == CPU_32_BIT
#define LARGEST_POD_TYPE 8
#else
#define LARGEST_POD_TYPE 4
#endif

#define PRIME_LARGEST_UI2 65521
#define PRIME_LARGEST_UI4 4294967291
#define PRIME_LARGEST_UI8 18446744073709551557

#define SIZEOF_ARRAY(type) ((CH1*)(&type + 1) - (CH1*)(&type))

namespace _ {
enum {
  kWordBitCount = (sizeof(void*) == 2)
                      ? 1
                      : (sizeof(void*) == 4) ? 2 : (sizeof(void*) == 8) ? 3 : 0,
  kWordLSbMask = sizeof(void*) - 1,
  kNaNInt = sizeof(SI4) == 4 ? static_cast<SI4>(0xFFFFFFFF)
                             : sizeof(SI4) == 2 ? static_cast<SI4>(0xFFFF) : 0,
  kCpuCacheLineSize = SCRIPT2_CPU_CACHE_LINE_SIZE,
  kMaxFloorsCount = SCRIPT2_MAX_WALLS,  //< Size of the Room Floor (socket).
  kMaxErrors = SCRIPT2_MAX_ERRORS,      //< Max errors before blowing up.
  kParamsMax = SCRIPT2_MAX_PARAMS,      //< Max number_ of parameters.
  kTimeoutMicroseconds = SCRIPT2_COM_TIMEOUT_TICKS,  //< Sub-second ticks.
  kAddressLengthMax =
      SCRIPT2_MAX_ADDRESS_LENGTH,  //< Max address (kADR) length.
  kOpNameLengthMax = SCRIPT2_OP_MAX_NAME_LENGTH,
  // Max length of a Op description .
  kOpDescriptionLengthMax = SCRIPT2_OP_MAX_DECRABSION_LENGTH,
  // Max length of a Text.
  kCharCount = SCRIPT2_TEXT_LENGTH,

  // Size of the Display Print Slot.
  kSlotBufferSizeRx = SCRIPT2_BUFFER_SIZE_RX,

  // Size of the KeyboardInBuffer.
  kSlotBufferSizeTx = SCRIPT2_BUFFER_SIZE_TX,

  kWindowSizeMin = SCRIPT2_WINDOW_SIZE_MIN,

  kBufferSizeDefault = SCRIPT2_BUFFER_SIZE_DEFAULT,
  kFloorSize = SCRIPT2_FLOOR_SIZE,  //< Size, or initial size, of the Floor.
  kStackCountMaxDefault = SCRIPT2_STACK_COUNT_MAX_DEFAULT,
  kObjSizeDefault = SCRIPT2_OBJ_SIZE_DEFAULT,
  kNo = 0,   //< Script no/false value.
  kYes = 1,  //< Script yes/true value.

  // Extra reserved memory at the stop of BOut.
  kBOutOverflowSize = 32,

  kWordAddressMask = kWordLSbMask,  //< For masking the word address.
  kPrintC0Offset = 176,             //< Value add to values 0-32 when printing.
  kTokenLongest = 31,               //< Max length of a token plus one.
  kMinStackSize = 1,                //< Min Crabs stack size.
  kBufferSizeDefaultWords =
      kBufferSizeDefault / sizeof(SIW) + kBufferSizeDefault % sizeof(SIW) ? 1
                                                                          : 0,
  kSTRCount = SCRIPT2_STRAND_COUNT_DEFAULT,
  kConsoleWidth = SCRIPT2_CONSOLE_WIDTH,
  kCInBufferSize =
      SCRIPT2_CIN_BUFFER_SIZE,         //< Preallocated CIn buffer char count.
  kKeyEnter = 0,                       //< The keyboard value for enter.
  kLargestPODType = LARGEST_POD_TYPE,  //< The largest POD type.
};

enum {
  kLargestPODTypeSize = LARGEST_POD_TYPE,
};

}  // namespace _

#undef MAX_ERRORS
#undef MAX_NUM_PARAMS
#undef MAX_STRING_LENGTH
#undef COM_TIMEOUT_TICKS
#undef SCRIPT2_LOG_SIZE
#undef OPERATION_MAX_NAME_LENGTH
#undef OPERATION_MAX_DECRABSION_LENGTH
#undef SCRIPT2_TEXT_LENGTH
#undef SCRIPT2_BUFFER_SIZE_RX
#undef SCRIPT2_BUFFER_SIZE_TX
#undef SCRIPT2_WINDOW_SIZE_MIN
#undef SCRIPT2_BUFFER_SIZE_DEFAULT
#undef SCRIPT2_STACK_COUNT_MAX_DEFAULT
#undef SCRIPT2_FLOOR_SIZE
#undef SCRIPT2_STACK_COUNT_MAX_DEFAULT
#undef SCRIPT2_OBJ_SIZE_DEFAULT
#undef SCRIPT2_TOKEN_COUNT
#undef SCRIPT2_CPU_CACHE_LINE_SIZE
#undef SCRIPT2_STRAND_COUNT_DEFAULT
