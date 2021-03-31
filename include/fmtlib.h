#pragma once

#ifndef __has_include
    // No way to check if <format> exists, so play safe
    #include "fmt/format.h"
#else
    #if __has_include(<format>)
        // Hurrah, we're using a C++20 compiler!
        #include <format>
        namespace fmt = std::format;
    #else
        // No full C++20 support
        #include "fmt/format.h"
    #endif
#endif
