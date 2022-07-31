#pragma once

#include "Compass.hpp"
#include "CompassMovement.hpp"
#include "Bitboard.hpp"
#include <array>
#include <type_traits>
#include <cassert>

namespace DxChess {
    namespace Compass {
        inline constexpr auto NullMove = CompassMovement::Create<0>();

        namespace detail {
            constexpr uint64_t NotAFile(uint64_t x) {
                return x & ~Bitboard::GetCommonLayout(File::A).bits;
            }

            constexpr uint64_t NotHFile(uint64_t x) {
                return x & ~Bitboard::GetCommonLayout(File::H).bits;
            }

            inline static constexpr int8_t NorthAmt     = +8;
            inline static constexpr int8_t NorthEastAmt = +9;
            inline static constexpr int8_t EastAmt      = +1;
            inline static constexpr int8_t SouthEastAmt = -7;
            inline static constexpr int8_t SouthAmt     = -8;
            inline static constexpr int8_t SouthWestAmt = -9;
            inline static constexpr int8_t WestAmt      = -1;
            inline static constexpr int8_t NorthWestAmt = +7;
        }

        namespace Single {
            inline static constexpr auto North     = CompassMovement::Create<detail::NorthAmt>();
            inline static constexpr auto NorthEast = CompassMovement::Create<detail::NorthEastAmt, &detail::NotAFile>();
            inline static constexpr auto East      = CompassMovement::Create<detail::EastAmt     , &detail::NotAFile>();
            inline static constexpr auto SouthEast = CompassMovement::Create<detail::SouthEastAmt, &detail::NotAFile>();
            inline static constexpr auto South     = CompassMovement::Create<detail::SouthAmt>();
            inline static constexpr auto SouthWest = CompassMovement::Create<detail::SouthWestAmt, &detail::NotHFile>();
            inline static constexpr auto West      = CompassMovement::Create<detail::WestAmt     , &detail::NotHFile>();
            inline static constexpr auto NorthWest = CompassMovement::Create<detail::NorthWestAmt, &detail::NotHFile>();
        };

        namespace Multi {
            namespace detail {
                // RFS = RepeatFilledShift
                inline constexpr Bitboard RFS(Bitboard b, CompassMovement movement, uint8_t n) {
                    for (uint8_t i = 0; i < n; i++) {
                        b |= b.ShiftedBy(movement);
                    }

                    return b;
                }

                template<auto Func, uint8_t... I>
                requires (std::is_invocable_v<decltype(Func), uint8_t> && !std::is_same_v<std::invoke_result_t<decltype(Func), uint8_t>, void>)
                constexpr auto GenerateArrayHelper(std::integer_sequence<uint8_t, I...> seq) -> std::array<std::invoke_result_t<decltype(Func), uint8_t>, seq.size()> {
                    return { Func(I)... };
                }

                template<uint8_t N, auto Func>
                constexpr auto GenerateArray() {
                    return GenerateArrayHelper<Func>(std::make_integer_sequence<uint8_t, N>());
                }

                inline static constexpr auto fillNorth = GenerateArray<8, [](uint8_t x) { return RFS(Bitboard::GetCommonLayout(Rank::R8), Single::South, x); }>();
                inline static constexpr auto fillSouth = GenerateArray<8, [](uint8_t x) { return RFS(Bitboard::GetCommonLayout(Rank::R1), Single::North, x); }>();
                inline static constexpr auto fillEast  = GenerateArray<8, [](uint8_t x) { return RFS(Bitboard::GetCommonLayout(File::H),  Single::West,  x); }>();
                inline static constexpr auto fillWest  = GenerateArray<8, [](uint8_t x) { return RFS(Bitboard::GetCommonLayout(File::A),  Single::East,  x); }>();

                inline static constexpr auto fillNorthEast = GenerateArray<8, [](uint8_t x) { return fillNorth[x] & fillEast[x]; }>();
                inline static constexpr auto fillNorthWest = GenerateArray<8, [](uint8_t x) { return fillNorth[x] & fillWest[x]; }>();
                inline static constexpr auto fillSouthEast = GenerateArray<8, [](uint8_t x) { return fillSouth[x] & fillEast[x]; }>();
                inline static constexpr auto fillSouthWest = GenerateArray<8, [](uint8_t x) { return fillSouth[x] & fillWest[x]; }>();

                template<const std::array<Bitboard, 8>& gutter, int8_t baseAmt> 
                struct Distance {
                    private:
                    template<uint8_t... I>
                    static consteval auto GetCompassMovements(std::integer_sequence<uint8_t, I...>) -> std::array<CompassMovement, 8> {
                        return { CompassMovement::Create<baseAmt * I, [](uint64_t x) { return x & gutter[8 - 1 - I].bits; }>()... };
                    }

                    public:
                    CompassMovement operator()(uint8_t numTiles) const {
                        assert(numTiles < 8);
                        return GetCompassMovements(std::make_integer_sequence<uint8_t, 8>())[numTiles];
                    }
                };

            }

            inline static constexpr auto North     = detail::Distance<detail::fillNorth    , Compass::detail::NorthAmt    >();
            inline static constexpr auto NorthEast = detail::Distance<detail::fillNorthEast, Compass::detail::NorthEastAmt>();
            inline static constexpr auto East      = detail::Distance<detail::fillEast     , Compass::detail::EastAmt     >();
            inline static constexpr auto SouthEast = detail::Distance<detail::fillSouthEast, Compass::detail::SouthEastAmt>();
            inline static constexpr auto South     = detail::Distance<detail::fillSouth    , Compass::detail::SouthAmt    >();
            inline static constexpr auto SouthWest = detail::Distance<detail::fillSouthWest, Compass::detail::SouthWestAmt>();
            inline static constexpr auto West      = detail::Distance<detail::fillWest     , Compass::detail::WestAmt     >();
            inline static constexpr auto NorthWest = detail::Distance<detail::fillNorthWest, Compass::detail::NorthWestAmt>();
        }
    }
}