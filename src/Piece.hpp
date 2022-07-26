#pragma once

namespace DxChess {
    enum class PieceType { Pawn, Bishop, Rook, Knight, Queen, King };
    enum class Color { White, Black };

    struct Piece {
        PieceType type;
        Color color;
    };
}