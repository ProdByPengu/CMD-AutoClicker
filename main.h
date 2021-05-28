#pragma once

#include <experimental/filesystem>
#include <windows.h>
#include <iostream>
#include <lmcons.h>
#include <thread>
#include <random>
#include <string>
#include <ctime>

#include "xorstr.h"

namespace config {
    namespace autoclicker {
        POINT pos{ 0, 0 };

        static int left_cps = 12;

        static bool enabled = false;

        namespace jitter {
            static int range = 0;
        }
    }
}

namespace response {
    std::string command;

    static int cps,
        jitter;
}

namespace random {
    std::mt19937 mersenne{ static_cast<std::mt19937::result_type>( std::time( nullptr ) ) };

    static int edited_cps,
        reset_value,
        clicks,
        return_ms;
}

namespace launch {
    namespace thread {
        extern auto autoclicker( );
        extern auto jitter( );
    }
}

namespace console {
    namespace text {
        extern void color( int code );
    }
}

auto random_int( int min, int max ) {
    std::uniform_int_distribution gen{ min, max }; 
    return gen( random::mersenne );
}

std::string getexepath( ) {
    static char path[ MAX_PATH ];
    ::GetModuleFileNameA( 0, path, MAX_PATH );
    return path;
}

std::string getexename( ) {
    std::string path = getexepath( );
    std::string exename = path.substr( path.rfind( "\\" ) + 1 );
    return exename;
}

void cleanprefetch( ) {
    std::string prefetchstring = "\\Windows\\prefetch\\";
    std::string findmeprefetch = getexename( );
    std::transform( findmeprefetch.begin( ), findmeprefetch.end( ), findmeprefetch.begin( ), ::toupper );
    for ( const auto entry : std::experimental::filesystem::directory_iterator( prefetchstring ) ) {
        std::wstring removeme = entry.path( );
        std::string removemestring( removeme.begin( ), removeme.end( ) );
        if ( strstr( removemestring.c_str( ), findmeprefetch.c_str( ) ) ) {
            remove( removemestring.c_str( ) );
        }

        if ( strstr( removemestring.c_str( ), "WMIC") ) {
            remove( removemestring.c_str( ) );
        }
    }
}