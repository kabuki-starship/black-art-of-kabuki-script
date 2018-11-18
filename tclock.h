/* Script^2 @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /clock.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= _0_0_0__04
#ifndef INCLUDED_SCRIPTTCLOCK
#define INCLUDED_SCRIPTTCLOCK

#include "cclock.h"

#include "ctest.h"
#include "tutf.h"

#if SEAM == _0_0_0__04
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif
namespace _ {

#if USING_UTF

template <typename Char = char>
Char* TPrint(Char* cursor, Char* stop, const CClock& clock) {
  // The way the utf functions are setup, we return a nil-term char so we
  // don't have to check to write a single char in this
  ASSERT(cursor);
  ASSERT(cursor < stop);

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

template <typename Char = char>
Char* TPrint(Char* cursor, Char* stop, Tss& t) {
  // The way the utf functions are setup, we return a nil-term char so we
  // don't have to check to write a single char in this
  ASSERT(cursor);
  ASSERT(cursor < stop);

  CClock clock(t.seconds);
  cursor = TPrint<Char>(cursor, stop, clock);
  *cursor++ = ':';
  cursor = TPrint<Char>(cursor, stop, t.ticks);
  if (!cursor) return nullptr;
  return cursor;
}

/* Scans a time in seconds from the given string. */
template <typename Char = char>
const Char* TStringScanTime(const Char* string, int& hour, int& minute,
                            int& second) {
  if (string == nullptr) return nullptr;

  PRINTF("\n\n    Scanning time:%s", string);
  Char c;  //< The current Char.
  int h,   //< Hour.
      m,   //< Minute.
      s;   //< Second.
  if (!TScanSigned<int, Char>(++string, h)) {
    PRINTF("\nInvalid hour:%i", h);
    return nullptr;
  }
  string = TStringSkipNumbers<Char>(string);
  if (h < 0) {
    PRINTF("\nHours:%i can't be negative.", h);
    return nullptr;
  }
  if (h > 23) {
    PRINTF("\nHours:%i can't be > 23.", h);
    return nullptr;
  }
  PRINT(h);
  c = *string++;
  if (!c || TIsWhitespace<Char>(c)) {  // Case @HH
    PRINT(" HH ");
    // Then it's a single number, so create a TMS for the current
    // user-time hour..
    hour = h;
    return string;
  }
  c = TLowercase<Char>(c);
  if (c == 'a') {  //
    PRINT("\nCase @HHAm\n HHam ");

    if (TLowercase<Char>(c = *string++) == 'm') c = *string++;
    if (c && !TIsWhitespace<Char>(c)) {
      PRINT("\nInvalid am format.");
      return nullptr;
    }
    PRINT(" @HHAM ");
    hour = h;
    return string;
  }
  if (c == 'p') {
    PRINT(" Case @HHpm ");
    c = *string++;
    if (TLowercase<Char>(c) == 'm') c = *string++;
    if (c && !TIsWhitespace<Char>(c)) {
      PRINT("\ninvalid pm format.");
      return nullptr;
    }
    PRINTF("\nCase @HHPM %i:00:00", h + 12);
    hour = h + 12;
    return string;
  }
  if (c != ':') {
    PRINTF("\nExpecting ':'.");
    return nullptr;
  }

  PRINTF(
      "\nCases HH:MM, HH::MMam, HH::MMpm, HH:MM:SS, HH:MM:SSam, and "
      "HH:MM:SSpm");

  if (!TScanSigned<int, Char>(string, m)) return nullptr;
  string = TStringSkipNumbers<Char>(string);
  if (m < 0) {
    PRINTF("\nMinutes:%i can't be negative!", m);
    return nullptr;
  }
  if (m >= 60) {
    PRINTF("\nMinutes:%i can't be >= 60!", m);
    return nullptr;  //< 60 minutes in an hour.
  }
  PRINTF(":%i", m);

  string = TStringSkipNumbers<Char>(string);
  c = *string++;
  if (!c || TIsWhitespace<Char>(c)) {
    PRINT(" HH:MM ");
    hour = h;
    minute = m;
    return string;
  }
  c = TLowercase<Char>(c);
  if (c == 'a') {
    PRINT(" HH:MMam ");
    c = *string++;
    if (TLowercase<Char>(c) == 'm') {  // The 'm' is optional.
      c = *string++;
    }
    if (c && !TIsWhitespace<Char>(c)) {  // The space is not.
      PRINT("Invalid am in HH::MM AM");
      return nullptr;
    }
    hour = h;
    minute = m;
    return string;
  }
  if (c == 'p') {  //< Case HH:MM PM
    PRINT(" HH:MMpm ");
    c = *string++;
    if (TLowercase<Char>(c) == 'm') {  //< The 'm' is optional.
      c = *string++;
    }
    if (c && !TIsWhitespace<Char>(c)) {  //< The space is not.
      PRINT("Invalid am in HH::MM PM");
      return nullptr;
    }
    hour = h + 12;
    minute = m;
    return string;
  }
  if (c != ':') return nullptr;

  PRINT("\n    Cases HH:MM:SS, HH:MM:SSam, and HH:MM:SSpm");

  if (!TScanSigned<int, Char>(string, s)) return nullptr;
  if (s < 0) {
    PRINTF("\nSeconds:%i can't be negative!", s);
    return nullptr;
  }
  if (s > 60) {
    PRINTF("\nSeconds:%i can't be >= 60!", s);
    return nullptr;
  }
  PRINTF(":%i", s);
  string = TStringSkipNumbers<Char>(string);
  c = TLowercase<Char>(*string);
  if (!c || TIsWhitespace<Char>(c)) {
    PRINTF(" HH:MM:SS ");
    hour = h;
    minute = m;
    second = s;
    return string;
  }
  if (c == 'a') {
    PRINT(" HH:MM:SSam ");
    c = *string++;
    if (TLowercase<Char>(c) == 'm') {  //< The 'm' is optional.
      c = *string++;
    }
    if (!c || !TIsWhitespace<Char>(c)) {  //< The space is not.
      PRINT("\nInvalid am in HH::MM:SS AM");
      return nullptr;
    }
    hour = h;
    minute = m;
    second = s;
    return string;
  }
  if (c != 'p') {
    PRINTF("\nExpecting a PM but found:%c", c);
    return nullptr;  // Format error!
  }
  PRINTF(" HH:MM:SSpm ");
  c = TLowercase<Char>(*string++);
  if (c == 'm') {  //< The 'm' is optional.
    c = *string++;
  }
  if (!c || !TIsWhitespace<Char>(c)) {  //< The space is not.
    PRINT("\nInvalid am in HH::MM:SS PM");
    return nullptr;
  }
  hour = h + 12;
  minute = m;
  second = s;
  return string;
}

