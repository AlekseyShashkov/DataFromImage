#include "GUI.h"

GUI::~GUI()
{
    m_GUIElementsList.clear();
}

void GUI::AddElement(GUIElementPtr _GUIElementPtr)
{
    m_GUIElementsList.emplace_back(std::move(_GUIElementPtr));
}

void GUI::DrawElements(HDC &_DC, const HWND &_Window)
{
    if (!m_GUIElementsList.empty()) {
        RECT lv_Rect;
        ::GetClientRect(_Window, &lv_Rect);

        for (const auto &i : m_GUIElementsList) {
            ::BitBlt(_DC, i->GetPositionPanel().x, i->GetPositionPanel().y,
                     lv_Rect.right, lv_Rect.bottom, i->GetHandlePanel(),
                     0, 0, SRCCOPY);      
        }
    }
}

void GUI::TakeImage(const BitmapPtr &_BitmapPtr)
{
    if (!m_GUIElementsList.empty()) {
        for (auto &i : m_GUIElementsList) {
            i->ProcessingData(_BitmapPtr);
        }
    }
}

void GUI::CleanPanels()
{
    if (!m_GUIElementsList.empty()) {
        for (const auto &i : m_GUIElementsList) {
            i->CleanPanel();
        }
    }
}