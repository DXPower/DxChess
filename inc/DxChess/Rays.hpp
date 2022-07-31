#include "Bitboard.hpp"
#include "Compass.hpp"

namespace DxChess {
    enum class RayDirection {
        All, Straights, Diagonals, Vertical, Horizontal, Diagonal, Antidiagonal
    };

    namespace Rays {
        constexpr Bitboard Lookup(Coord coord, RayDirection dir) {
            using enum RayDirection;

            auto selfLookup = [coord](RayDirection dir) { return Lookup(coord, dir); };

            switch (dir) {
                case All:
                    return selfLookup(Vertical) | selfLookup(Horizontal) | selfLookup(Diagonal) | selfLookup(Antidiagonal);
                case Straights:
                    return selfLookup(Vertical) | selfLookup(Horizontal);
                case Diagonals:
                    return selfLookup(Diagonal) | selfLookup(Antidiagonal);
                case Horizontal: {
                    auto rank = Bitboard::GetCommonLayout(Rank::R1).ShiftedBy(CompassMovement movement)
                }
            }
        }
    }
}