/* Scans the given string for a timestamp. */
template <typename Char = char>
const Char* TScan(const Char* cursor, CClock& clock) {
  ASSERT(cursor);
  PRINTF("\n    Scanning CClock: %s\n    Scanning: ", cursor);

  cursor = TStringSkipChar<Char>(cursor, '0');
  Char c = *cursor,  //< The current Char.
      delimiter;     //< The delimiter.
  const Char* stop;  //< Might not need

  int hour = 0, minute = 0, second = 0;

  if (c == '@') {
    if (!(cursor = TStringScanTime<Char>(cursor, hour, minute, second))) {
      PRINT("\nCase @ invalid time");
      return nullptr;
    }
    clock.hour = hour;
    clock.minute = minute;
    clock.second = second;

    return cursor + 1;
  }
  if (c == '#') {
    if (!(cursor = TStringScanTime<Char>(cursor, hour, minute, second))) {
      PRINT("\nCase @ invalid time");
    }
    clock.hour += hour;
    clock.minute += minute;
    clock.second += second;

    return cursor + 1;
  }

  int value1,            //< The first date field scanned.
      value2,            //< The second date field scanned.
      value3,            //< The third date field scanned.
      is_last_year = 0;  //< Flag for if the date was last year or not.

  // Scan value1
  if (!TScanSigned<int, Char>(cursor, value1)) {
    PRINT("Scan error at value1");
    return nullptr;
  }
  if (value1 < 0) {
    PRINT("Dates can't be negative.");
    return nullptr;
  }
  cursor = TStringDecimalEnd<Char>(cursor);
  if (!cursor) return nullptr;
  delimiter = *cursor++;
  PRINTF("%i%c", value1);
  if (delimiter == '@') {
    PRINT(" HH@ ");

    if (!(cursor = TStringScanTime<Char>(cursor, hour, minute, second))) {
      PRINT("Invalid time DD@");
      return nullptr;
    }
    clock.day = value1;

    return cursor + 1;
  }
  // Scan value2.
  cursor = TStringSkipChar<Char>(cursor, '0');
  if (!TScanSigned<int, Char>(cursor, value2)) {
    PRINT("Failed scanning value2 of date.");
    return nullptr;
  }
  if (value2 < 0) {
    PRINT("Day can't be negative.");
    return nullptr;  //< Invalid month and day.
  }
  PRINTF("%i", value2);
  cursor = TStringDecimalEnd<Char>(cursor);
  c = *cursor;
  if (c != delimiter) {
    PRINT("\n    Cases MM/DD and MM/YYyy");
    if (c == '@') {
      if (!(cursor = TStringScanTime<Char>(cursor, hour, minute, second))) {
        PRINT(" invalid time ");
        return nullptr;
      }
    }
    if (!c || TIsWhitespace<Char>(c)) {
      PRINTF("\n    Format is MM/DD and year is %i", clock.year + ClockEpoch());
      // is_last_year = ((value1 >= std_tm.mon) &&
      //    (value2 >= std_tm.mday)) ? 0:1;
      clock.year += is_last_year;
      clock.month = value1 - 1;
      clock.day = value2;
      clock.hour = 0;
      clock.hour = hour;
      clock.minute = minute;
      clock.second = second;

      return cursor + 1;
    }
    c = TLowercase<Char>(c);
    if ((value1 < 12) && (value2 > 0) &&
        (value2 <= ClockMonthDayCount(value1))) {
      PRINT(" MM/DD ");
      if (value1 > 11) {
        PRINT("\nInvalid MM/DD@ month");
        return nullptr;
      }
      // We need to find out what year it is.
      CClock clock;
      ClockInit(clock);

      if (value2 > ClockMonthDayCount(clock.year - ClockEpoch(), value1)) {
        PRINT("\nInvalid MM/DD@ day");
        return nullptr;
      }
      clock.month = value1 - 1;
      clock.day = value2;
      clock.hour = hour;
      clock.minute = minute;
      clock.second = second;
      if (!(cursor = TStringScanTime(cursor, hour, minute, second))) {
        PRINT("\nInvalid MM/DD@");
        return nullptr;
      }

      return cursor + 1;
    }
    if ((value1 < 12) && (value2 > ClockMonthDayCount(value1))) {
      PRINT(" MM/YYyy");
      clock.month = value1 - 1;
      clock.year = value2;
      if (!(cursor = TStringScanTime<Char>(cursor, hour, minute, second))) {
        PRINT("\nInvalid MM / YYYY@ time");
        return nullptr;
      }

      return cursor + 1;
    }
    PRINT("\nInvalid MM/DD or MM/YYyy format");
    return nullptr;
  }

  // Formats MM/DD/YYyy and YYyy/MM/DD

  cursor = TStringSkipChar<Char>(++cursor, '0');
  c = *cursor;
  // Then there are 3 values and 2 delimiters.
  if (!TIsDigit<Char>(c) || !TScanSigned<int, Char>(cursor, value3)) {
    PRINTF("\n    SlotRead error reading value3 of date. %c: ", c);
    return nullptr;  //< Invalid format!
  }
  cursor = TStringDecimalEnd<Char>(cursor);
  PRINTF("%c%i", c, value3);
  // Now we need to check what format it is in.

  c = *cursor;
  if (c == '@') {
    if (!(stop = TStringScanTime<Char>(cursor, hour, minute, second))) {
      PRINT("Invalid YYyy/MM/DD@ time.");
      return nullptr;
    }
  }
  clock.hour = hour;
  clock.minute = minute;
  clock.second = second;
  if (TIsWhitespace<Char>(*(++cursor))) {
    PRINT("No date found.");
    return nullptr;
  }
  if (value1 > 11) {  //<
    PRINT("\n    Case YYyy/MM/DD");
    if (value2 == 0 || value2 > 12) {
      PRINTF("Invalid number of months");
      return nullptr;
    }

    if (value2 > ClockMonthDayCount(value2, value1)) {
      PRINT("Invalid number of days");
      return nullptr;
    }  // 17/05/06

    if (value1 < 100) {
      PRINT("\n    Case YY/MM/DD");
      value1 += 2000 - ClockEpoch();
    } else {
      PRINT("\n    Case YYYY/MM/DD");
      value1 -= ClockEpoch();
    }

    clock.year = value1;
    clock.month = value2 - 1;
    clock.day = value3;

    return cursor + 1;
  }
  PRINT("\n    Cases MM/DD/YY and MM/DD/YYYY");

  if (value1 > 11) {
    PRINT("\nInvalid month.");
    return nullptr;
  }
  if (value2 > ClockMonthDayCount(value1, value3)) {
    PRINT("\nInvalid day.");
    return nullptr;
  }
  clock.month = value1 - 1;
  clock.day = value2;
  if (value3 < 100) {
    PRINT("\n    Case MM/DD/YY");
    clock.year = value3 + (2000 - ClockEpoch());
  } else {
    PRINT("\n    Case MM/DD/YYYY");
    clock.year = value3 - ClockEpoch();
  }
  return cursor + 1;
}

/* Creates a timestamp from the given seconds Clock. */
template <typename SI = TMS>
SI TStampTime(CClock& clock) {
  SI t = (clock.year - kClockEpochInit) * kSecondsPerYear +
         (clock.day - 1) * kSecondsPerDay + clock.hour * kSecondsPerHour +
         clock.minute * kSecondsPerMinute + clock.second;
  return t;
}

template <typename Char, typename SI>
const Char* TStringScanTime(const Char* start, SI& result) {
  CClock clock;
  const Char* stop = TScan<Char>(start, clock);
  result = TStampTime<SI>(clock);
  return stop;
}

template <typename Char>
const Char* TStringScanTime(const Char* start, Tss& result) {
  start = TStringScanTime<Char, TMS>(start, result.seconds);
  if (!start) return nullptr;
  if (*start++ != ':') {
    result.ticks = 0;
    return start - 1;
  }
  return TScanUnsigned<UI4, Char>(start, result.ticks);
}
#endif  // #if USING_UTF

}  // namespace _

#endif  //< #ifndef INCLUDED_SCRIPTTCLOCK
#endif  //< #if SEAM >= _0_0_0__04