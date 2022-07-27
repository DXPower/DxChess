#include <iostream>
#include <thread>
#include <chrono>
#include "Bitboard.hpp"

using namespace std::chrono_literals;
using namespace DxChess;

int main() {
    auto printBoard = [](auto layout) {
        system("cls");

        Bitboard b = Bitboard::GetCommonLayout(layout);
        b.Mark(Coord::B7);
        b.Remove(Coord::G2);

        std::cout << b << '\n';
        std::this_thread::sleep_for(0.5s);
    };

    printBoard(Diagonal::Diagonal);
    printBoard(Diagonal::Antidiagonal);
    printBoard(Checkerboard::Light);
    printBoard(Checkerboard::Dark);

    for (int i = 0; i < 8; i++) {
        printBoard(static_cast<File>(i));
    }

    for (int i = 1; i <= 8; i++) {
        printBoard(static_cast<Rank>(i));
    }
}