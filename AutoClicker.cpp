#include "AutoClicker.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <dinput.h>
#include <tchar.h>
#include <random>
#include <string>
#include <MMSystem.h>


using std::string;
using std::mbstowcs;


DWORD pid;



POINT pt;

bool tabbed;

float color[];
static bool clicker = false;
static bool rightClicker = false;
static int min = 10;
int movex;
int movey;
bool bindCheck;
static int max = 10;
static int rmin = 10;
static int rmax = 10;
static bool jitter = false;
static int maxjitter = 10;
bool soundEvent = false;
bool soundEventR = false;
static const char* items[]{"1.7.10", "1.8.9", "Lunar 1.7.10", "Lunar 1.8.9", "Badlion 1.7.10", "Badlion 1.8.9", "LabyMod 1.8.9"};
static int selectedItem = 0;
POINT ptr;

void AutoClicker::Clicker()
{
    if (GetAsyncKeyState(0x46))
    {
        clicker = !clicker;
        Sleep(10);
        keybd_event(0x46, 0, KEYEVENTF_KEYUP, 0);
        soundEvent = !soundEvent;
        if (soundEvent)
        {
            PlaySound(TEXT("on.wav"), NULL, SND_SYNC);
        }
        if (!soundEvent)
        {
            PlaySound(TEXT("off.wav"), NULL, SND_SYNC);
        }
        
    }
    

    if (GetAsyncKeyState(0x56))
    {
        rightClicker = !rightClicker;
        Sleep(10);
        keybd_event(0x56, 0, KEYEVENTF_KEYUP, 0);
        soundEventR = !soundEventR;
        if (soundEventR)
        {
            PlaySound(TEXT("on.wav"), NULL, SND_SYNC);
        }
        if (!soundEventR)
        {
            PlaySound(TEXT("off.wav"), NULL, SND_SYNC);
        }
        
    }
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Begin("Auto Clicker", 0, ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Minecraft Version");
    ImGui::Combo("", &selectedItem, items, IM_ARRAYSIZE(items));
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigWindowsResizeFromEdges = false;
    ImGui::Checkbox("AutoClicker", &clicker);

    ImGui::SliderInt("Min CPS", &min, 0, 20);
    ImGui::SliderInt("Max CPS", &max, 0, 20);

    //RightClicker
    ImGui::Text("Right-Clicker");
    ImGui::Checkbox("Right-Clicker", &rightClicker);
    ImGui::SliderInt("Min", &rmin, 0, 60);
    ImGui::SliderInt("Max", &rmax, 0, 60);
    ImGui::Text("Fake Jitter");
    ImGui::Checkbox("Jitter", &jitter);
    ImGui::SliderInt("Max Pixels", &maxjitter, 0, 60);
    ImGui::Text("F = Autoclicker, V = Right-Clicker");
    ImGui::Text("Made by Clearly");
    ImGui::Text("Powered by imgui");
    

    
    //VERSION CODE
    LPCSTR WindowName = "Lunar Client (1.7.10-646ed7f/master)";
    if (selectedItem == 0)
    {
        WindowName = "Minecraft 1.7.10";
    }
    if (selectedItem == 1)
    {
        WindowName = "Minecraft 1.8.9";
    }
    if (selectedItem == 2)
    {
        WindowName = "Lunar Client (1.7.10-646ed7f/master)";    
    }
    if (selectedItem == 3)
    {
        WindowName = "Lunar Client (1.8.9-646ed7f/master)";
    }
    if (selectedItem == 4)
    {
        WindowName = "Badlion Minecraft Client v2.17.2-da01326-PRODUCTION (1.7.10)";
    }
    if (selectedItem == 5)
    {
        WindowName = "Badlion Minecraft Client v2.17.2-da01326-PRODUCTION (1.8.9)";
    }
    if (selectedItem == 6)
    {
        WindowName = "Minecraft 1.8.9 | LabyMod 3.7.7";
    }
    HWND hwnd = FindWindowA(NULL, WindowName);
    HWND hWnd = GetForegroundWindow();

    //
    


    
    if (clicker && hWnd == hwnd && selectedItem != 7)
    {
        
        while (GetAsyncKeyState(VK_LBUTTON) && hWnd == hwnd)
        {
            if (jitter)
            {
                GetCursorPos(&ptr);
                std::random_device rd;     // only used once to initialise (seed) engine
                std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
                std::uniform_int_distribution<int> movex(ptr.x - maxjitter, ptr.x + maxjitter); // guaranteed unbiased
                std::uniform_int_distribution<int> movey(ptr.y - maxjitter, ptr.y + maxjitter);

                SetCursorPos(movex(rng), movey(rng));
                
            }
             std::random_device rd;     // only used once to initialise (seed) engine
             std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
             std::uniform_int_distribution<int> delay(min,max); // guaranteed unbiased
             
             //int delay = (min + (rand() % (int)(max - min + 1)));
             
             GetCursorPos(&pt);
             SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(movex, movey));
             Sleep(1000 / delay(rng) / 2);
             SendMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(ptr.x, ptr.y));
             Sleep(1000 / delay(rng) / 2);         
        }
    }
    if (rightClicker && hWnd == hwnd)
    {
        while (GetAsyncKeyState(VK_RBUTTON))
        {
            
            int rightdelay = (rmin + (rand() % (int)(rmax - rmin + 1)));
            GetCursorPos(&pt);
            SendMessage(hwnd, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(pt.x, pt.y));
            
            Sleep(1000 / rightdelay / 2);
            
            SendMessage(hwnd, WM_RBUTTONUP, 0, MAKELPARAM(pt.x, pt.y));
            Sleep(1000 / rightdelay / 2);
        }
    }
    
    

    ImGui::End();
}
