#include "Input.h"
#include "../window/Window.h"

Input::Input(Window* window) : 
    // We keep the window pointer for potential future use
    m_window(window),
    m_mouseX(0),
    m_mouseY(0),
    m_prevMouseX(0),
    m_prevMouseY(0),
    m_mouseDeltaX(0),
    m_mouseDeltaY(0),
    m_mouseScrollDelta(0.0f)
{
    // Initialize arrays to false
    std::fill(m_keysDown.begin(), m_keysDown.end(), false);
    std::fill(m_prevKeysDown.begin(), m_prevKeysDown.end(), false);
    std::fill(m_mouseButtonsDown.begin(), m_mouseButtonsDown.end(), false);
    std::fill(m_prevMouseButtonsDown.begin(), m_prevMouseButtonsDown.end(), false);

    // Get initial mouse position (using window size center as default)
    if (window) {
        m_mouseX = window->GetWidth() / 2;
        m_mouseY = window->GetHeight() / 2;
        m_prevMouseX = m_mouseX;
        m_prevMouseY = m_mouseY;
    }
}

Input::~Input()
{
}

void Input::Update()
{
    // Store previous states
    m_prevKeysDown = m_keysDown;
    m_prevMouseButtonsDown = m_mouseButtonsDown;
    
    // Update mouse delta
    m_mouseDeltaX = m_mouseX - m_prevMouseX;
    m_mouseDeltaY = m_mouseY - m_prevMouseY;
    
    // Store current mouse position as previous for next frame
    m_prevMouseX = m_mouseX;
    m_prevMouseY = m_mouseY;
    
    // Reset scroll delta (it's a one-frame event)
    m_mouseScrollDelta = 0.0f;
}

bool Input::IsKeyDown(int keyCode) const
{
    int index = KeyToIndex(keyCode);
    return (index >= 0 && index < MAX_KEYS) ? m_keysDown[index] : false;
}

bool Input::IsKeyUp(int keyCode) const
{
    return !IsKeyDown(keyCode);
}

bool Input::IsKeyPressed(int keyCode) const
{
    int index = KeyToIndex(keyCode);
    return (index >= 0 && index < MAX_KEYS) ? (m_keysDown[index] && !m_prevKeysDown[index]) : false;
}

bool Input::IsKeyReleased(int keyCode) const
{
    int index = KeyToIndex(keyCode);
    return (index >= 0 && index < MAX_KEYS) ? (!m_keysDown[index] && m_prevKeysDown[index]) : false;
}

bool Input::IsMouseButtonDown(MouseButton button) const
{
    size_t index = static_cast<size_t>(button);
    return (index < static_cast<size_t>(MouseButton::Count)) ? m_mouseButtonsDown[index] : false;
}

bool Input::IsMouseButtonUp(MouseButton button) const
{
    return !IsMouseButtonDown(button);
}

bool Input::IsMouseButtonPressed(MouseButton button) const
{
    size_t index = static_cast<size_t>(button);
    return (index < static_cast<size_t>(MouseButton::Count)) ? 
        (m_mouseButtonsDown[index] && !m_prevMouseButtonsDown[index]) : false;
}

bool Input::IsMouseButtonReleased(MouseButton button) const
{
    size_t index = static_cast<size_t>(button);
    return (index < static_cast<size_t>(MouseButton::Count)) ? 
        (!m_mouseButtonsDown[index] && m_prevMouseButtonsDown[index]) : false;
}

void Input::OnKeyEvent(int keyCode, bool pressed)
{
    int index = KeyToIndex(keyCode);
    if (index >= 0 && index < MAX_KEYS)
    {
        m_keysDown[index] = pressed;
    }
}

void Input::OnMouseButtonEvent(int button, bool pressed)
{
    MouseButton mouseButton;
    switch (button)
    {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        mouseButton = MouseButton::Left;
        break;
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
        mouseButton = MouseButton::Middle;
        break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        mouseButton = MouseButton::Right;
        break;
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
        // X1 or X2 button determined by high word of wParam
        mouseButton = (GET_XBUTTON_WPARAM(button) == XBUTTON1) ? MouseButton::X1 : MouseButton::X2;
        break;
    default:
        return; // Unrecognized button
    }

    size_t index = static_cast<size_t>(mouseButton);
    if (index < static_cast<size_t>(MouseButton::Count))
    {
        m_mouseButtonsDown[index] = pressed;
    }
}

void Input::OnMouseMoveEvent(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
}

void Input::OnMouseScrollEvent(float delta)
{
    m_mouseScrollDelta = delta;
}

int Input::KeyToIndex(int keyCode) const
{
    // Simple mapping for Windows virtual key codes
    // This handles most common keys directly
    return keyCode & 0xFF;
}