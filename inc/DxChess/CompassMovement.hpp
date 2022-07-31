#pragma once

#include <cstdint>

namespace DxChess {
    struct CompassMovement {
        private:
        using ShiftFunc = uint64_t(*)(uint64_t);
        ShiftFunc shifter;

        constexpr CompassMovement(ShiftFunc f) : shifter(f) { }
        
        public:
        template<int8_t Amt, ShiftFunc SecondStep = +[](uint64_t x) { return x; }>
        static constexpr CompassMovement Create() {
            return CompassMovement(+[](uint64_t b) {
                if constexpr (Amt > 0)
                    return SecondStep(b << Amt);
                else if constexpr (Amt < 0)
                    return SecondStep(b >> -Amt);
                else
                    return b;
            });
        }

        constexpr uint64_t operator()(uint64_t b) {
            return shifter(b);
        }

        friend constexpr bool operator==(CompassMovement lhs, CompassMovement rhs) = default;
    };


}