/* Script @version 0.x
@link    https://github.com/kabuki-starship/script.git
@file    /script2/t_stack.h
@author  Cale McCollough <cale@astartup.net>
@license Copyright (C) 2014-2018 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= SCRIPT2_5
#ifndef INCLUDED_SCRIPTTSTACK
#define INCLUDED_SCRIPTTSTACK 1

#include "c_socket.h"
#include "c_utf8.h"
#include "t_object.h"

#if SEAM == SCRIPT2_5
#include "global_debug.inl"
#else
#include "global_release.inl"
#endif

namespace _ {

/* @ingroup Stack
Stack is an ASCII Data Type designed to use a C-style templated struct in C++
using no dynamic memory and with dynamic memory as a templated C++ warper class
and cross-language bindings and deterministic CPU cache optimizations. */

/* Returns the maximum value of the given signed type. */
constexpr uint32_t NaNSignedUI4();

/* Returns the maximum value of the given unsigned type. */
constexpr uint32_t UnsignedMaxUI4();

/* An array of 8, 16, 32, or 64 bit plain-old-data (POD) types.

An array may use two different memory layouts, one for a 1D stack of a
given types, and another for a multi-dimensional array that uses the 1D
array in order to store the dimensions. The only different between them is
that the size_array variable gets set to 0.

# 64-Bit Alignment

All ASCII Data Types are 64-bit aligned so stack sizes are:

# Stack Memory Layout

@code
    +----------------+
    |  Packed Stack  |  <-- Only if header_size = 0
    |----------------|
    |     Buffer     |
    |----------------|
    | Stack Elements |
 ^  |----------------|
 |  |  CStack struct |
0xN +----------------+
@endcode

# Multi-dimensional Array Memory Layout

@code
    +-----------------+
    |  C-Style Array  |  <-- Only if header_size > 0
    |-----------------|
    | Dimension Stack |
 ^  |-----------------|
 |  |  CStack struct  |
0xN +-----------------+
@endcode
*/
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
struct CStack {
  UI size_array,   //< Used for multi-dimensional array.
      size_stack;  //< Total size of the Stack in 64-bit aligned bytes.
  SI count_max,    //< Max element count.
      count;       //< Element count.
};

/* Gets the size of a Stack with the given count_max. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
inline UI StackSize(SI count_max) {
  enum { kCountMaxMin = sizeof(uint64_t) / sizeof(T) };
  if (count_max < kCountMaxMin) count_max = kCountMaxMin;
  return sizeof(CStack<T, UI, SI>) + sizeof(T) * count_max;
}

/* Gets the min size of a Stack. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
inline UI StackSizeMin() {
  enum {
    kStackCountMin = sizeof(T) > 8 ? 1 : 8 / sizeof(T),
    kStackCountMaxMin = sizeof(CStack<T, UI, SI>) + sizeof(T) * kStackCountMin,
  };
  return kStackCountMaxMin;
}

/*
template<typename T = intptr_t, typename UI = uint, typename SI = SI4>
inline UI StackSize (SI count) {
    UI size = sizeof (CStack<T, UI, SI>) + (sizeof (T) * 8) * count;
    return MemoryAlign8<UI> (size);
}*/

/* Gets the max number_ of elements in an stack with the specific index
width. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
inline SI StackCountMax() {
  return (SI)((((~(UI)0) - 7) - (UI)sizeof(CStack<T, UI, SI>)) / (UI)sizeof(T));
}

/*
template<typename T = intptr_t, typename UI = uint, typename SI = SI4>
SI StackCountMax () {
    return (SI)((UnsignedMax<UI> () - (UI)sizeof (CStack<T, UI, SI>)) /
        (UI)sizeof (T));
}*/

