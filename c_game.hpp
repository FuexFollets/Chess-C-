#include "metadata.hpp"
#include "primitives.hpp"
#pragma once

class Cboard {
  uint8_t turn;
  grid Cboard_grid;
  C_move last_move;

  struct {
    bool white_king : 1;
    bool black_king : 1;
    bool white_queen : 1;
    bool black_queen : 1;
  } c_rights;

  piece& at_cord(const C_cord cval) {
    return Cboard_grid[cval.y][cval.x];
  }

  public:

  Cboard() : turn{} {}

  Cboard(const Cboard& board) : Cboard_grid{board.get_grid_ref()}, turn{board.get_turn()} {}
  
  grid get_grid() const { return Cboard_grid; }
  const grid& get_grid_ref() const { return Cboard_grid; }
  uint8_t get_turn() const { return turn; }
  const uint8_t& get_turn_ref() const { return turn; }

  piece& operator[](const C_cord pos) {
    return Cboard_grid[pos.y][pos.x];
  }

  void make_move(C_move);

  Cboard after_move(const C_move) const;

  std::unordered_set<C_move> square_moves(C_cord);

  std::unordered_set<C_move> av_moves() const;
  
  bool is_legal();

  friend Cboard from_pgn();
};


void Cboard::make_move(C_move move_val) {
  if (from_cord(Cboard_grid, last_move.end).type_ == 0) if (
    (difference(last_move.start.y, last_move.end.y) == 2) &&
    (last_move.end.y == move_val.start.y) &&
    (last_move.end.x == move_val.end.x)
  ) {
    Cboard_grid[last_move.end.y][last_move.end.x] = piece();
  }
  
  last_move = move_val;
  
  if (move_val.castletype) {
    switch (move_val.castletype) {
      case 1: 
        Cboard_grid[7][6] = piece(5, 1);
        Cboard_grid[7][5] = piece(3, 1);
      break;
      case 3:
        Cboard_grid[0][6] = piece(5, 0);
        Cboard_grid[0][5] = piece(3, 0);
      break;
      case 2:
        Cboard_grid[7][2] = piece(5, 1);
        Cboard_grid[7][3] = piece(3, 1);
      break;
      case 4:
        Cboard_grid[0][2] = piece(5, 0);
        Cboard_grid[0][3] = piece(3, 0);
      break;
    }

    switch (move_val.castletype) {
      case 1: case 2:
        Cboard_grid[7][move_val.rookstart] = piece();
        Cboard_grid[7][move_val.kingstart] = piece();
      break;
      case 3: case 4:
        Cboard_grid[0][move_val.rookstart] = piece();
        Cboard_grid[0][move_val.kingstart] = piece();
      break;
    }
    return;
  } 
  
  if (move_val.is_promotion) {
    from_cord(Cboard_grid, move_val.end) = piece{move_val.promotion_to.color_, move_val.promotion_to.type_};
    from_cord(Cboard_grid, move_val.end) = piece();
    return;
  }

  from_cord(Cboard_grid, move_val.end) = from_cord(Cboard_grid, move_val.start);
  from_cord(Cboard_grid, move_val.start) = piece();

  
}

Cboard Cboard::after_move(const C_move move_val) const {
  Cboard board_copy(*this);
  board_copy.make_move(move_val);
  return board_copy;
}

std::unordered_set<C_move> Cboard::square_moves(C_cord pos) {
  std::unordered_set<C_move> moves_list;
    
  piece at_pos{from_cord(Cboard_grid, pos)};
  if (at_pos.color_ == 3) { return moves_list; }
    
  switch (at_pos.type_) {
    case 0:
      for (int capture_side{-1}; capture_side < 3; capture_side += 2) {
        if (in_bounds(pos.x + capture_side)) {
          if ((int)(at_cord(C_cord{pos.x + capture_side, pos.y + at_pos.color_ * 2 - 1).color_})) {
            moves_list.insert(C_move(pos, C_cord{pos.x + capture_side, pos.y + at_pos.color_ * 2 - 1}));
          }
        }
      }

      if (at_cord(C_cord{pos.x, pos.y + at_pos.color_ * 2 - 1}) == 3) break;

      moves_list.insert(C_cord{pos.x, (int)pos.y + 1});

      if (((int)pos.y * (-6) - 7) == at_pos.color_) if (at_cord(C_cord{pos.x, pos.y + at_pos.color_ * 4 - 2}) == 3) {}

      if (at_cord(last_move.end).type_ == piece{})
      
    break;
    case 1:
      for (const auto&[xd, yd]: diff::knight) {
        if (at_cord(pos).color_ != at_pos.color_) {
          moves_list.insert(C_move(pos, C_cord{pos.x + xd, pos.y + yd}));
        }
      }
      
    break;
    case 2:
      for (const auto& dir: diff::bishop) [&] {
        for (const auto&[xd, yd]: dir) {
          C_cord new_pos{pos.x + xd, pos.y + yd};
          
          if (in_bounds(new_pos.x, new_pos.y)) {
            
            if (at_cord(new_pos).color_ == 3) {
              moves_list.insert(C_move(pos, new_pos));
            } else if (at_cord(new_pos).color_ != at_cord(pos).color_) {
              moves_list.insert(C_move(pos, new_pos));
              return;
            } else return;
          }
        }
      }();
    break;
    case 3:
      for (const auto& dir: diff::rook) [&] {
        for (const auto&[xd, yd]: dir) {
          C_cord new_pos{pos.x + xd, pos.y + yd};
          if (in_bounds(new_pos.x, new_pos.y)) {
            if (at_cord(new_pos).color_ == 3) {
              moves_list.insert(C_move(pos, new_pos));
            } 
            else if (at_cord(new_pos).color_ != at_cord(pos).color_) {
              moves_list.insert(C_move(pos, new_pos));
              return;
            } else return;
          }
        }
      }();

    break;
    case 4:
      for (const auto& dir: diff::queen) [&] {
        for (const auto&[xd, yd]: dir) {
          C_cord new_pos{pos.x + xd, pos.y + yd};
          if (in_bounds(new_pos.x, new_pos.y)) {
            if (at_cord(new_pos).color_ == 3) {
              moves_list.insert(C_move(pos, new_pos));
            } 
            else if (at_cord(new_pos).color_ != at_cord(pos).color_) {
              moves_list.insert(C_move(pos, new_pos));
              return;
            } else return;
          }
        }
      }();
    break;
    case 5:
      for (const auto&[xd, yd]: diff::king) {
        if (at_cord(pos).color_ != at_pos.color_) {
          moves_list.insert(C_move(pos, C_cord{pos.x + xd, pos.y + yd}));
        }
      }
    break;
  }
  return moves_list;
}
