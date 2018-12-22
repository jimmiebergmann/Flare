/*
* MIT License
*
* Copyright(c) 2018 Jimmie Bergmann
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#ifndef FLARE_BUILD_HPP
#define FLARE_BUILD_HPP

#include <stdint.h>
#include <exception>
#include <memory>
#include <string>

// Platform
#if defined( _WIN32 ) || defined( __WIN32__ ) || defined( _WIN64 ) || defined( __WIN64__ )
    #define FLARE_PLATFORM_WINDOWS
    #include "platform\win32Headers.hpp"
#elif defined( linux ) || defined( __linux )
    #define FLARE_PLATFORM_LINUX
#else
    #error Unkown platform.
#endif

// Build type
#if defined( NDEBUG ) || !defined( _DEBUG )
    #define FLARE_BUILD_RELEASE
#else
    #define FLARE_BUILD_DEBUG
#endif


#if !defined(FLARE_STATIC_LIB)
    // Turn off microsoft vsc warning
    #ifdef _MSC_VER
        #pragma warning(disable : 4251)
    #endif

    // Define as export or import, if FLARE_EXPORTS is defined.
    #if defined(FLARE_PLATFORM_WINDOWS)
        #if defined(FLARE_EXPORTS)
            #define FLARE_API __declspec(dllexport)
        #else
            #define FLARE_API __declspec(dllimport)
        #endif
    #else
        #define FLARE_API
    #endif

#else
    #define FLARE_API
#endif

#define FLARE_VULKAN

#endif