#include <iostream>
#include <thread>
#include <chrono>
#include "Bitboard.hpp"
#include "Compass.hpp" 

using namespace std::chrono_literals;
using namespace DxChess;

int main() {
    std::cout << "\nNorth ";
    for (int i = 0; i < 8; i++) {
        std::cout << i << '\n' << Compass::Multi::detail::fillNorth[i] << '\n';
    }
    std::cout << "\nEast ";
    for (int i = 0; i < 8; i++) {
        std::cout << i << '\n' << Compass::Multi::detail::fillEast[i] << '\n';
    }
    std::cout << "\nSouth ";
    for (int i = 0; i < 8; i++) {
        std::cout << i << '\n' << Compass::Multi::detail::fillSouth[i] << '\n';
    }
    std::cout << "\nWest ";
    for (int i = 0; i < 8; i++) {
        std::cout << i << '\n' << Compass::Multi::detail::fillWest[i] << '\n';
    }

    auto printBoard = [](Bitboard b) {
        system("cls");

        std::cout << b << '\n';
        std::this_thread::sleep_for(0.5s);
    };


    Bitboard b = 
          Bitboard::GetCommonLayout(Diagonal::Diagonal)
        | Bitboard::GetCommonLayout(Diagonal::Antidiagonal)
        | Bitboard::GetCommonLayout(File::D)
        | Bitboard::GetCommonLayout(Rank::R4);

    auto doShifting = [&](auto shifter) {
        for (uint8_t i = 0; i < 8; i++) {
            printBoard(b.ShiftedBy(shifter(i)));
        }
    };

    printBoard(b);
    doShifting(Compass::Multi::North);
    doShifting(Compass::Multi::NorthEast);
    doShifting(Compass::Multi::East);
    doShifting(Compass::Multi::SouthEast);
    doShifting(Compass::Multi::South);
    doShifting(Compass::Multi::SouthWest);
    doShifting(Compass::Multi::West);
    doShifting(Compass::Multi::NorthWest);

    // printBoard(Diagonal::Diagonal);
    // printBoard(Diagonal::Antidiagonal);
    // printBoard(Checkerboard::Light);
    // printBoard(Checkerboard::Dark);

    // for (int i = 0; i < 8; i++) {
    //     printBoard(static_cast<File>(i));
    // }

    // for (int i = 1; i <= 8; i++) {
    //     printBoard(static_cast<Rank>(i));
    // }
}