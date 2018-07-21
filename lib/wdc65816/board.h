#ifndef WDC65816_BOARD_H
#define WDC65816_BOARD_H

#include "base/base.h"
#include "mapper.h"

typedef struct {
    uint maps_length;
    Wdc65816MapProperties map_properties[8];
} Wdc65816Memory;

typedef struct {
    String name;
    uint maps_length;
    Wdc65816MapProperties map_properties[256];
} Wdc65816Board;

extern Wdc65816Board board_shvc_1a1b_06;

void wdc65816_load_board(Wdc65816Board* board, String board_name);

#endif //WDC65816_BOARD_H
