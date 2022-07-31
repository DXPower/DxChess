#include <catch2/catch_test_macros.hpp>
#include "Bitboard.hpp"
#include "Compass.hpp"

using namespace DxChess;

TEST_CASE("Shifting Distance 1, Single Tile", "[bitboard][movement]") {
    auto start = Bitboard::FromCoords(Coord::D4);

    SECTION("North") {
        auto moved = start.ShiftedBy(Compass::Single::North);
        REQUIRE(moved == Bitboard::FromCoords(Coord::D5));

        SECTION("Northeast") {
            auto moved2 = moved.ShiftedBy(Compass::Single::East);

            REQUIRE(moved2 == Bitboard::FromCoords(Coord::E5));
            REQUIRE(moved2 == start.ShiftedBy(Compass::Single::NorthEast));
        }

        SECTION("Northwest") {
            auto moved2 = moved.ShiftedBy(Compass::Single::West);

            REQUIRE(moved2 == Bitboard::FromCoords(Coord::C5));
            REQUIRE(moved2 == start.ShiftedBy(Compass::Single::NorthWest));
        }
    }

    SECTION("South") {
        auto moved = start.ShiftedBy(Compass::Single::South);
        REQUIRE(moved == Bitboard::FromCoords(Coord::D3));

        SECTION("Southheast") {
            auto moved2 = moved.ShiftedBy(Compass::Single::East);

            REQUIRE(moved2 == Bitboard::FromCoords(Coord::E3));
            REQUIRE(moved2 == start.ShiftedBy(Compass::Single::SouthEast));
        }

        SECTION("Southhwest") {
            auto moved2 = moved.ShiftedBy(Compass::Single::West);

            REQUIRE(moved2 == Bitboard::FromCoords(Coord::C3));
            REQUIRE(moved2 == start.ShiftedBy(Compass::Single::SouthWest));
        }
    }

    SECTION("Shifting out of bounds") {
        SECTION("North") {
            start = Bitboard::FromCoords(Coord::A8);

            for (int i = 0; i < 8; i++) {
                REQUIRE(Bitboard::NullBoard() == start.ShiftedBy(Compass::Single::North));
                REQUIRE(Bitboard::NullBoard() == start.ShiftedBy(Compass::Single::NorthEast));
                REQUIRE(Bitboard::NullBoard() == start.ShiftedBy(Compass::Single::NorthWest));

                start = start.ShiftedBy(Compass::Single::East);
            }
        }

        SECTION("South") {
            start = Bitboard::FromCoords(Coord::H1);

            for (int i = 0; i < 8; i++) {
                REQUIRE(Bitboard::NullBoard() == start.ShiftedBy(Compass::Single::South));
                REQUIRE(Bitboard::NullBoard() == start.ShiftedBy(Compass::Single::SouthEast));
                REQUIRE(Bitboard::NullBoard() == start.ShiftedBy(Compass::Single::SouthWest));
                
                start = start.ShiftedBy(Compass::Single::West);
            }
        }

        SECTION("East") {
            start = Bitboard::FromCoords(Coord::H8);

            for (int i = 0; i < 8; i++) {
                REQUIRE(Bitboard::NullBoard() == start.ShiftedBy(Compass::Single::East));
                REQUIRE(Bitboard::NullBoard() == start.ShiftedBy(Compass::Single::NorthEast));
                REQUIRE(Bitboard::NullBoard() == start.ShiftedBy(Compass::Single::SouthEast));
                
                start = start.ShiftedBy(Compass::Single::South);
            }
        }

        SECTION("West") {
            start = Bitboard::FromCoords(Coord::A1);

            for (int i = 0; i < 8; i++) {
                REQUIRE(Bitboard::NullBoard() == start.ShiftedBy(Compass::Single::West));
                REQUIRE(Bitboard::NullBoard() == start.ShiftedBy(Compass::Single::NorthWest));
                REQUIRE(Bitboard::NullBoard() == start.ShiftedBy(Compass::Single::SouthWest));
                
                start = start.ShiftedBy(Compass::Single::North);
            }
        }
    }
}

