#include <iostream>
#include "Coord.hpp"

std::ostream& DxChess::operator<<(std::ostream& out, CoordNumerical c) {
    out << "{ x: " << static_cast<int>(c.x) << ", y: " << static_cast<int>(c.y) << " }";
    return out;
}

std::ostream& DxChess::operator<<(std::ostream& out, ManhattanDistanceResult m) {
    out << "{ x: " << static_cast<int>(m.x) << ", y: " << static_cast<int>(m.y) << " }";
    return out;
}