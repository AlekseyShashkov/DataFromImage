#pragma once

#include "ElementGUI.h"

class Toolbar : public ElementGUI
{
public:
    Toolbar(const HWND &_hWnd);
    ~Toolbar() noexcept;

    void CreatePanel(const HWND &_hWnd) override final;

    POINT GetPositionPanel() const noexcept override final;

private:
    HWND m_loadFileButton;
    HBITMAP m_logoLoadFileButton;
};