TEST_CASE("Shifting Distance 1, Multiple Tiles", "[bitboard][movement]") {
    SECTION("Full rank") {
        auto row = Bitboard::GetCommonLayout(Rank::R1);

        REQUIRE(row.ShiftedBy(Compass::Single::North) == Bitboard::GetCommonLayout(Rank::R2));
        REQUIRE(row.ShiftedBy(Compass::Single::South) == Bitboard::NullBoard());
        REQUIRE(row.ShiftedBy(Compass::Single::East) == row.Removed(Coord::A1));
        REQUIRE(row.ShiftedBy(Compass::Single::West) == row.Removed(Coord::H1));

        row = Bitboard::GetCommonLayout(Rank::R8);

        REQUIRE(row.ShiftedBy(Compass::Single::North) == Bitboard::NullBoard());
        REQUIRE(row.ShiftedBy(Compass::Single::South) == Bitboard::GetCommonLayout(Rank::R7));
        REQUIRE(row.ShiftedBy(Compass::Single::East) == row.Removed(Coord::A8));
        REQUIRE(row.ShiftedBy(Compass::Single::West) == row.Removed(Coord::H8));
    }

    SECTION("Full file") {
        auto row = Bitboard::GetCommonLayout(File::A);

        REQUIRE(row.ShiftedBy(Compass::Single::North) == row.Removed(Coord::A1));
        REQUIRE(row.ShiftedBy(Compass::Single::South) == row.Removed(Coord::A8));
        REQUIRE(row.ShiftedBy(Compass::Single::East) == Bitboard::GetCommonLayout(File::B));
        REQUIRE(row.ShiftedBy(Compass::Single::West) == Bitboard::NullBoard());

        row = Bitboard::GetCommonLayout(File::H);

        REQUIRE(row.ShiftedBy(Compass::Single::North) == row.Removed(Coord::H1));
        REQUIRE(row.ShiftedBy(Compass::Single::South) == row.Removed(Coord::H8));
        REQUIRE(row.ShiftedBy(Compass::Single::East) == Bitboard::NullBoard());
        REQUIRE(row.ShiftedBy(Compass::Single::West) == Bitboard::GetCommonLayout(File::G));
    }
}

Bitboard RepeatMovement(Bitboard b, CompassMovement movement, uint8_t n) {
    for (uint8_t i = 0; i < n; i++) {
        b = b.ShiftedBy(movement);
    }

    return b;
}

TEST_CASE("Shifting Distance Multi, Multiple Tiles", "[bitboard][movement]") {
    using namespace Compass;

    auto cross =
          Bitboard::GetCommonLayout(Diagonal::Diagonal)
        | Bitboard::GetCommonLayout(Diagonal::Antidiagonal)
        | Bitboard::GetCommonLayout(File::D)
        | Bitboard::GetCommonLayout(Rank::R4);

    for (uint8_t i = 0; i < 8; i++) {
        REQUIRE(cross.ShiftedBy(Multi::North    (i)) == RepeatMovement(cross, Single::North    , i));
        REQUIRE(cross.ShiftedBy(Multi::NorthEast(i)) == RepeatMovement(cross, Single::NorthEast, i));
        REQUIRE(cross.ShiftedBy(Multi::East     (i)) == RepeatMovement(cross, Single::East     , i));
        REQUIRE(cross.ShiftedBy(Multi::SouthEast(i)) == RepeatMovement(cross, Single::SouthEast, i));
        REQUIRE(cross.ShiftedBy(Multi::South    (i)) == RepeatMovement(cross, Single::South    , i));
        REQUIRE(cross.ShiftedBy(Multi::SouthWest(i)) == RepeatMovement(cross, Single::SouthWest, i));
        REQUIRE(cross.ShiftedBy(Multi::West     (i)) == RepeatMovement(cross, Single::West     , i));
        REQUIRE(cross.ShiftedBy(Multi::NorthWest(i)) == RepeatMovement(cross, Single::NorthWest, i));
    }
}

TEST_CASE("Bitscan", "[bitboard][bitscan]") {
    
    SECTION("Forward") {

    }
}