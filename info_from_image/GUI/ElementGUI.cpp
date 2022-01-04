#include "ElementGUI.h"

ElementGUI::ElementGUI() noexcept
    : m_handlePanel{}, m_memoryPanel{}, m_positionPanel{}
{

}

ElementGUI::~ElementGUI() noexcept
{
    ::DeleteDC(m_handlePanel);
    ::DeleteObject(m_memoryPanel);
}

HDC ElementGUI::GetHandlePanel() const noexcept
{
    return m_handlePanel;
}