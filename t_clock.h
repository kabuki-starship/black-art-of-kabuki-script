/* SCRIPT Script @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /script2/c_lock.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <cale@astartup.net>;
All right reserved (R). This Source Code Form is subject to the terms of the
Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with
this file, You can obtain one at <https://mozilla.org/MPL/2.0/>. */

#pragma once
#include <pch.h>

#if SEAM >= SEAM_SCRIPT2_CLOCK

#ifndef INCLUDED_SCRIPT2_TCLOCK
#define INCLUDED_SCRIPT2_TCLOCK

#include "c_clock.h"
#include "c_test.h"
#include "t_utf.h"

#if SEAM == SEAM_SCRIPT2_CLOCK
#include "module_debug.inl"
#else
#include "module_release.inl"
#endif

namespace _ {

template <typename SI>
AClock* TClockInit(AClock& clock, SI t) {
  // Algorithm:
  // 1. Using manual modulo convert in the following order:
  //   a. Year based on seconds per year.
  //   b. Day of year based on seconds per day.
  //   c. Month based on day of year and leap year.
  //   d. Hour.
  //   e. Minute.
  //   f. Second.
  SIN value = (SIN)(t / kSecondsPerYear);
  t -= value * kSecondsPerYear;
  clock.year = value + ClockEpoch();
  value = (SIN)(t / kSecondsPerDay);
  t -= value * kSecondsPerDay;
  clock.day = value;
  value = (SIN)(t / kSecondsPerHour);
  t -= value * kSecondsPerHour;
  clock.hour = value;
  value = (SIN)(t / kSecondsPerMinute);
  clock.minute = value;
  clock.second = (SIN)(t - value * kSecondsPerMinute);
  return &clock;
}

template <typename SI>
SI TClockTime(SIN year, SIN month, SIN day, SIN hour, SIN minute, SIN second) {
  if (year >= (ClockEpoch() + 10)) {
    if (month >= 1 && day >= 19 && hour >= 3 && minute >= 14 && second >= 7)
      return 0;
  }
  if (month < 1 || month >= 12 || hour >= 23 || minute >= 60 || second >= 60)
    return 0;
  return (SI)((year - ClockEpoch()) * kSecondsPerYear +
              ClockDayOfYear(year, month, day) * kSecondsPerDay +
              hour * kSecondsPerHour + minute * kSecondsPerMinute + second);
}

/*
template <typename Char = CHR>
Char* Print(Char* cursor, Char* stop, TME& t) {
  AClock c (t.seconds);
  cursor = TPrint<Char>(cursor, stop, c);
  cursor = TPrint<Char>(cursor, stop, ':');
  return TPrint<Char>(cursor, stop, t.ticks);
}*/

#if USING_STR

template <typename Char = CHR>
Char* TPrint(Char* cursor, Char* stop, const AClock& clock) {
  // The way the utf functions are setup, we return a nil-term CH1 so we
  // don't have to check to write a single CH1 in this
  A_ASSERT(cursor);
  A_ASSERT(cursor < stop);

  cursor = TPrint<Char>(cursor, stop, clock.year + ClockEpoch());
  if (!cursor) return nullptr;
  *cursor++ = '-';
  cursor = TPrint<Char>(cursor, stop, clock.month + 1);
  if (!cursor) return nullptr;
  *cursor++ = '-';
  cursor = TPrint<Char>(cursor, stop, clock.day);
  if (!cursor) return nullptr;
  *cursor++ = '@';
  cursor = TPrint<Char>(cursor, stop, clock.hour);
  if (!cursor) return nullptr;
  *cursor++ = ':';
  cursor = TPrint<Char>(cursor, stop, clock.minute);
  if (!cursor) return nullptr;
  *cursor++ = ':';
  cursor = TPrint<Char>(cursor, stop, clock.second);
  if (!cursor) return nullptr;
  return cursor;
}

template <typename Printer>
Printer& TPrint(Printer& o, const _::AClock& clock) {
  return o << clock.year + ClockEpoch() << '-' << clock.month + 1 << '-'
           << clock.day << '@' << clock.hour << ':' << clock.minute << ':'
           << clock.second;
}

template <typename Char = CHR, typename Time = TM8>
Char* TClockPrint(Char* cursor, Char* stop, Time t) {
  AClock clock;
  ClockInit(clock, t);
  return TPrint<Char>(cursor, stop, clock);
}

template <typename Char = CHR>
Char* TPrint(Char* cursor, Char* stop, TME& t) {
  // The way the utf functions are setup, we return a nil-term CH1 so we
  // don't have to check to write a single CH1 in this
  A_ASSERT(cursor);
  A_ASSERT(cursor < stop);

  AClock clock;
  ClockInit(clock, t.seconds);
  cursor = TPrint<Char>(cursor, stop, clock);
  *cursor++ = ':';
  cursor = TPrint<Char>(cursor, stop, t.ticks);
  if (!cursor) return nullptr;
  return cursor;
}

template <typename Printer>
Printer& TPrint(Printer& o, TME& t) {
  AClock clock;
  ClockInit(clock, t.seconds);
  return TPrint<Printer>(o, clock) << ':' << t.ticks;
}

/* Scans a time in seconds from the given string. */
template <typename Char = CHR>
const Char* TScanTime(const Char* string, SI4& hour, SI4& minute, SI4& second) {
  if (string == nullptr) return nullptr;

  D_PRINTF("\n\n    Scanning time:%s", string);
  Char c;  //< The current Char.
  SI4 h,   //< Hour.
      m,   //< Minute.
      s;   //< Second.
  if (!TScanSigned<SI4, UI4, Char>(++string, h)) {
    D_PRINTF("\nInvalid hour:%i", h);
    return nullptr;
  }
  string = TSTRSkipNumbers<Char>(string);
  if (h < 0) {
    D_PRINTF("\nHours:%i can't be negative.", h);
    return nullptr;
  }
  if (h > 23) {
    D_PRINTF("\nHours:%i can't be > 23.", h);
    return nullptr;
  }
  D_COUT(h);
  c = *string++;
  if (!c || TIsWhitespace<Char>(c)) {  // Case @HH
    D_COUT(" HH ");
    // Then it's a single number_, so create a TM4 for the current
    // user-time hour..
    hour = h;
    return string;
  }
  c = TLowercase<Char>(c);
  if (c == 'a') {  //
    D_COUT("\nCase @HHAm\n HHam ");

    if (TLowercase<Char>(c = *string++) == 'm') c = *string++;
    if (c && !TIsWhitespace<Char>(c)) {
      D_COUT("\nInvalid am format.");
      return nullptr;
    }
    D_COUT(" @HHAM ");
    hour = h;
    return string;
  }
  if (c == 'p') {
    D_COUT(" Case @HHpm ");
    c = *string++;
    if (TLowercase<Char>(c) == 'm') c = *string++;
    if (c && !TIsWhitespace<Char>(c)) {
      D_COUT("\ninvalid pm format.");
      return nullptr;
    }
    D_PRINTF("\nCase @HHPM %i:00:00", h + 12);
    hour = h + 12;
    return string;
  }
  if (c != ':') {
    D_PRINTF("\nExpecting ':'.");
    return nullptr;
  }

  D_PRINTF(
      "\nCases HH:MM, HH::MMam, HH::MMpm, HH:MM:SS, HH:MM:SSam, and "
      "HH:MM:SSpm");

  if (!TScanSigned<SI4, UI4, Char>(string, m)) return nullptr;
  string = TSTRSkipNumbers<Char>(string);
  if (m < 0) {
    D_PRINTF("\nMinutes:%i can't be negative!", m);
    return nullptr;
  }
  if (m >= 60) {
    D_PRINTF("\nMinutes:%i can't be >= 60!", m);
    return nullptr;  //< 60 minutes in an hour.
  }
  D_PRINTF(":%i", m);

  string = TSTRSkipNumbers<Char>(string);
  c = *string++;
  if (!c || TIsWhitespace<Char>(c)) {
    D_COUT(" HH:MM ");
    hour = h;
    minute = m;
    return string;
  }
  c = TLowercase<Char>(c);
  if (c == 'a') {
    D_COUT(" HH:MMam ");
    c = *string++;
    if (TLowercase<Char>(c) == 'm') {  // The 'm' is optional.
      c = *string++;
    }
    if (c && !TIsWhitespace<Char>(c)) {  // The space is not.
      D_COUT("Invalid am in HH::MM AM");
      return nullptr;
    }
    hour = h;
    minute = m;
    return string;
  }
  if (c == 'p') {  //< Case HH:MM PM
    D_COUT(" HH:MMpm ");
    c = *string++;
    if (TLowercase<Char>(c) == 'm') {  //< The 'm' is optional.
      c = *string++;
    }
    if (c && !TIsWhitespace<Char>(c)) {  //< The space is not.
      D_COUT("Invalid am in HH::MM PM");
      return nullptr;
    }
    hour = h + 12;
    minute = m;
    return string;
  }
  if (c != ':') return nullptr;

  D_COUT("\n    Cases HH:MM:SS, HH:MM:SSam, and HH:MM:SSpm");

  if (!TScanSigned<SI4, UI4, Char>(string, s)) return nullptr;
  if (s < 0) {
    D_PRINTF("\nSeconds:%i can't be negative!", s);
    return nullptr;
  }
  if (s > 60) {
    D_PRINTF("\nSeconds:%i can't be >= 60!", s);
    return nullptr;
  }
  D_PRINTF(":%i", s);
  string = TSTRSkipNumbers<Char>(string);
  c = TLowercase<Char>(*string);
  if (!c || TIsWhitespace<Char>(c)) {
    D_PRINTF(" HH:MM:SS ");
    hour = h;
    minute = m;
    second = s;
    return string;
  }
  if (c == 'a') {
    D_COUT(" HH:MM:SSam ");
    c = *string++;
    if (TLowercase<Char>(c) == 'm') {  //< The 'm' is optional.
      c = *string++;
    }
    if (!c || !TIsWhitespace<Char>(c)) {  //< The space is not.
      D_COUT("\nInvalid am in HH::MM:SS AM");
      return nullptr;
    }
    hour = h;
    minute = m;
    second = s;
    return string;
  }
  if (c != 'p') {
    D_PRINTF("\nExpecting a PM but found:%c", c);
    return nullptr;  // Format error!
  }
  D_PRINTF(" HH:MM:SSpm ");
  c = TLowercase<Char>(*string++);
  if (c == 'm') {  //< The 'm' is optional.
    c = *string++;
  }
  if (!c || !TIsWhitespace<Char>(c)) {  //< The space is not.
    D_COUT("\nInvalid am in HH::MM:SS PM");
    return nullptr;
  }
  hour = h + 12;
  minute = m;
  second = s;
  return string;
}

/* Scans the given string for a timestamp. */
template <typename Char = CHR>
const Char* TScan(const Char* string, AClock& clock) {
  D_ASSERT(string);
  D_PRINTF("\n    Scanning AClock: %s\n    Scanning: ", string);

  string = TSTRSkipChar<Char>(string, '0');
  Char c = *string,  //< The current Char.
      delimiter;     //< The delimiter.
  const Char* stop;  //< Might not need

  SI4 hour = 0, minute = 0, second = 0;

  if (c == '@') {
    if (!(string = TScanTime<Char>(string, hour, minute, second))) {
      D_COUT("\nCase @ invalid time");
      return nullptr;
    }
    clock.hour = hour;
    clock.minute = minute;
    clock.second = second;

    return string + 1;
  }
  if (c == '#') {
    if (!(string = TScanTime<Char>(string, hour, minute, second))) {
      D_COUT("\nCase @ invalid time");
    }
    clock.hour += hour;
    clock.minute += minute;
    clock.second += second;

    return string + 1;
  }

  SI4 value1,            //< The first date field scanned.
      value2,            //< The second date field scanned.
      value3,            //< The third date field scanned.
      is_last_year = 0;  //< Flag for if the date was last year or not.

  // Scan value1
  if (!TScanSigned<SI4, UI4, Char>(string, value1)) {
    D_COUT("Scan error at value1");
    return nullptr;
  }
  if (value1 < 0) {
    D_COUT("Dates can't be negative.");
    return nullptr;
  }
  string = TSTRDecimalEnd<Char>(string);
  if (!string) return nullptr;
  delimiter = *string++;
  D_PRINTF("%i%c", value1);
  if (delimiter == '@') {
    D_COUT(" HH@ ");

    if (!(string = TScanTime<Char>(string, hour, minute, second))) {
      D_COUT("\nInvalid time DD@");
      return nullptr;
    }
    clock.day = value1;

    return string + 1;
  }
  // Scan value2.
  string = TSTRSkipChar<Char>(string, '0');
  if (!TScanSigned<SI4, UI4, Char>(string, value2)) {
    D_COUT("\n    Failed scanning value2 of date.");
    return nullptr;
  }
  if (value2 < 0) {
    D_COUT("Day can't be negative.");
    return nullptr;  //< Invalid month and day.
  }
  D_PRINTF("%i", value2);
  string = TSTRDecimalEnd<Char>(string);
  c = *string;
  if (c != delimiter) {
    D_COUT("\n    Cases MM/DD and MM/YYyy");
    if (c == '@') {
      if (!(string = TScanTime<Char>(string, hour, minute, second))) {
        D_COUT(" invalid time ");
        return nullptr;
      }
    }
    if (!c || TIsWhitespace<Char>(c)) {
      D_PRINTF("\n    Format is MM/DD and year is %i",
               clock.year + ClockEpoch());
      // is_last_year = ((value1 >= std_tm.mon) &&
      //    (value2 >= std_tm.mday)) ? 0:1;
      clock.year += is_last_year;
      clock.month = value1 - 1;
      clock.day = value2;
      clock.hour = 0;
      clock.hour = hour;
      clock.minute = minute;
      clock.second = second;

      return string + 1;
    }
    c = TLowercase<Char>(c);
    if ((value1 < 12) && (value2 > 0) &&
        (value2 <= ClockMonthDayCount(value1))) {
      D_COUT(" MM/DD ");
      if (value1 > 11) {
        D_COUT("\nInvalid MM/DD@ month");
        return nullptr;
      }
      // We need to find out what year it is.
      AClock clock;
      ClockInit(clock);

      if (value2 > ClockMonthDayCount(clock.year - ClockEpoch(), value1)) {
        D_COUT("\nInvalid MM/DD@ day");
        return nullptr;
      }
      clock.month = value1 - 1;
      clock.day = value2;
      clock.hour = hour;
      clock.minute = minute;
      clock.second = second;
      if (!(string = TScanTime(string, hour, minute, second))) {
        D_COUT("\nInvalid MM/DD@");
        return nullptr;
      }

      return string + 1;
    }
    if ((value1 < 12) && (value2 > ClockMonthDayCount(value1))) {
      D_COUT(" MM/YYyy");
      clock.month = value1 - 1;
      clock.year = value2;
      if (!(string = TScanTime<Char>(string, hour, minute, second))) {
        D_COUT("\nInvalid MM / YYYY@ time");
        return nullptr;
      }

      return string + 1;
    }
    D_COUT("\nInvalid MM/DD or MM/YYyy format");
    return nullptr;
  }

  // Formats MM/DD/YYyy and YYyy/MM/DD

  string = TSTRSkipChar<Char>(++string, '0');
  c = *string;
  // Then there are 3 values and 2 delimiters.
  if (!TIsDigit<Char>(c) || !TScanSigned<SI4, UI4, Char>(string, value3)) {
    D_PRINTF("\n    SlotRead error reading value3 of date. %c: ", c);
    return nullptr;  //< Invalid format!
  }
  string = TSTRDecimalEnd<Char>(string);
  D_PRINTF("%c%i", c, value3);
  // Now we need to check what format it is in.

  c = *string;
  if (c == '@') {
    if (!(stop = TScanTime<Char>(string, hour, minute, second))) {
      D_COUT("Invalid YYyy/MM/DD@ time.");
      return nullptr;
    }
  }
  clock.hour = hour;
  clock.minute = minute;
  clock.second = second;
  if (TIsWhitespace<Char>(*(++string))) {
    D_COUT("No date found.");
    return nullptr;
  }
  if (value1 > 11) {  //<
    D_COUT("\n    Case YYyy/MM/DD");
    if (value2 == 0 || value2 > 12) {
      D_PRINTF("Invalid number of months");
      return nullptr;
    }

    if (value2 > ClockMonthDayCount(value2, value1)) {
      D_COUT("Invalid number of days");
      return nullptr;
    }  // 17/05/06

    if (value1 < 100) {
      D_COUT("\n    Case YY/MM/DD");
      value1 += 2000 - ClockEpoch();
    } else {
      D_COUT("\n    Case YYYY/MM/DD");
      value1 -= ClockEpoch();
    }

    clock.year = value1;
    clock.month = value2 - 1;
    clock.day = value3;

    return string + 1;
  }
  D_COUT("\n    Cases MM/DD/YY and MM/DD/YYYY");

  if (value1 > 11) {
    D_COUT("\nInvalid month.");
    return nullptr;
  }
  if (value2 > ClockMonthDayCount(value1, value3)) {
    D_COUT("\nInvalid day.");
    return nullptr;
  }
  clock.month = value1 - 1;
  clock.day = value2;
  if (value3 < 100) {
    D_COUT("\n    Case MM/DD/YY");
    clock.year = value3 + (2000 - ClockEpoch());
  } else {
    D_COUT("\n    Case MM/DD/YYYY");
    clock.year = value3 - ClockEpoch();
  }
  return string + 1;
}

template <typename Char, typename SI>
const Char* TScanTime(const Char* begin, TM4& result) {
  AClock clock;
  const Char* stop = TScan<Char>(begin, clock);
  result = (TM4)ClockSeconds(clock);
  return stop;
}

template <typename Char, typename SI>
const Char* TScanTime(const Char* begin, TM8& result) {
  AClock clock;
  const Char* stop = TScan<Char>(begin, clock);
  result = (TM8)ClockSeconds(clock);
  return stop;
}

template <typename Char>
const Char* TScan(const Char* begin, TME& result) {
  begin = TScanTime<Char, TM4>(begin, result.seconds);
  if (!begin) return nullptr;
  if (*begin++ != ':') {
    result.ticks = 0;
    return begin - 1;
  }
  return TScanUnsigned<UI4, Char>(begin, result.ticks);
}
#endif  // #if USING_STR

template <typename SI>
SI TClockSet(AClock* clock, SI t) {
  // Algorithm:
  // 1. Using manual modulo convert in the following order:
  //   a. Year based on seconds per year.
  //   b. Day of year based on seconds per day.
  //   c. Month based on day of year and leap year.
  //   d. Hour.
  //   e. Minute.
  //   f. Second.
  SI value = t / kSecondsPerYear;
  t -= value * kSecondsPerYear;
  clock->year = (SI4)(value + ClockEpoch());
  value = t / kSecondsPerDay;
  t -= value * kSecondsPerDay;
  clock->day = (SI4)value;
  value = t / kSecondsPerHour;
  t -= value * kSecondsPerHour;
  clock->hour = (SI4)value;
  value = t / kSecondsPerMinute;
  clock->minute = (SI4)value;
  clock->second = (SI4)(t - value * kSecondsPerMinute);
  return t;
}

/* A time in seconds stored as either a 32-bit or 64-bit SI.
The difference between a TClock and AClock is that that TClock stores the AClock
and the TM8 or TM4. */
template <typename SI>
struct LIB_MEMBER TClock {
  AClock clock;  //< A human-readable clock.

  /* Constructs a clock from the given seconds timestamp. */
  TClock(SI t) { ClockInit(clock, t); }

  /* Prints the given */
  template <typename Printer, typename SI>
  Printer& Print(Printer& o) {
    return o << clock.Clock();
  }
};

}  // namespace _
#if USING_CONSOLE == YES_0

inline _::COut& operator<<(_::COut& o, const _::AClock& item) {
  return _::TPrint(o, item);
}

template <typename SI>
_::COut& operator<<(_::COut& o, _::TClock<SI> item) {
  return o << item.clock;
}
#endif

#endif
#endif
