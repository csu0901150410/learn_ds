/**
 * @file ls_utils.h
 * @author lhy (lianghongyu.csu@gmail.com)
 * @brief 本文件描述了常用的工具函数接口
 * @version 0.1
 * @date 2024-09-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __LS_UTILS_H__
#define __LS_UTILS_H__

#include <stdbool.h>

/**
 * @brief 比较两个'\0'结尾的字符串是否相等
 * 
 * @param str1 字符串指针1
 * @param str2 字符串指针2
 * @return true 字符串相等
 * @return false 字符串不等
 */
bool ls_utils_is_string_equal(const char* str1, const char* str2);

#endif// __LS_UTILS_H__