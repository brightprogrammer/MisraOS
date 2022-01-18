#include "Print.hpp"
#include "Renderer/FontRenderer.hpp"

#define DEBUG_MSG_COLOR 0xff7dff52
#define WARNING_MSG_COLOR 0xffffcb3d
#define ERROR_MSG_COLOR 0xffff5258
#define NORMAL_MSG_COLOR 0xff00c3ff

void Print(const char* str){
    GetDefaultFontRenderer()->DrawString(str);
}

void PrintDebug(const char* str){
    GetDefaultFontRenderer()->foregroundColour = DEBUG_MSG_COLOR;
    GetDefaultFontRenderer()->DrawString(str);
    GetDefaultFontRenderer()->foregroundColour = NORMAL_MSG_COLOR;
}

void PrintWarning(const char* str){
    GetDefaultFontRenderer()->foregroundColour = WARNING_MSG_COLOR;
    GetDefaultFontRenderer()->DrawString(str);
    GetDefaultFontRenderer()->foregroundColour = NORMAL_MSG_COLOR;
}

void PrintError(const char* str){
    GetDefaultFontRenderer()->foregroundColour = ERROR_MSG_COLOR;
    GetDefaultFontRenderer()->DrawString(str);
    GetDefaultFontRenderer()->foregroundColour = NORMAL_MSG_COLOR;
}