/* The minimum stack size. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
inline UI StackSizeMin(SI count_max) {
  SI count_upper_bounds = StackCountMax<T, UI, SI>();
  if (count_max > count_upper_bounds) count_max = count_upper_bounds;
  return (UI)(sizeof(CStack<T, UI, SI>) + count_max * sizeof(T));
}

/* Rounds up the count to the 64-bit align the value. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
inline UI StackCountMax(SI count_max) {
  enum {
    kStackCountMax =
        (UnsignedMax<UI>() - (UI)sizeof(CStack<T, UI, SI>)) / (UI)sizeof(T),
  };
  count_max = TAlignUpSigned<SI, UI, SI>(count_max);
  if (count_max > kStackCountMax) count_max = kStackCountMax;
  return count_max;
}

/* Initializes an stack from a preallocated socket who's size is a multiple of
8 bytes.
@param socket An stack of bytes large enough to fit the stack.
@return A dynamically allocated socket. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
uintptr_t* StackInit(uintptr_t* socket, UI size) {
  ASSERT(socket);

  CStack<T, UI, SI>* stack = reinterpret_cast<CStack<T, UI, SI>*>(socket);
  stack->size_array = 0;
  stack->size_stack = size;
  SI count_max = (SI)((size - sizeof(CStack<T, UI, SI>)) >> kWordBitCount);
  stack->count_max = count_max;
  stack->count = 0;
  return socket;
}

/* Initializes an stack from a preallocated socket who's size is a multiple of
8 bytes.
@param socket An stack of bytes large enough to fit the stack.
@return A dynamically allocated socket. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
uintptr_t* StackInit(uintptr_t* socket, UI size, SI count_max) {
  ASSERT(socket);

  CStack<T, UI, SI>* stack = reinterpret_cast<CStack<T, UI, SI>*>(socket);
  stack->size_array = 0;
  stack->size_stack = size;
  stack->count_max = (size - sizeof(CStack<T, UI, SI>)) >> kWordBitCount;
  stack->count = 0;
  return socket;
}

template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
uintptr_t* StackClone(CStack<T, UI, SI>* stack) {
  ASSERT(stack);
  UI size = stack->size_stack >> kWordBitCount;
  uintptr_t other_buffer = new uintptr_t[size];
  uintptr_t *source = reinterpret_cast<uintptr_t*>(stack),
            *destination = other_buffer;
  UI data_amount =
      (stack->count * sizeof(T) + sizeof(CStack<T, UI, SI>)) >> kWordBitCount;
  size -= data_amount;
  while (data_amount-- > 0) *destination++ = *source++;
  return destination;
}

/* Clones the given stack. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
uintptr_t* StackClone(CStack<T, UI, SI>* stack, CStack<T, UI, SI>* other) {
  ASSERT(stack);
  ASSERT(other);

  UI this_size = stack->size_stack, other_size = other->size_stack;

  if (this_size < other_size) return StackClone(other);

  // We've got enough room in the stack's memory.

  uintptr_t *read = reinterpret_cast<uintptr_t*>(TStackStart(stack)),
            *write = reinterpret_cast<uintptr_t*>(TStackStart(other));

  SI count = other->count;
  stack->count = count;
  uintptr_t* write_end = write + ((count * sizeof(T)) >> kWordBitCount);
  while (write < write_end) *write++ = *read++;
  return reinterpret_cast<uintptr_t*>(stack);
}

/* Returns the first element in the Stack TArray. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
T* TStackStart(CStack<T, UI, SI>* stack) {
  ASSERT(stack);
  return reinterpret_cast<T*>(reinterpret_cast<CH1*>(stack) +
                              sizeof(CStack<T, UI, SI>));
}

/* Returns the first element in the Stack TArray. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
T* StackStop(CStack<T, UI, SI>* stack) {
  ASSERT(stack);
  return TStackStart<T, UI, SI>(stack) + stack->count - 1;
}

/* Inserts the item into the stack at the given index.
@warning Function does not check for bounds and pushing to the top
if the Stack (@see SI StackInsert<UI, SI> (T*, SI, T, SI)).
@param items Pointer to element 0 of the array.
@param item  The item to insert.
@param index The index to insert at.
@return -1 if a is nil and -2 if the stack is full. */
template <typename T = intptr_t, typename SI = SI4>
inline SI StackInsert(T* items, SI count, T item, SI index) {
  T *target = items + index, *stop = items + count;
  // Shift the elements up.
  while (target < stop) *stop-- = *stop;
  *target = item;
  return count + 1;
}

