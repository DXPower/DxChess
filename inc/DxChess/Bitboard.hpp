#pragma once

#include <ostream>
#include "Coord.hpp"
#include "Piece.hpp"
#include "CompassMovement.hpp"

namespace DxChess {
    struct Bitboard {
        uint64_t bits;

        static consteval Bitboard NullBoard() {
            return { 0 } ;
        }

        static constexpr Bitboard FromCoords(std::same_as<Coord> auto... coords) {
            if constexpr (sizeof...(coords) > 0) return { (CoordToBit(coords) | ...) };
            else return NullBoard();
        }

        constexpr Bitboard& Mark(Coord coord) {
            bits |= CoordToBit(coord);
            return *this;
        }

        constexpr Bitboard Marked(Coord coord) const {
            Bitboard b = *this;
            return b.Mark(coord);
        }

        constexpr Bitboard& Remove(Coord coord) {
            bits &= ~CoordToBit(coord);
            return *this;
        }

        constexpr Bitboard Removed(Coord coord) const {
            Bitboard b = *this;
            return b.Remove(coord);
        }

        constexpr bool IsMarked(Coord coord) const {
            return (bits & CoordToBit(coord)) == CoordToBit(coord);
        }
        
        constexpr Bitboard ShiftedBy(CompassMovement movement) const {
            return { movement(bits) };
        }

        constexpr uint8_t BitscanForward(uint8_t startingIndex) const {
            int index = __builtin_ffsll(static_cast<int64_t>(bits) >> startingIndex);
            return static_cast<uint8_t>(index) + startingIndex;
        }

        template<typename T>
        requires RankOrFile_c<T> || std::is_same_v<T, Diagonal> || std::is_same_v<T, Checkerboard>
        static constexpr Bitboard GetCommonLayout(T x) {
            if constexpr (std::is_same_v<T, Rank>) {
                switch (x) {
                    case Rank::R1: return { 0x00000000000000FF };
                    case Rank::R2: return { 0x000000000000FF00 };
                    case Rank::R3: return { 0x0000000000FF0000 };
                    case Rank::R4: return { 0x00000000FF000000 };
                    case Rank::R5: return { 0x000000FF00000000 };
                    case Rank::R6: return { 0x0000FF0000000000 };
                    case Rank::R7: return { 0x00FF000000000000 };
                    case Rank::R8: return { 0xFF00000000000000 };
                    default: __assume(0);
                }
            } else if constexpr (std::is_same_v<T, File>) {
                switch (x) {
                    case File::A: return { 0x0101010101010101 };
                    case File::B: return { 0x0202020202020202 };
                    case File::C: return { 0x0404040404040404 };
                    case File::D: return { 0x0808080808080808 };
                    case File::E: return { 0x1010101010101010 };
                    case File::F: return { 0x2020202020202020 };
                    case File::G: return { 0x4040404040404040 };
                    case File::H: return { 0x8080808080808080 };
                    default: __assume(0);
                }
            } else if constexpr (std::is_same_v<T, Diagonal>) {
                switch (x) {
                    case Diagonal::Diagonal:     return { 0x8040201008040201 };
                    case Diagonal::Antidiagonal: return { 0x0102040810204080 };
                    default: __assume(0);
                }
            } else if constexpr (std::is_same_v<T, Checkerboard>) {
                switch (x) {
                    case Checkerboard::Light: return { 0x55AA55AA55AA55AA };
                    case Checkerboard::Dark:  return { 0xAA55AA55AA55AA55 };
                    default: __assume(0);
                }
            }
        }

        static constexpr Bitboard GetStartingLayout(Piece piece) {
            using enum PieceType;
            using enum Coord;

            if (piece.color == Color::White) {
                switch (piece.type) {
                    case Pawn:   return GetCommonLayout(Rank::R2);
                    case Bishop: return { CoordToBit(C1) | CoordToBit(F1) };
                    case Knight: return { CoordToBit(B1) | CoordToBit(G1) };
                    case Rook:   return { CoordToBit(B1) | CoordToBit(G1) };
                    case Queen:  return { CoordToBit(D1)                  };
                    case King:   return { CoordToBit(E1)                  };
                    default: __assume(0);
                }
            } else {
                switch (piece.type) {
                    case Pawn:   return GetCommonLayout(Rank::R7);
                    case Bishop: return { CoordToBit(C8) | CoordToBit(F8) };
                    case Knight: return { CoordToBit(B8) | CoordToBit(G8) };
                    case Rook:   return { CoordToBit(B8) | CoordToBit(G8) };
                    case Queen:  return { CoordToBit(D8)                  };
                    case King:   return { CoordToBit(E8)                  };
                    default: __assume(0);
                }
            }
        }

        friend constexpr bool operator==(Bitboard lhs, Bitboard rhs) = default;

        constexpr Bitboard& operator&=(Bitboard rhs) {
            bits &= rhs.bits;
            return *this;
        }

        friend constexpr Bitboard operator&(Bitboard lhs, Bitboard rhs) {
            lhs &= rhs;
            return lhs;
        }

        constexpr Bitboard& operator|=(Bitboard rhs) {
            bits |= rhs.bits;
            return *this;
        }

        friend constexpr Bitboard operator|(Bitboard lhs, Bitboard rhs) {
            lhs |= rhs;
            return lhs;
        }

        friend constexpr Bitboard operator~(Bitboard b) {
            return { ~b.bits };
        }

        friend std::ostream& operator<<(std::ostream& out, Bitboard b);

        private:
        static constexpr uint64_t CoordToBit(Coord coord) {
            return 1ULL << static_cast<uint8_t>(coord);
        }
    };

}