#include <bits/stdc++.h>
#pragma once

struct piece { // 1 byte
  uint8_t color_ : 2;
  uint8_t type_ : 6;

  piece(const uint8_t type, const uint8_t color) : color_{color}, type_{type} {}

  piece() : color_{3} {}
};

struct C_cord { // 1 byte
  uint8_t x : 4;
  uint8_t y : 4;

  C_cord(const uint8_t xval = 0, const uint8_t yval = 0) : x{xval}, y{yval} {}

  bool operator==(const C_cord cord) const {
    return (cord.x == x) && (cord.y == y);
  }
  bool operator!=(const C_cord cord) const {
    return !(*this == cord);
  }
};

struct C_move { // 4 bytes
  C_cord start; // 1 byte 8 bits
  C_cord end; // 1 byte 8 bits

  uint8_t castletype : 3;
  // 0 no castle
  // 1 kingside w
  // 2 queenside w
  // 3 kingside b
  // 4 queenside b

  uint8_t kingstart : 3;
  uint8_t rookstart : 3;
  
  bool is_promotion : 1;
  struct {
    uint8_t color_ : 2;
    uint8_t type_ : 4;
  } promotion_to; // 6 bits
  // 0 type_ = none

  bool operator==(const C_move& mov) const {
    return ((*(uint32_t *) this) == *(uint32_t *) &mov);
  }

  bool operator!=(const C_move& mov) const {
    return !(*this == mov);
  }

  C_move() : start{}, end{}, castletype{}, promotion_to{}, is_promotion{} {} // defualt constructor

  C_move(const C_cord startval, const C_cord endval) : start{startval}, end{endval} {} // normal move

  C_move(const C_cord startval, const C_cord endval, const piece promotion) : start{startval}, end{endval}, promotion_to{promotion.color_, promotion.type_}, is_promotion{true} {} // promotion

  C_move(const uint8_t castle_way, const uint8_t king_pos = 4, const uint8_t rook_pos = 9) : castletype{castle_way}, kingstart{king_pos}, rookstart{(rook_pos == 9) ? (((castle_way == 1) || (castle_way == 3)) ? 7 : 0) : rook_pos} {} // castle
};

struct C_move_union_beta {
  uint8_t type : 2;

  
  
};

struct C_diff {
  int8_t xd : 4;
  int8_t yd : 4;
};

namespace std {
  template <> struct hash <piece> {
    size_t operator()(const piece& piece_val) const {
      return hash<uint8_t>()(*(uint8_t *) &piece_val);
    }
  };

  template <> struct hash <C_cord> {
    size_t operator()(const C_cord& cord) const {
      return hash<uint8_t>()(*(uint8_t *) &cord);
    }
  };

  template <> struct hash <C_move> {
    size_t operator()(const C_move& move) const {
      return hash<uint32_t>()(*(uint32_t *) &move);
    }
  };
}

typedef std::array<std::array<piece, 8>, 8> grid;

piece& from_cord(grid& g_obj, const C_cord cord) {
  return g_obj[cord.y][cord.x];
}


template <typename T1, typename T2>
auto difference(const T1 v1, const T2 v2) -> decltype(
  (v1 > v2) ? (v1 - v2) : (v2 - v1)
) {
  return (v1 > v2) ? (v1 - v2) : (v2 - v1);
}

template <typename T> bool in_bounds(const T x, const T y = 1) {
  return (
    (static_cast<int8_t>(x) < 8) &&
    (static_cast<int8_t>(x) > -1) &&
    (static_cast<int8_t>(y) < 8) &&
    (static_cast<int8_t>(y) > -1)
    );
}


