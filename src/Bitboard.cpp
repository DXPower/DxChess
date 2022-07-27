#include "Bitboard.hpp"
#include <type_traits>

std::ostream& DxChess::operator<<(std::ostream& out, Bitboard b) {
    for (auto coord = static_cast<std::underlying_type_t<Coord>>(Coord::A8);;) {
        if (b.IsMarked(static_cast<Coord>(coord)))
            out << "@";
        else
            out << "-";

        coord++;
        
        if (coord == 8)
            break;

        if (coord % 8 == 0) {
            coord -= 16;
            out << '\n';
        }
    }

    return out;
}
