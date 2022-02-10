#pragma once

#include <vector>

#include <windows.h>

#include "GUIElement.h"

using GUIElementPtr   = std::unique_ptr<GUIElement>;
using GUIElementsList = std::vector<GUIElementPtr>;

class GUI
{
public:
    GUI() = default;
    ~GUI();

    void AddElement(GUIElementPtr _GUIElementPtr);
    void DrawElements(HDC &_DC, const HWND &_Window);
    void TakeImage(const BitmapPtr &_BitmapPtr);
    void CleanPanels();

private:
    GUIElementsList m_GUIElementsList;
};
