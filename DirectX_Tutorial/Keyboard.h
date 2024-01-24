#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		uint8_t code;
	public:
		Event()noexcept :type{ Type::Invalid }, code{0u} {}
		Event(Type type,uint8_t code) noexcept: type{type},code{code}{}

		bool IsPress()const noexcept { return type == Type::Press; }
		bool IsRelease()const noexcept { return type == Type::Release; }
		bool IsValid()const noexcept { return type == Type::Invalid; }

		uint8_t getCode()const noexcept { return code; }
	};
public:
	Keyboard() = default;
	// We won't copy the keyboard class
	Keyboard(const Keyboard& copy) = delete;
	Keyboard& operator = (const Keyboard& copy) = delete;
	// key event stuff
	bool KeyIsPressed(uint8_t keyCode)const noexcept;
	Event ReadKey()noexcept;
	bool KeyIsEmpty()const noexcept;
	void FlushKey()noexcept; // Just Clear the queue
	// char event stuff
	char ReadChar()noexcept;
	bool CharIsEmpty()const noexcept;
	void FlushChar()noexcept; // Just Clear the queue
	void Flush()noexcept; // Clear both char and key queues
	// autorepeat control mech
	void EnableAutoRepeat()noexcept;
	void DisableAutoRepeat()noexcept;
	bool AutoRepeatIsEnabled()const noexcept;
private:
	void OnKeyPressed(uint8_t keyCode) noexcept;
	void OnKeyReleased(uint8_t keyCode)noexcept;

	void OnChar(char character)noexcept;
	void ClearState()noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer);

private:
	static constexpr uint32_t nKeys = 256u;
	static constexpr uint32_t bufferSize = 16u;
	bool AutoReleatEnabled = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};

