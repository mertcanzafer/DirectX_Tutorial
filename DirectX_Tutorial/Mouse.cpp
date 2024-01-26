#include "Mouse.h"
#include "FixedWin.h"

std::pair<int, int> Mouse::GetPos() const noexcept
{
    return { x,y };
}

int Mouse::GetXpos() const noexcept
{
    return x;
}

int Mouse::GetYpos() const noexcept
{
    return y;
}

bool Mouse::IsInWindow() const noexcept
{
    return isInWindow;
}

bool Mouse::LeftIsPressed() const noexcept
{
    return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
    return rightIsPressed;
}

bool Mouse::ScrollIsPressed() const noexcept
{
    return scrollIsPressed;
}

Mouse::Event Mouse::Read() noexcept
{
    if (buffer.size() > 0u)
    {
        Mouse::Event e = buffer.front();
        buffer.pop();
        return e;
    }
    else {
        return Mouse:: Event();
    }
}

void Mouse::Flush() noexcept
{
    buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int x, int y) noexcept
{
    this->x = x;
    this->y = y;

    buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
    isInWindow = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
    TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
    isInWindow = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
    TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
    leftIsPressed = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
    TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
    leftIsPressed = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
    TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
    rightIsPressed = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
    TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
    rightIsPressed = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
    TrimBuffer();
}

void Mouse::OnScrollPressed(int x, int y) noexcept
{
    scrollIsPressed = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::WPress,*this));
    TrimBuffer();
}

void Mouse::OnScrollReleased(int x, int y) noexcept
{
    scrollIsPressed = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::WRelease, *this));
    TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
    buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
    TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
    buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
    TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
    while (bufferSize < buffer.size())
    {
        buffer.pop();
    }
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
    // Wheel Delta Factor
    wheelDeltaCarry += delta;
    // generate events for every 120 
    while (wheelDeltaCarry >= WHEEL_DELTA)
    {
        wheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp(x, y);
    }
    while (wheelDeltaCarry <= -WHEEL_DELTA)
    {
        wheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown(x, y);
    }
}






