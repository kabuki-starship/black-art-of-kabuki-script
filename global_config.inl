#include <climits>
#include <cstdarg>
#include <cstdint>
#include <new>

#if defined(_MSC_VER) && defined(_M_AMD64)
#include <intrin.h>
#endif

typedef char CH1;
typedef char16_t CH2;
typedef char32_t CH4;
typedef wchar_t CHN;

typedef int8_t SI1;
typedef uint8_t UI1;
typedef int16_t SI2;
typedef uint16_t UI2;
typedef bool BOL;
typedef int32_t SI4;
typedef uint32_t UI4;
typedef int32_t TM4;
typedef int64_t TM8;
typedef int64_t SI8;
typedef uint64_t UI8;

typedef UI2 FP2;
typedef float FP4;
typedef double FP8;

typedef uintptr_t UIW;
typedef intptr_t SIW;

typedef int SIN;
typedef unsigned int UIN;

typedef SI4 TM4;  //< A 32-bit seconds since epoch timestamp.
typedef SI8 TM8;  //< A 64-bit seconds since epoch timestamp.

typedef UI2 UI2;  //< Using unsigned 16-bit hash type.
typedef UI4 UI4;  //< Using unsigned 32-bit hash type.
typedef UI8 UI8;  //< Using unsigned 64-bit hash type.

#if SEAM < SCRIPT2_3
#undef USING_UTF16
#undef USING_UTF32
#define USING_UTF16 NO
#define USING_UTF32 NO
#endif

#if SEAM < SCRIPT2_4
#undef USING_FP4
#undef USING_FP8
#define USING_FP4 NO
#define USING_FP8 NO
#else
#if ALU_SIZE == 32
typedef FP4 FPW;
#elif ALU_SIZE == 64
typedef FP8 FPW;
#endif
#endif

#ifndef USING_UTF
#define USING_UTF 1
#endif

#if USING_UTF == 1
typedef CH1 CHR;
#elif USING_UTF == 2
typedef CH2 CHR;
#elif USING_UTF == 4
typedef CH4 CHR;
#endif

#if SCRIPT2_MAX_PARAMS < 0
#error MAX_ERRORS must be greater than 0
#endif

#if SCRIPT2_TEXT_SIZE_DEFAULT < 0
#error MAX_ERRORS must be greater than 0
#endif

#if COM_TIMEOUT_TICKS < 0
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
#ifndef COM_TIMEOUT_TICKS
#define COM_TIMEOUT_TICKS 65535
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

#define PRIME_LARGEST_UI4 4294967291
#define PRIME_LARGEST_UI8 18446744073709551557

namespace _ {
enum {
  NIL = 0,
  kNUL = 0,                  //< ASCII NUL 0
  kSOH,                      //< ASCII NUL 1
  kSTX,                      //< ASCII STX 2
  kETX,                      //< ASCII ETX 3
  kEOT,                      //< ASCII EOT 4
  kENQ,                      //< ASCII ENQ 5
  kACK,                      //< ASCII ACK 6
  kBEL,                      //< ASCII BEL 7
  kBS,                       //< ASCII BS 8
  kTAB,                      //< ASCII TAB 9
  kLF,                       //< ASCII LF 10
  kVT,                       //< ASCII VT 11
  kFF,                       //< ASCII FF 12
  kCR,                       //< ASCII CR 13
  kSO,                       //< ASCII SO 14
  kSI,                       //< ASCII SI 15
  kDLE,                      //< ASCII DLE 16
  kDC1,                      //< ASCII DC1 17
  kDC2,                      //< ASCII DC2 18
  kDC3,                      //< ASCII DC3 19
  kDC4,                      //< ASCII DC4 20
  kNAK,                      //< ASCII NAK 21
  kSYN,                      //< ASCII SYN 22
  kETB,                      //< ASCII ETB 23
  kCAN,                      //< ASCII CAN 24
  kEM,                       //< ASCII EM 25
  kSUB,                      //< ASCII SUB 26
  kESC,                      //< ASCII ESC 27
  kFS,                       //< ASCII FS 28
  kGS,                       //< ASCII GS 29
  kRS,                       //< ASCII RS 30
  kUS,                       //< ASCII US 31
  kDEL = 127,                //< ASCII DEL 127
  kPrimeLargestUI2 = 65535,  //< Largest 16-bit prime.
  kWordBitCount = (sizeof(void*) == 2)
                      ? 1
                      : (sizeof(void*) == 4) ? 2 : (sizeof(void*) == 8) ? 3 : 0,
  kWordLSbMask = sizeof(void*) - 1,
  kAlignMask = 0x7,
  kNaNInt = sizeof(SI4) == 4 ? static_cast<SI4>(0xFFFFFFFF)
                             : sizeof(SI4) == 2 ? static_cast<SI4>(0xFFFF) : 0,
  kCpuCacheLineSize = SCRIPT2_CPU_CACHE_LINE_SIZE,
  kMaxFloorsCount = SCRIPT2_MAX_WALLS,  //< Size of the Room Floor (socket).
  kMaxErrors = SCRIPT2_MAX_ERRORS,      //< Max errors before blowing up.
  kParamsMax = SCRIPT2_MAX_PARAMS,      //< Max number_ of parameters.
  kTimeoutMicroseconds = COM_TIMEOUT_TICKS,  //< Timeout time in microseconds.
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
  kMinStackSize = 1,                //< Min CCrabs stack size.
  kBufferSizeDefaultWords =
      kBufferSizeDefault / sizeof(SIW) + kBufferSizeDefault % sizeof(SIW) ? 1
                                                                          : 0,
  kSTRCount = SCRIPT2_STRAND_COUNT_DEFAULT,
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

/*
// Largest 64-bit prime: 18446744073709551557
static const UI8 kLargest64BitPrime = 0xFFFFFFFFFFFFFFC5;
enum {
  kLargest16BitPrime = 0xFFE1,      //< Largest 16-bit prime: 65521.
  kLargest32BitPrime = 0xFFFFFFFB,  //< Largest 32-bit prime: 4294967291.
};*/
