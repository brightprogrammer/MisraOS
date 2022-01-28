/**
 *@file Keyboard.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/28/2022
 *@brief definition of keyboard helper class
 *@copyright BSD 3-Clause License

 Copyright (c) 2022, Siddharth Mishra
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Keyboard.hpp"
#include "Puts.hpp"

// keycodes
enum KeyCode {
    NULL_KEY = 0,
    Q_PRESSED = 0x10,
    Q_RELEASED = 0x90,
    W_PRESSED = 0x11,
    W_RELEASED = 0x91,
    E_PRESSED = 0x12,
    E_RELEASED = 0x92,
    R_PRESSED = 0x13,
    R_RELEASED = 0x93,
    T_PRESSED = 0x14,
    T_RELEASED = 0x94,
    Z_PRESSED = 0x15,
    Z_RELEASED = 0x95,
    U_PRESSED = 0x16,
    U_RELEASED = 0x96,
    I_PRESSED = 0x17,
    I_RELEASED = 0x97,
    O_PRESSED = 0x18,
    O_RELEASED = 0x98,
    P_PRESSED = 0x19,
    P_RELEASED = 0x99,
    A_PRESSED = 0x1E,
    A_RELEASED = 0x9E,
    S_PRESSED = 0x1F,
    S_RELEASED = 0x9F,
    D_PRESSED = 0x20,
    D_RELEASED = 0xA0,
    F_PRESSED = 0x21,
    F_RELEASED = 0xA1,
    G_PRESSED = 0x22,
    G_RELEASED = 0xA2,
    H_PRESSED = 0x23,
    H_RELEASED = 0xA3,
    J_PRESSED = 0x24,
    J_RELEASED = 0xA4,
    K_PRESSED = 0x25,
    K_RELEASED = 0xA5,
    L_PRESSED = 0x26,
    L_RELEASED = 0xA6,
    Y_PRESSED = 0x2C,
    Y_RELEASED = 0xAC,
    X_PRESSED = 0x2D,
    X_RELEASED = 0xAD,
    C_PRESSED = 0x2E,
    C_RELEASED = 0xAE,
    V_PRESSED = 0x2F,
    V_RELEASED = 0xAF,
    B_PRESSED = 0x30,
    B_RELEASED = 0xB0,
    N_PRESSED = 0x31,
    N_RELEASED = 0xB1,
    M_PRESSED = 0x32,
    M_RELEASED = 0xB2,

    // 1,2,3...9,0
    ONE_PRESSED = 0x02,
    NINE_PRESSED = 0x0A,
    ZERO_PRESSED = 0x0B,

    POINT_PRESSED = 0x34,
    POINT_RELEASED = 0xB4,

    FWDSLASH_PRESSED = 0x35,
    FWDSLASH_RELEASED = 0xB5,

    BACKSPACE_PRESSED = 0xE,
    BACKSPACE_RELEASED = 0x8E,

    SPACE_PRESSED = 0x39,
    SPACE_RELEASED = 0xB9,

    ENTER_PRESSED = 0x1C,
    ENTER_RELEASED = 0x9C,
};


static const char* _qwertzuiop = "qwertyuiop"; // 0x10-0x1c
static const char* _asdfghjkl = "asdfghjkl";
static const char* _yxcvbnm = "zxcvbnm";
static const char* _num = "1234567890";

uint8_t KeyboardToASCII(uint8_t key){
    // special keys
    if(key == ENTER_PRESSED) return '\n';
    if(key == SPACE_PRESSED) return ' ';
    if(key == BACKSPACE_PRESSED) return '\b';
    if(key == POINT_PRESSED) return '.';
    if(key == FWDSLASH_PRESSED) return '/';

    // numbers
    if(key >= ONE_PRESSED && key <= ZERO_PRESSED){
        return _num[key - ONE_PRESSED];
    }

    // alphabets
    if(key >= Q_PRESSED && key <= P_PRESSED){
        return _qwertzuiop[key - Q_PRESSED];
    } else if(key >= A_PRESSED && key <= L_PRESSED) {
        return _asdfghjkl[key - A_PRESSED];
    } else if(key >= Y_PRESSED && key <= M_PRESSED) {
        return _yxcvbnm[key - Y_PRESSED];
    }

    return 0;
}

void INTERRUPT_CALLEE_API HandleKeyboardEvent(uint8_t scancode){
    if(scancode == 0) return;
    PutChar(KeyboardToASCII(scancode));
}
