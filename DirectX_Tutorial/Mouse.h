#pragma once
#include <queue>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	 public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WPress,
			WRelease,
			WheelDown,
			WheelUp,
			Move,
			Enter,
			Leave,
			Invalid
		};
	 private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		bool scrollIsPressed;
		int x, y;
	 public:
		Event()noexcept:type{Type::Invalid}, leftIsPressed{false},
			rightIsPressed{ false }, scrollIsPressed{false}, x{ 0 }, y{ 0 } {}
		Event(Type type, const Mouse& parent)noexcept
		{
			this->type = type;
			leftIsPressed = parent.leftIsPressed;
			rightIsPressed = parent.rightIsPressed;
			scrollIsPressed = parent.scrollIsPressed;
		}

		bool IsVald()const noexcept
		{
			return type != Type::Invalid;
		}

		Type getType()const noexcept { return type; }
		std::pair<int, int> getPos()const noexcept { return { x,y }; }
		int getXpos()const noexcept { return x; }
		int getYpos()const noexcept { return y; }
		bool LeftIsPressed()const noexcept { return LeftIsPressed; }
		bool RightIsPressed()const noexcept { return RightIsPressed; }
		bool ScrollIsPressed()const noexcept { return ScrollIsPressed; }
	};

public:
	Mouse() = default;
	Mouse(const Mouse& copy) = delete;
	Mouse& operator =(const Mouse& copy) = delete;

	std::pair<int, int> GetPos()const noexcept;
	int GetXpos() const noexcept;
	int GetYpos()const noexcept;
	// Check If the cursor is in the correct area
	bool IsInWindow()const noexcept;
	bool LeftIsPressed()const noexcept;
	bool RightIsPressed()const noexcept;
	bool ScrollIsPressed()const noexcept;
	Mouse::Event Read()noexcept;
	inline bool IsEmpty()const noexcept
	{
		return buffer.empty();
	}
	void Flush()noexcept;

private:
	// Methods
	void OnMouseMove(int x, int y)noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;

	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnScrollPressed(int x, int y) noexcept;
	void OnScrollReleased(int x, int y) noexcept;

	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer()noexcept;
	void OnWheelDelta(int x,int y,int delta)noexcept;
private:
	//Attributes
	static constexpr uint32_t bufferSize = 16u;
	int x, y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool scrollIsPressed = false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};