/* Inserts the item into the stack at the given index.
@param items Pointer to element 0 of the array.
@param item  The item to insert.
@param index The index to insert at.
@return -1 if a is nil and -2 if the stack is full. */
template <typename T = intptr_t, typename SI = SI4>
inline SI StackAdd(T* items, SI count, T item, SI index) {
  ASSERT(items);
  if (index < 0 || index > count) return -1;
  if (index == count) {
    items[count] = item;
    return count + 1;
  }
  if (count == 1) {
    items[1] = items[0];
    items[0] = item;
    return 1;
  }
  return StackInsert<T, SI>(T, count, item, index);
}

/* Inserts the item into the stack at the given index.
@param a     The stack.
@param item  The item to insert.
@param index The index to insert at.
@return -1 if a is nil and -2 if the stack is full.
template<typename T = intptr_t, typename UI = uint, typename SI = SI4>
T StackAdd (CStack<T, UI, SI>* stack, T item, T index) {
  ASSERT (stack);
  SI count_max  = stack->count_max,
      count = stack->count;
  if (count >= count_max)
      return -1;
  T* items = TStackStart<T, UI, SI> (This ());
  return StackAdd<T, SI> (items, count, item, index);
} */

/* Removes an element from the given array. */
template <typename T = intptr_t, typename SI = intptr_t>
inline SI TStackRemove(T* elements, SI size, SI index) {
  ASSERT(elements);
  if (index < 0) return index;
  if (size < 0) return size;
  if (size == 1) return 1;
  if (index >= size) return index * -1;
  elements += index;
  T* elements_end = elements + --size;
  while (elements <= elements_end) *elements++ = *elements;
  return size;
}

