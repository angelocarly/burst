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
