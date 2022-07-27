#pragma once

#include "Coord.hpp"
#include "Piece.hpp"
#include <ostream>
#include <iostream>

namespace DxChess {
    struct Bitboard {
        uint64_t bits;

        constexpr Bitboard& Mark(Coord coord) {
            bits |= CoordToBit(coord);
            return *this;
        }

        constexpr Bitboard& Remove(Coord coord) {
            bits &= ~CoordToBit(coord);
            return *this;
        }

        bool IsMarked(Coord coord) const {
            return (bits & CoordToBit(coord)) == CoordToBit(coord);
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
                }
            } else if constexpr (std::is_same_v<T, Diagonal>) {
                switch (x) {
                    case Diagonal::Diagonal:     return { 0x8040201008040201 };
                    case Diagonal::Antidiagonal: return { 0x0102040810204080 };
                }
            } else if constexpr (std::is_same_v<T, Checkerboard>) {
                switch (x) {
                    case Checkerboard::Light: return { 0x55AA55AA55AA55AA };
                    case Checkerboard::Dark:  return { 0xAA55AA55AA55AA55 };
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
                }
            } else {
                switch (piece.type) {
                    case Pawn:   return GetCommonLayout(Rank::R7);
                    case Bishop: return { CoordToBit(C8) | CoordToBit(F8) };
                    case Knight: return { CoordToBit(B8) | CoordToBit(G8) };
                    case Rook:   return { CoordToBit(B8) | CoordToBit(G8) };
                    case Queen:  return { CoordToBit(D8)                  };
                    case King:   return { CoordToBit(E8)                  };
                }
            }
        }

        friend std::ostream& operator<<(std::ostream& out, Bitboard b);

        friend constexpr Bitboard operator&(Bitboard lhs, Bitboard rhs) {
            return { lhs.bits & rhs.bits };
        }

        constexpr Bitboard& operator&=(Bitboard rhs) {
            *this = *this & rhs;
            return *this;
        }

        friend constexpr Bitboard operator|(Bitboard lhs, Bitboard rhs) {
            return { lhs.bits & rhs.bits };
        }

        constexpr Bitboard& operator|=(Bitboard rhs) {
            *this = *this | rhs;
            return *this;
        }

        private:
        static constexpr uint64_t CoordToBit(Coord coord) {
            return 1uLL << static_cast<uint8_t>(coord);
        }
    };

}