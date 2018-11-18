/* Script^2 @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /room.h
@author  Cale McCollough <cale.mccollough@gmail.com>
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
#if SEAM >= _0_0_0__13
#ifndef INCLUDED_CRABS_ROOM
#define INCLUDED_CRABS_ROOM
#include "interrupts.h"
#include "twall.h"

namespace _ {

/* A list of Requests that can be sent from Slot<UIT, TSize> to
    Slot<UIT, TSize>.  */
typedef enum Requests {
  OpenDoorRequest = 0,
  CloseDoorRequest,
  ConnectionRequest,
  DisconnectRequest,
  InvalidRequest,
} Request;

enum {
  NumRequests = 2,  //< The number of Requests.
};

/* Returns an array of pointers to strings that describe the program states.
 */
API const char** RoomStateTexts();

/* Returns a pointer to an array of pointers to the Request strings. */
API const char** RequestTexts();

/* Gets the response char corresponding to the given request. */
API const char* RequestText(Request r);

/* A Chinese Room.
An Chinese Room works the same way as in the Chinese Room thought
experiment. An Room receives a message through a slot in the door, the man
in the room reads does manual optical character recognition with a pen and
paper stack, some filing cabinets, and a library of multimaps.
  The size of the CCrabs Stack is defined by the Script Protocol to be a
a maximum of 2^15-1 expressions tall.
# Memory Layout
Some systems have the stack grow up from the bottom and heap grow up and
vice versa.
@code
    +-----------------------+
    |          Heap         |
    |         Wall 1        |
    |         Wall 2        |
    |          ...          |
    |         Wall N        |
    |vvvvvvvvvvvvvvvvvvvvvvv|
    |     Unused Memory     |
    |^^^^^^^^^^^^^^^^^^^^^^^|
    |     Program Stack     |
+/- |-----------------------|
  |  | Floor (Static Memory) |
0xN +-----------------------+
@endcode

# ASCII Mappings

The mapping of the layout of the Chinese Room can be seen in the following
super cute ASCII house is:

@code
 _________
/_________\
[_#_]@[_#_]
@endcode


# Room Connections

There are multiple doors in a Chinese Room that lead to other Chinese Rooms.
Each door has multiple slots in lead to the same room. These slots are the
various IO ports of the system.

# Doors

There are two ways to access Doors in the CR. in the Root Scope, there are:

@code
Use Case Scenario:
Actors: Jo  - The app developer.
        Sam - The stop-user.
1.  Jo is creating an App and needs to make a Chinese Room so Jo downloads
    Script, starts his app, and makes a Room. Jo defines a Floor
    statically but is does not define any walls.
    2.  Host creates a memory stack without any heap space.
2.  Jo needs to add a UART port out to his App so he adds a BOut with a
    slot_size of 2KB.
    3.  Host adds the Slot to the Program Stack.
4.  Jo needs to add a SPI IO device so he initializes a Slot with 256
    bytes.
    5.  Host adds the new Slot on top of the Slot.
6.  Jo is out of memory in the Floor so he creates a Ceiling of size 2KB.
    7.  Host creates a Heap Block for the Ceiling.
7.  Jo needs Interprocess communication to three threads: one SlotIn,
    one SlotOut, and a Slot of size 2KB.
    8.  Host adds the SlotIn, MirroOut, and Slot to the Ceiling.
9.  Jo wants to add a Server so Jo creates Wall_1 with 1MB space.
    10. Host creates a Wall_1 with 1MB memory.
11. Jo wants needs to distribute information to the stop-users so Jo
    creates Dictionary in Wall_1 with some keys and values.
    12. Host creates dictionary and adds keys and values.
13. Jo complies the program and launches the server.
    14. Host loads.
15. Sam launches Jo's app and connections to the Host.
    16. Host receives login attempt from Sam and opens a Door for him.
17. Sam needs to get the values Jo stored in step 11 so Same sends a
    Dictionary GET request.
    @endcode
*/
class Room : public Operand {
  // NONCOPYABLE (Room) //< Not sure why this isn't working?

 public:
  typedef enum States {
    kStateBooting = 0,
    kStateGoingToSleep,
    kShutdown,
  } State;

  enum {
#ifndef CRABS_MAX_WALLS
    kRoomFloorSize = 1024,
#else
    kRoomFloorSize = CRABS_MAX_WALLS,
#undef ROOM_FLOOR_SIZE
#endif
    kFloorSizeWords = kRoomFloorSize / sizeof(UIW) + 2,  //< +2 socket.
  };

  /* Creates a Room with the given size.
      @param floor Buffer used to create the Wall Stack. Set to nullptr to
                   enable dynamic memory.
      @param size  The room size that is bounded between the kMinRoomSize and
                   kMaxRoomSize. */
  Room(const char* room_name = "chinese_room", int num_states = 2);

  /* AsciiFactory. */
  virtual ~Room();

  /* Gets the Room state_. */
  int GetState();

  /* Gets the Room state_. */
  int GetStateCount();

  const char* GetRoomName();

  /* Sets the Room state_. */
  virtual BOL SetRoomName(const char* name);

  /* Processes a request from another Room.
      @return Returns false upon success and true if there is an error. */
  Request HandleNextRequest(Request r);

  /* Clears the log. */
  void ClearLog();

  /* Processes all of the errors in the error log. */
  void ProcessLog();

  /* Prints the error log to a expression. */
  void PrintErrors(BOut* bout);

  int_t WallCount();

  Wall* GetWall(int_t wall_number);

  Wall* AddWall(Wall* new_wall);

  BOL RemoveWall(int_t wall_number);

  UIW GetSizeBytes();

  /* Function run every main loop cycle to check the system status. */
  virtual void DiagnoseProblems();

  /* Sets up the Room. */
  virtual const Op* Init(CCrabs* crabs);

  /* Handler for shut down event. */
  virtual void ShutDown();

  /* Handler go to sleep event. */
  virtual void Sleep();

  /* Handler for wake from sleep event. */
  virtual void Wake();

  /* Handler for recovering from an external crash message. */
  virtual void Crash();

  /* Main program loop. */
  virtual const Op* Loop();

  /* Returns true if the Room is on. */
  virtual BOL IsOn();

  /* The default main function.
      Please feel free to override this with your own main function. */
  virtual int Main(const char** args, int args_count);

  /* Handles Script Commands.
      @param text     Beginning of the Text socket.
      @param text_end End of the Text socket.
      @return Returns nil upon success and an error string upon failure. */
  virtual char CommandNext();

  /* Script expressions. */
  virtual const Op* Star(CHW index, CCrabs* crabs);

#if CRABS_TEXT
  /* Prints the Room to the stdout. */
  virtual UTF1& Print(UTF1& utf);
#endif

 protected:
  int state_count_,                    //< Number of FSM states.
      state_;                          //< Room state.
  const char* name_;                   //< Room Name.
  TCArray<Wall*, UIT, int_t>* walls_;  //< Walls in the Room.
  CCrabs* expr_;                       //< Current CCrabs being executed.
                                       //< DC1: this.
  Door* this_;                         //< DC2: The Door to this room.
  Operand *xoff_,                      //< DC3: XOFF - XOFF handling device.
      *device_,                        //< DC4: the current device control.
      *devices_;               //< Pointer to the current device control.
  UIW start[kFloorSizeWords];  //< Room Floor socket.

 private:
  /* Sets the Room state_. */
  BOL SetState(int new_state);
};

/* Returns the Room-Level Script. */
// API Room* ChineseRoom (Room* room = nullptr);

}  // namespace _
#endif  //< CRABS_ROOM_HDi
#endif  //< #if SEAM >= _0_0_0__13