#include "Keyboard.h"

bool Keyboard::KeyIsPressed(uint8_t keyCode) const noexcept
{
	return keyStates[keyCode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (keyBuffer.size() > 0u)
	{
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	else 
	{
		Keyboard::Event();
	}
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return keyBuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
	keyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept
{
	if (charBuffer.size() > 0u)
	{
		uint8_t charCode = charBuffer.front();
		charBuffer.pop();
		return charCode;
	}
	else 
	{
		return 0;
	}
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return charBuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
	charBuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	FlushKey(),FlushKey();
}

void Keyboard::EnableAutoRepeat() noexcept
{
	AutoReleatEnabled = true;
}

void Keyboard::DisableAutoRepeat() noexcept
{
	AutoReleatEnabled = false;
}

bool Keyboard::AutoRepeatIsEnabled() const noexcept
{
	return AutoReleatEnabled;
}

// window class stuff
void Keyboard::OnKeyPressed(uint8_t keyCode) noexcept
{
	keyStates[keyCode] = true;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press,keyCode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(uint8_t keyCode) noexcept
{
	keyStates[keyCode] = false;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keyCode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(char character) noexcept
{
	charBuffer.push(character);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept
{
	keyStates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}