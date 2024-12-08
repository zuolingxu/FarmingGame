#pragma once
#include "DocumentManager.h"
#include "cocos2d.h"
#include <sstream>
#include <iostream>
constexpr int GridSize = 16;

const cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
const cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
const cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 720);
const cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

template <class T>
class Vec
{
private:
	T x, y;
public:
	Vec() : x(T()), y(T()) {}
	~Vec() = default;
	Vec(const cocos2d::Vec2& vec) : x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y)) {}
	Vec(const cocos2d::Size& vec) : x(static_cast<T>(vec.width)), y(static_cast<T>(vec.height)) {}
	Vec(const T x,const T y) : x(x), y(y) {}
	Vec(const Vec& other) : x(other.x), y(other.y) {}
	Vec(Vec&& other) noexcept: x(std::move(other.x)), y(std::move(other.y)) {}
	Vec& operator=(const Vec& other)
	{
		if (this != &other)
		{
			x = other.x;
			y = other.y;
		}
		return *this;
	}
	Vec& operator=(Vec&& other) noexcept
	{
		if (this != &other) {
			x = std::move(other.x);
			y = std::move(other.y);
		}
		return *this;
	}

	[[nodiscard]] T X() const { return x; }
	[[nodiscard]] T Y() const { return y; }
	T& modX() { return x; }
	T& modY() { return y; }
	bool operator==(const Vec& other) const { return x == other.x && y == other.y; }
	bool operator!=(const Vec& other) const { return x != other.x || y != other.y; }
	Vec& operator+=(const Vec& other) { x += other.x; y += other.y; return *this; }
	[[nodiscard]] Vec operator+(const Vec& other) const { return Vec(x + other.x , y + other.y); }
	Vec& operator-=(const Vec& other) { x -= other.x; y -= other.y; return *this; }
	[[nodiscard]] Vec operator-(const Vec& other) const { return Vec( x - other.x , y - other.y); }
	[[nodiscard]] double length() const { return std::sqrt(x * x + y * y); }
	[[nodiscard]] double distance(const Vec& other) const {
		T dx = other.x - x;
		T dy = other.y - y;
		return std::sqrt(dx * dx + dy * dy);
	}
	friend std::ostream& operator<<(std::ostream& os, const Vec& vec)
	{
		os << vec.x << " " << vec.y;
		return os;
	}
	friend std::istream& operator>>(std::istream& in, Vec& vec)
	{
		in >> vec.x >> vec.y;
		return in;
	}
	operator cocos2d::Vec2() const { return { static_cast<float>(x), static_cast<float>(y) }; }
	operator cocos2d::Size() const { return { static_cast<float>(x), static_cast<float>(y) }; }
};

// Vec2 is the 2D vector class provided by cocos2dx
// press ctrl and click Vec2 to see class code
inline Vec<int> toGrid(const cocos2d::Vec2& vec){
	const Vec<int> vecInt(vec);
	return { vecInt.X() / GridSize, vecInt.Y() / GridSize };
}

template<typename T>
inline Vec<int> toGrid(const Vec<T>& vec) {
	const Vec<int> vecInt = { static_cast<int>(vec.x), static_cast<int>(vec.y) };
	return { vecInt.X() / GridSize, vecInt.Y() / GridSize };
}

inline cocos2d::Vec2 toPixel(const Vec<int>& vec)
{
	return Vec<int>{vec.X() * GridSize, vec.Y() * GridSize};
}


// Get Vec2 from string in format "x y"
inline Vec<int> toVec2(const std::string& str)
{
	Vec<int> vec;
	std::istringstream iss(str);
	iss >> vec;
	return vec;
}

inline std::string toString(const Vec<int>& vec)
{
	std::ostringstream oss;
	oss << vec;
	return oss.str();
}

inline std::string getFrameName(std::string frame_format, int i)
{
	return frame_format.replace(frame_format.find("{}"), 2, std::to_string(i));
}