/* Removes the given index from the stack.
@param  a     The stack.
@param  index The index the item to remove.
@return True if the index is out of bounds. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
SI TStackRemove(CStack<T, UI, SI>* stack, SI index) {
  ASSERT(stack);
  SI result =
      TStackRemove<T, SI>(TStackStart<T, UI, SI>(stack), stack->count, index);
  if (result < 0) return result;
  stack->count = result;
  return result;
}

/* Adds the given item to the stop of the stack.
@param  a    The stack.
@param  item The item to push onto the stack.
@return The index of the newly stacked item. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
SI TStackPush(CStack<T, UI, SI>* stack, T item) {
  ASSERT(stack);
  SI count_max = stack->count_max, count = stack->count;
  if (count >= count_max) return -1;
  T* items = TStackStart<T, UI, SI>(stack);
  items[count] = item;
  stack->count = count + 1;
  return count;
}

/* Pops the top item off of the stack.
@note We do not delete the item at the
@param  a The stack.
@return The item popped off the stack. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
T TStackPop(CStack<T, UI, SI>* stack) {
  ASSERT(stack);
  SI count = stack->count;
  if (count == 0) return 0;
  T* items = TStackStart<T, UI, SI>(stack);
  stack->count = count - 1;
  T item = items[count - 1];
  return item;
}

/* Pops the top item off of the stack.
@note We do not delete the item at the
@param  a The stack.
@return The item popped off the stack. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
T TStackPeek(CStack<T, UI, SI>* stack) {
  ASSERT(stack);
  SI count = stack->count;
  if (count == 0) return 0;
  T* items = TStackStart<T, UI, SI>(stack);
  T item = items[stack->count - 1];
  return item;
}

/* Gets the element at the given index.
@param  stack    The stack.
@param  index The index of the element to get.
@return -1 if a is nil and -2 if the index is out of bounds. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
T TStackGet(CStack<T, UI, SI>* stack, SI index) {
  ASSERT(stack);
  if (index >= stack->count) return 0;
  CH1* address = reinterpret_cast<CH1*>(stack) + sizeof(CStack<T, UI, SI>);
  return reinterpret_cast<T*>(address)[index];
}

/* Returns true if the given stack contains the given address.
@return false upon failure. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
BOL TStackContains(CStack<T, UI, SI>* stack, void* address) {
  ASSERT(stack);
  CH1 *ptr = reinterpret_cast<CH1*>(stack),
       *adr = reinterpret_cast<CH1*>(address);
  if (adr < ptr) return false;
  if (adr >= ptr + stack->size_array) return false;
  return true;
}

/* The stack size in words. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
inline UI TStackSizeWords(SI count) {
  return StackSizeMin<T, UI, SI>(count) / sizeof(uintptr_t);
}

/* The upper bounds defines exactly how many elements can fit into a space
in memory.
@warning Anything above this threshold may cause a critical error; AND
sizeof (T) must be 1, 2, 4, or 8. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
inline SI TStackCountUpperBounds() {
  enum {
    kShift = (sizeof(T) == 8) ? 3 :             //< Used to divide by 8.
                 (sizeof(T) == 4) ? 2 :         //< Used to divide by 4.
                     (sizeof(T) == 2) ? 1 : 0,  //< Used to divide by 2.
  };
  return (SI)((((~(UI)0) - 7) - (UI)sizeof(CStack<T, UI, SI>)) / (UI)sizeof(T));
}

/* Doubles the size of the array until the max count is reached.
@return Returns nil if the count_max is greater than the amount of memory that
can fit in type UI, the unaltered socket pointer if the Stack has grown to the
count_max upper bounds, or a new dynamically allocated socket upon failure. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
BOL TStackGrow(CObject& stack) {
  static SI count_max_auto_size_init = kStackCountMaxDefault;

  uintptr_t* socket = stack.socket;

  ASSERT(socket);

  CStack<T, UI, SI>* stack = reinterpret_cast<CStack<T, UI, SI>*>(socket);
  SI count_max = stack->count_max,
     count_uppoer_bounds = TStackCountUpperBounds<T, UI, SI>();
  if (count_max > count_uppoer_bounds || count_max >= count_uppoer_bounds)
    return false;
  count_max += count_max;
  if (count_max > count_uppoer_bounds) count_max = count_uppoer_bounds;
  UI new_size = sizeof(CStack<T, UI, SI>) + count_max * sizeof(T), word_count;
  if ((new_size & 0x7) == 0)
    word_count = new_size >> kWordBitCount;
  else
    word_count = (new_size >> kWordBitCount) + 1;
  uintptr_t* new_buffer = new uintptr_t[word_count];
  CStack<T, UI, SI>* new_stack =
      reinterpret_cast<CStack<T, UI, SI>*>(new_buffer);
  new_stack->size_array = 0;
  new_stack->size_stack = new_size;
  SI count = stack->count;
  new_stack->count = count;
  new_stack->count_max = count_max;

  T *source = TStackStart(stack), *destination = TStackStart(new_stack);
  for (; count > 0; count--) *destination++ = *source++;
  delete socket;
  return true;
}

/* Prints the given stack to the console. */
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
UTF8& TPrintStack(UTF8& utf, CStack<T, UI, SI>* stack) {
  ASSERT(stack);
  UI size_array = stack->size_array;
  SI count = stack->count;
  if (size_array != 0) return utf << "\n\nStack: count: Invalid size_array!";
  utf << "\n\nStack: count: " << count << " count_max:" << stack->count_max
      << " size_stack:" << stack->size_stack;
  if (stack->size_array != 0) utf << " size_array:invalid";
  T* elements = TStackStart(stack);
  for (SI4 i = 0; i < count; ++i) {
    utf << '\n' << i + 1 << ".) " << elements[i];
  }
  return utf;
}

/* A stack of data.

This is a wrapper class for the

# Stack Memory Map

@code
+----------------+
|  Packed Stack  |  <-- Only if header_size = 0
|----------------|
| 64-bit Aligned |
|     Buffer     |
|----------------|
| Stack Elements |
|----------------|  +
|  Stack struct  |  |
+----------------+ 0x0
@endcode
*/
template <typename T = intptr_t, typename UI = uint, typename SI = SI4>
class TStack {
 public:
  /* Initializes an stack of n elements of the given type.
  @param count_max The max number_ of elements that can fit in memory in this
  Stack. */
  TStack(SI count_max = 0) {
    // Align the count_max to a 64-bit word boundary
    if (count_max == 0) {
      count_max = 32;
    }
    UI size = StackSize<T, UI, SI>(count_max);
    uintptr_t* socket = new uintptr_t[size >> kWordBitCount];
    obj_->begin = socket;
    StackInit(socket, size, count_max);
  }

