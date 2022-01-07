#pragma once

#include "ElementGUI.h"
#include <windows.h>
#include <gdiplus.h>
#include <memory>
#include <vector>

using ElementGUIPtr = std::unique_ptr<ElementGUI>;
using ListGUIElements = std::vector<ElementGUIPtr>;

class GUI
{
public:
    GUI() = default;
    ~GUI() noexcept;

    void AddElement(const HWND &_hWnd, ElementGUIPtr _elementGUIPtr) noexcept;
    void DrawElements(HDC &_hDC, const HWND &_hWnd) noexcept;
    void TakeImage(BitmapPtr _image) noexcept;

private:
    ListGUIElements m_listGUIElements;
};
