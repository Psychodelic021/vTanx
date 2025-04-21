#pragma once

#include <windows.h>
#include <array>
#include <unordered_map>

// Forward declaration
class Window;

// Define mouse buttons
enum class MouseButton {
    Left,
    Middle,
    Right,
    X1,
    X2,
    Count
};

// Input system for handling keyboard and mouse input
class Input {
public:
    Input(Window* window);
    ~Input();

    void Update();

    // Keyboard state methods
    bool IsKeyDown(int keyCode) const;
    bool IsKeyUp(int keyCode) const;
    bool IsKeyPressed(int keyCode) const;  // Key was pressed this frame
    bool IsKeyReleased(int keyCode) const; // Key was released this frame

    // Mouse state methods
    bool IsMouseButtonDown(MouseButton button) const;
    bool IsMouseButtonUp(MouseButton button) const;
    bool IsMouseButtonPressed(MouseButton button) const;  // Button was pressed this frame
    bool IsMouseButtonReleased(MouseButton button) const; // Button was released this frame

    // Mouse position methods
    int GetMouseX() const { return m_mouseX; }
    int GetMouseY() const { return m_mouseY; }
    int GetMouseDeltaX() const { return m_mouseDeltaX; }
    int GetMouseDeltaY() const { return m_mouseDeltaY; }
    float GetMouseScrollDelta() const { return m_mouseScrollDelta; }
    
    // Get associated window
    Window* GetWindow() const { return m_window; }

    // Window event handlers
    void OnKeyEvent(int keyCode, bool pressed);
    void OnMouseButtonEvent(int button, bool pressed);
    void OnMouseMoveEvent(int x, int y);
    void OnMouseScrollEvent(float delta);

private:
    // Map virtual key code to array index
    int KeyToIndex(int keyCode) const;

private:
    Window* m_window;

    // Keyboard state tracking (current and previous frame)
    static const int MAX_KEYS = 256;
    std::array<bool, MAX_KEYS> m_keysDown;
    std::array<bool, MAX_KEYS> m_prevKeysDown;

    // Mouse button state tracking
    std::array<bool, static_cast<size_t>(MouseButton::Count)> m_mouseButtonsDown;
    std::array<bool, static_cast<size_t>(MouseButton::Count)> m_prevMouseButtonsDown;

    // Mouse position
    int m_mouseX;
    int m_mouseY;
    int m_prevMouseX;
    int m_prevMouseY;
    int m_mouseDeltaX;
    int m_mouseDeltaY;
    float m_mouseScrollDelta;
};