  /* Copy constructor. */
  TStack(const TStack& other) : obj_(other.CObject()) {}

  /* Deallocates the buffer_. */
  ~TStack() {}

  TStack<T, UI, SI>& Clone() { StackClone<T, UI, SI>(CObject()); }

  TStack<T, UI, SI>& Clone(TStack<T, UI, SI>& other) {
    StackClone<T, UI, SI>(CObject(), *other);
  }

  /* Gets the max number_ of elements in an stack with the specific index
  width. */
  inline SI GetElementsMax() { return StackCountMax<T, UI, SI>(); }

  /* Gets the size of the entire Stack, including header, in bytes. */
  inline UI Size() { return CObject()->size; }

  /* Gets the min size of the entire Stack, including header, in bytes. */
  inline UI SizeMin() { return StackSizeMin<T, UI, SI>(); }

  /* Gets a pointer to the first element in the stack. */
  inline T* Elements() { return StackBegin<T, UI, SI>(CObject()); }

  /* Inserts the item into the stack at the given index.
  @param item  The item to insert.
  @param index The index to insert at.
  @return -1 if a is nil and -2 if the stack is full. */
  T Insert(T item, T index) {
    return TStack<T, UI, SI>(CObject(), item, index);
  }

  /* Removes the given index from the stack.
  @param  index The index the item to remove.
  @return True if the index is out of bounds. */
  BOL Remove(SI index) { return TStackRemove<T, UI, SI>(CObject(), index); }

  /* Adds the given item to the stop of the stack.
  @param  item The item to push onto the stack.
  @return The index of the newly stacked item. */
  SI Push(T item) {
    SI result = TStackPush<T, UI, SI>(CObject(), item);
    // std::count << "\n  Pushing " << item;
    if (result < 0) {
      // Printf (" and growing.");
      Grow();
      SI result = TStackPush<T, UI, SI>(CObject(), item);
      // COUT << this;
      if (result < 0) return -1;
      return result;
    }
    return result;
  }

  /* Pops the top item off of the stack.
  @note We do not delete the item at the
  @param  a The stack.
  @return The item popped off the stack. */
  inline T Pop() {
    T value = TStackPop<T, UI, SI>(CObject());
    // utf << "\n  Popping " << value;
    return value;
  }

  /* Pops the top item off of the stack.
  @note We do not delete the item at the
  @param  a The stack.
  @return The item popped off the stack. */
  inline T Peek() { return TStackPeek<T, UI, SI>(CObject()); }

  /* Gets the element at the given index.
  @param  index The index of the element to get.
  @return -1 if a is nil and -2 if the index is out of bounds. */
  inline T Get(SI index) { return TStackGet<T, UI, SI>(CObject(), index); }

  /* Returns true if the given stack contains the given address.
  @return false upon failure. */
  inline BOL Contains(void* address) {
    return TStackContains<T, UI, SI>(CObject(), address);
  }

  /* Resizes the stack to the new_count. */
  inline BOL Resize(SI new_count) { return TStackResize<T, UI, SI>(obj_); }

  /* Doubles the size of the stack. */
  inline BOL Grow() { return TStackGrow<T, UI, SI>(obj_); }

  /* Prints this object to the given UTF. */
  inline UTF8& Print(UTF8& out_) { return Print(CObject(), out_); }

  /* Returns the CStack CObject. */
  inline CStack<T, UI, SI>* CObject() {
    return reinterpret_cast<CStack<T, UI, SI>*>(base);
  }

  inline TStack<T, UI, SI>& operator=(const TStack<T, UI, SI>& other) {
    Clone(other);
    return *this;
  }

 private:
  TObject<SI> obj_;  //< ASCII Object.

  inline void SetBuffer(CStack<T, UI, SI>* stack) {
    ASSERT(stack);
    obj_->begin = reinterpret_cast<uintptr_t*>(stack);
  }
};

}  // namespace _

#include "global_footer.inl"
#endif  //< INCLUDED_SCRIPTTSTACK
#endif  //< #if SEAM >= SCRIPT2_5