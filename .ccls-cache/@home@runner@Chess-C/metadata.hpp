#include "primitives.hpp"
#pragma once

/*
type:
  0 pawn
  1 knight
  2 bishop
  3 rook
  4 queen
  5 king

color:
  0 white
  1 black
*/

namespace diff {
  const std::array<C_diff, 8> knight{[]{
    std::array<C_diff, 8> nm;
    const std::array<int8_t, 4> dv{-2, -1, 1, 2};
    uint8_t app_ind{};
    for (auto& xd: dv) for (auto& yd: dv) {
      if (xd*xd != yd*yd) {
        nm[app_ind++] = C_diff{xd, yd};
      }
    }
    return nm;
  }()};

  const std::array<C_diff, 8> king{[]{
    std::array<C_diff, 8> km;
    uint8_t app_ind{};
    for (int8_t xd{-1}; xd < 2; xd++) for (int8_t yd{-1}; yd < 2; yd++) {
      if ((xd == 0) && (yd == 0)) { continue; }
      km[app_ind++] = C_diff{xd, yd};
    }
    return km;
  }()};

  const std::array<std::array<C_diff, 7>, 4> bishop{[]{
    std::array<std::array<C_diff, 7>, 4> bm;
    uint8_t dn{};
    for (int8_t dir1{-1}; dir1 < 2; dir1 += 2) for (int8_t dir2{-1}; dir2 < 2; dir2 += 2) {
      for (int8_t di{1}; di < 8; ++di) {
        bm[dn][di - 1] = C_diff{dir1*di, dir2*di};
      }
      ++dn;
    }
    return bm;
  }()};

  const std::array<std::array<C_diff, 7>, 4> rook{[]{
    std::array<std::array<C_diff, 7>, 4> rm;
    uint8_t dn{};
    uint8_t in_d{};
    for (int8_t d1{-7}; d1 < 8; ++d1) {
      if (d1 == 0) {
        ++dn;
        in_d = 0;
        continue;
      } else {
        rm[dn][in_d++] = C_diff{(d1 > 0) ? (d1): (-8 - (d1)), 0};
      }
    }
    
    in_d = 0;
    ++dn;
    
    for (int8_t d1{-7}; d1 < 8; ++d1) {
      if (d1 == 0) {
        ++dn;
        in_d = 0;
        continue;
      } else {
        rm[dn][in_d++] = C_diff{0, (d1 > 0) ? (d1): (-8 - (d1))};
      }
    }
    return rm;
  }()};

  const std::array<std::array<C_diff, 7>, 8> queen{[]{
    std::array<std::array<C_diff, 7>, 8> qm;
    uint8_t dn{};
    
    for (int8_t dir1{-1}; dir1 < 2; dir1 += 2) for (int8_t dir2{-1}; dir2 < 2; dir2 += 2) {
      for (int8_t di{1}; di < 8; ++di) {
        qm[dn][di - 1] = C_diff{dir1*di, dir2*di};
      }
      ++dn;
    }

    uint8_t in_d{};
    for (int8_t d1{-7}; d1 < 8; ++d1) {
      if (d1 == 0) {
        ++dn;
        in_d = 0;
        continue;
      } else {
        qm[dn][in_d++] = C_diff{(d1 > 0) ? (d1): (-8 - (d1)), 0};
      }
    }
    
    in_d = 0;
    ++dn;
    
    for (int8_t d1{-7}; d1 < 8; ++d1) {
      if (d1 == 0) {
        ++dn;
        in_d = 0;
        continue;
      } else {
        qm[dn][in_d++] = C_diff{0, (d1 > 0) ? (d1): (-8 - (d1))};
      }
    }

    return qm;
  }()};

  //auto all_pieces{std::make_tuple(bishop, )};
}
