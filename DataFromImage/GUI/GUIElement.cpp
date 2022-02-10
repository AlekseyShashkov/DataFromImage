#include "GUIElement.h"

GUIElement::~GUIElement()
{
    ::SelectObject(m_PanelDC, m_PanelBitmapDefault);
    ::SelectObject(m_PanelBasicDC, m_PanelBasicBitmapDefault);  
    ::DeleteObject(m_PanelBitmap);  
    ::DeleteObject(m_PanelBasicBitmap);
    ::DeleteDC(m_PanelDC);
    ::DeleteDC(m_PanelBasicDC);
}

HDC GUIElement::GetHandlePanel() const
{
    return m_PanelDC;
}