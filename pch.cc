// pch.cpp: source file corresponding to pre-compiled header; necessary for
// compilation to succeed

#include "pch.h"

#if SCRIPT2_RUN_TESTS == YES_0

#include "t_test.h"

#if SEAM >= SCRIPT2_UNIPRINTER
#include "00_uniprinter.h"
#endif
#if SEAM >= SCRIPT2_RNG
#include "01_rng.h"
#endif
#if SEAM >= SCRIPT2_ITOS
#include "02_itos.h"
#endif
#if SEAM >= SCRIPT2_FTOS
#include "03_ftos.h"
#endif
#if SEAM >= SCRIPT2_SPRINTER
#include "04_sprinter.h"
#endif
#if SEAM >= SCRIPT2_CLOCK
#include "05_clock.h"
#endif
#if SEAM >= SCRIPT2_STACK
#include "06_stack.h"
#endif
#if SEAM >= SCRIPT2_MATRIX
#include "07_matrix.h"
#endif
#if SEAM >= SCRIPT2_MAP
#include "08_map.h"
#endif
#if SEAM >= SCRIPT2_STRAND
#include "09_strand.h"
#endif
#if SEAM >= SCRIPT2_LOOM
#include "10_loom.h"
#endif
#if SEAM >= SCRIPT2_TABLE
#include "11_table.h"
#endif
#if SEAM >= SCRIPT2_LIST
#include "12_list.h"
#endif
#if SEAM >= SCRIPT2_BOOK
#include "13_book.h"
#endif
#if SEAM >= SCRIPT2_DICTIONARY
#include "14_dictionary.h"
#endif
#if SEAM >= SCRIPT2_EXPR
#include "15_expr.h"
#endif
#if SEAM >= SCRIPT2_DOOR
#include "16_door.h"
#endif
#if SEAM >= SCRIPT2_ROOM
#include "17_room.h"
#endif

using namespace _;

namespace script2 {
static const CH1* TestNode(const CH1* args) {
  return TTestTree<_00_Uniprinter
#if SEAM >= SCRIPT2_RNG
                   ,
                   _01_RNG
#endif
#if SEAM >= SCRIPT2_ITOS
                   ,
                   _02_ItoS
#endif
#if SEAM >= SCRIPT2_FTOS
                   ,
                   _04_FtoS
#endif
#if SEAM >= SCRIPT2_SPRINTER
                   ,
                   _04_SPrinter
#endif
#if SEAM >= SCRIPT2_CLOCK
                   ,
                   _05_Clock
#endif
#if SEAM >= SCRIPT2_STACK
                   ,
                   _06_Stack
#endif
#if SEAM >= SCRIPT2_MATRIX
                   ,
                   _07_Matrix
#endif
#if SEAM >= SCRIPT2_MAP
                   ,
                   _08_Map
#endif
#if SEAM >= SCRIPT2_STRAND
                   ,
                   _09_Strand
#endif
#if SEAM >= SCRIPT2_LOOM
                   ,
                   _10_Loom
#endif
#if SEAM >= SCRIPT2_TABLE
                   ,
                   _11_Table
#endif
#if SEAM >= SCRIPT2_LIST
                   ,
                   _12_List
#endif
#if SEAM >= SCRIPT2_BOOK
                   ,
                   _13_Book
#endif
#if SEAM >= SCRIPT2_DICTIONARY
                   ,
                   _14_Dictionary
#endif
#if SEAM >= SCRIPT2_EXPR
                   ,
                   _15_Expr
#endif
#if SEAM >= SCRIPT2_DOOR
                   ,
                   _16_Door
#endif
#if SEAM >= SCRIPT2_ROOM
                   ,
                   _17_Room_Wall
#endif
                   >(args);
}
}  // namespace script2

SIN main(SIN arg_count, CH1** args) {
  return _::TTestTree<script2::TestNode>(arg_count, args);
}
#endif
