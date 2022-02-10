#pragma once

#include "GUIElement.h"

class Toolbar : public GUIElement
{
public:
    Toolbar(const HWND &_Window);
    ~Toolbar();

public:
    // Overrides.
    void ProcessingData(const BitmapPtr &_Image) override final;
    void CleanPanel() override final;
    POINT GetPositionPanel() const override final;

public:
    enum class Controller : int {
        LOAD_FILE_BUTTON    = WM_USER + 1,
        CLEAN_PANELS_BUTTON = WM_USER + 2
    };

private:
    void CreateButton(HWND &_Button, HBITMAP &_Logo,
                      const wchar_t *_LogoFilePath, const int _X,
                      const int _Y, const HWND _Window, 
                      const HINSTANCE _Instance, const HMENU _ControllerID);

private:
    HWND m_LoadFileButton;
    HWND m_CleanPanelsButton;
    HBITMAP m_LogoLoadFile;
    HBITMAP m_LogoCleanPanels;
};