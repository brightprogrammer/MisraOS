/**
 *@file Printf.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/22/2022
 *@brief Provides a printf for kernel
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef PRINTF_HPP
#define PRINTF_HPP

__attribute__ ((format (printf, 1, 2))) int Printf (const char* fmtstr, ...);

#endif // PRINTF_HPP
