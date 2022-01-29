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
#include "KeyCodes.hpp"
#include "Utils/String.hpp"

enum class KeyState : uint8_t{
    Pressed, Released
};

// if caps is on then case will be changed
char ChangeCase(char c, bool caps){
    if(caps){
        if(isupper(c)) return tolower(c);
        if(islower(c)) return toupper(c);
    }

    return c;
}

static const char* _qwertzuiop = "qwertyuiop"; // 0x10-0x1c
static const char* _asdfghjkl = "asdfghjkl";
static const char* _yxcvbnm = "zxcvbnm";
static const char* _num = "1234567890";
static const char* _shiftnum = "!@#$%^&*()";

char KeyboardToASCII(uint8_t key){
    static bool caps = false;
    static bool capslock = false;
    static bool numlock = false;
    static KeyState lshift = KeyState::Released;
    static KeyState rshift = KeyState::Released;
    static KeyState lctrl = KeyState::Released;
    static KeyState rctrl = KeyState::Released;

    if(key == CAPSLOCK_RELEASED){
        capslock = !capslock;
    }else if(key == LSHIFT_PRESSED){
        lshift = KeyState::Pressed;
    }else if(key == LSHIFT_RELEASED){
        lshift = KeyState::Released;
    }else if(key == RSHIFT_PRESSED){
        rshift = KeyState::Pressed;
    }else if(key == RSHIFT_RELEASED){
        rshift = KeyState::Released;
    }else if(key == NUMLOCK_RELEASED){
        numlock = !numlock;
    }

    if(lshift == KeyState::Pressed || rshift == KeyState::Pressed){
        if(capslock) caps = false;
        else caps = true;
    }else if(lshift == KeyState::Released || rshift == KeyState::Released){
        caps = capslock;
    }

    // alphabets
    if(key >= Q_PRESSED && key <= P_PRESSED){
        return ChangeCase(_qwertzuiop[key - Q_PRESSED], caps);
    } else if(key >= A_PRESSED && key <= L_PRESSED) {
        return ChangeCase(_asdfghjkl[key - A_PRESSED], caps);
    } else if(key >= Y_PRESSED && key <= M_PRESSED) {
        return ChangeCase(_yxcvbnm[key - Y_PRESSED], caps);
    }

    // numbers
    if(key >= ONE_PRESSED && key <= ZERO_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? _shiftnum[key - ONE_PRESSED] : _num[key - ONE_PRESSED];
    }

    // special keys
    if(key == ENTER_PRESSED) return '\n';
    if(key == SPACE_PRESSED) return ' ';
    if(key == BACKSPACE_PRESSED) return '\b';
    if(key == TAB_PRESSED) return '\t';

    if(numlock){
        if(key == KPD1_PRESSED) return '1';
        if(key == KPD2_PRESSED) return '2';
        if(key == KPD3_PRESSED) return '3';
        if(key == KPD4_PRESSED) return '4';
        if(key == KPD5_PRESSED) return '5';
        if(key == KPD6_PRESSED) return '6';
        if(key == KPD7_PRESSED) return '7';
        if(key == KPD8_PRESSED) return '8';
        if(key == KPD9_PRESSED) return '9';
        if(key == KPD0_PRESSED) return '0';
    }

    if(key == KPDPLUS_PRESSED) return '+';
    if(key == KPDHYPHEN_PRESSED) return '-';
    if(key == KPDSTAR_PRESSED) return '*';
    if(key == KPDPOINT_PRESSED) return '.';

    if(key == EQUALTO_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? '+' : '=';
    }

    if(key == SINGLEQUOTE_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? '"' : '\'';
    }

    if(key == POINT_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? '>' : '.';
    }

    if(key == COMMA_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? '<' : ',';
    }

    if(key == SEMICOLON_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? ':' : ';';
    }

    if(key == COMMA_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? '<' : ',';
    }

    if(key == OPENSQBRACKET_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? '{' : '[';
    }

    if(key == CLOSEDSQBRACKET_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? '}' : ']';
    }

    if(key == BACKSLASH_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? '|' : '\\';
    }

    if(key == HYPHEN_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? '_' : '-';
    }

    if(key == FWDSLASH_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? '?' : '/';
    }

    if(key == BACKTICK_PRESSED){
        return (lshift == KeyState::Pressed || rshift == KeyState::Pressed) ? '~' : '`';
    }

    return 0;
}

void INTERRUPT_CALLEE_API HandleKeyboardEvent(uint8_t scancode){
    if(scancode == 0) return;
    PutChar(KeyboardToASCII(scancode));
}
