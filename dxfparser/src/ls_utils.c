#include <string.h>

#include "ls_utils.h"

bool ls_utils_is_string_equal(const char *str1, const char *str2)
{
    if (NULL == str1 || NULL == str2)
        return false;

    return 0 == strcmp(str1, str2);
}
