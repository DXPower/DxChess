#include <catch2/catch_test_macros.hpp>
#include"Coord.hpp"

using namespace DxChess;

TEST_CASE("Manhattan Distance", "coord") {
    auto testCoords = [](Coord lhs, Coord rhs, ManhattanDistanceResult res) {
        REQUIRE(ManhattanDistance(lhs, rhs) == res);
        REQUIRE(ManhattanDistance(rhs, lhs) == ManhattanDistanceResult{static_cast<int_least8_t>(-res.x), static_cast<int_least8_t>(-res.y)});

    };

    testCoords(Coord::C4, Coord::C4, {  0,  0 });
    testCoords(Coord::E5, Coord::D2, {  1,  3 });
    testCoords(Coord::B6, Coord::F4, { -4,  2 });
    testCoords(Coord::H1, Coord::A8, {  7, -7 });
}