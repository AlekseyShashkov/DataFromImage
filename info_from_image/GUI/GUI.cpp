#include "GUI.h"

GUI::~GUI() noexcept
{
    m_listGUIElements.clear();
}

void GUI::AddElement(const HWND &_hWnd, ElementGUIPtr _elementGUIPtr) noexcept
{
    m_listGUIElements.emplace_back(std::move(_elementGUIPtr));
}

void GUI::DrawElements(HDC &_hDC, const HWND &_hWnd) noexcept
{
    if (!m_listGUIElements.empty()) {
        RECT lv_rect{};
        ::GetClientRect(_hWnd, &lv_rect);

        for (auto &i : m_listGUIElements) {
            ::BitBlt(_hDC,
                i->GetPositionPanel().x, i->GetPositionPanel().y,
                lv_rect.right, lv_rect.bottom,
                i->GetHandlePanel(),
                0, 0,
                SRCCOPY);
        }
    }
}

void GUI::TakeImage(BitmapPtr _image) noexcept
{
    if (!m_listGUIElements.empty()) {
        for (auto &i : m_listGUIElements) {
            i->ProcessingData(_image);
        }
    }
}