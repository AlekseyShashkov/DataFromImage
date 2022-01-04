#pragma once

#include "ElementGUI.h"

class Image : public ElementGUI
{
public:
    Image(const HWND &_hWnd);
    ~Image() noexcept;

    void CreatePanel(const HWND &_hWnd) override final;

    POINT GetPositionPanel() const noexcept override final;

private:

};