#include <string.h>

#include "ls_utils.h"

bool ls_utils_is_string_equal(const char *str1, const char *str2)
{
    if (NULL == str1 || NULL == str2)
        return false;

    return 0 == strcmp(str1, str2);
}

bool ls_utils_is_string_equal_n(const char* str1, const char* str2, int num)
{
    if (NULL == str1 || NULL == str2)
        return false;

    return 0 == strncmp(str1, str2, num);
}

lsReal ls_utils_angle_normalize(lsReal angle)
{
    if (angle >= 0)
    {
        // angle = 2n * 360 + theta
        int n = (int)(angle / 360.0f);

        // angle = theta
        angle -= 2 * n * 360.0f;
    }
    else
    {
        // angle = -2n * 360 + theta
        int n = -(int)(angle / 360.0f);

        // 诸如-2.3这种，取-3
        if (angle / 360.0f != (lsReal)n)
            n--;
        
        // angle = theta [+ 360]
        angle -= 2 * n * 360.0f;
    }
    
    return angle;
}
