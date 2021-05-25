#pragma once

#include <windows.h>
#include <iostream>
#include <lmcons.h>
#include <thread>
#include <random>
#include <string>
#include <ctime>

#include "xorstr.h"

namespace config {
	std::string response;

    namespace autoclicker {
        POINT pos{ 0, 0 };

        static int left_cps = 16;

        static bool enabled = false;
    }
}

namespace random {
    std::mt19937 mersenne{ static_cast<std::mt19937::result_type>( std::time( nullptr ) ) };

    int edited_cps,
        reset_value,
        clicks,
        return_ms;
}

namespace launch {
    namespace thread {
        extern auto autoclicker( );
    }
}

auto random_int( int min, int max ) {
    std::uniform_int_distribution gen{ min, max }; 
    return gen( random::mersenne );
}