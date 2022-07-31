#pragma once

#include <concepts>
#include <cstdint>
#include <ostream>

namespace DxChess {
    enum class Coord : uint8_t {
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8
    };

    enum class File : uint8_t {
        A, B, C, D, E, F, G, H
    };

    enum class Rank : uint8_t {
        R1 = 1, R2, R3, R4, R5, R6, R7, R8
    };

    enum class Diagonal : uint8_t {
        Diagonal, Antidiagonal
    };

    enum class Checkerboard : uint8_t {
        Light, Dark
    };

    template<typename T>
    concept RankOrFile_c = std::is_same_v<T, Rank> || std::is_same_v<T, File>;

    struct CoordNumerical {
        uint8_t x, y;

        constexpr CoordNumerical() = default;
        constexpr CoordNumerical(Coord coord) {
            x = static_cast<uint8_t>(coord) % 8;
            y = static_cast<uint8_t>(coord) / 8;
        }

        friend bool operator==(CoordNumerical lhs, CoordNumerical rhs) = default;
        friend std::ostream& operator<<(std::ostream& out, CoordNumerical rhs);
    };

    struct ManhattanDistanceResult {
        int_least8_t x, y;

        friend bool operator==(ManhattanDistanceResult lhs, ManhattanDistanceResult rhs) = default;
        friend std::ostream& operator<<(std::ostream& out, ManhattanDistanceResult rhs);
    };

    // Positive is up and to the right, negative is down and to the left. 
    constexpr ManhattanDistanceResult ManhattanDistance(CoordNumerical lhs, CoordNumerical rhs) {
        return { static_cast<int8_t>(lhs.x - rhs.x), static_cast<int8_t>(lhs.y - rhs.y) };
    }
}