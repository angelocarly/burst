/**
 * Utils.cpp
 *
 * @file	Utils.cpp
 * @author	Angelo Carly
 * @date	02/11/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#include "burst/Utils.h"

bool
burst::Utils::IsTargetApple()
{
    #ifdef __APPLE__
        return true;
    #else
        return false;
    #endif
}
