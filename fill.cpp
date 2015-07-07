// fill.cpp : 
//
/*
  Loop based and multi-thread implemetations for filling box
  Loop based in 5x-20x faster
*/

#include "stdafx.h"
#include <memory.h>
#include <list>
#include <set>
#include <unordered_set>
#include <mutex>
#include <iostream>
#include <Windows.h>
#include <thread>

typedef unsigned short uint16;
uint16 WIDTH = 100;
uint16 HEIGHT = 100;

typedef unsigned char byte;

template<class T>
class Stack_Set
{
	typedef std::unordered_set<T> _Stack;

	_Stack _stack;

public:
	void Pop()
	{
		_stack.erase(_stack.begin());
	}

	T Get() const
	{
		return *_stack.begin();
	}

	void Push(const T& v)
	{
		_stack.insert(v);
	}

	void Clear()
	{
		_stack.clear();
	}

	bool IsEmpty() const
	{
		return _stack.empty();
	}

	size_t Size() const
	{
		return _stack.size();
	}
};

struct Chunk
{
	uint16 x, y;

	Chunk(uint16 _X = 0, uint16 _Y = 0)
		: x(_X), y(_Y)
	{}

	bool operator==(const Chunk& _Right) const
	{
		return x == _Right.x && y == _Right.y;
	}
};

template<>
struct std::hash<Chunk>
{
	size_t operator()(const Chunk& _Keyval) const
	{	// hash _Keyval to size_t value by pseudorandomizing transform
		return std::hash<size_t>()(_Keyval.y * WIDTH + _Keyval.x);
	}
};

byte GetColor(byte* F, uint16 X, uint16 Y)
{
	if (X >= WIDTH || Y >= HEIGHT)
		throw std::out_of_range("invalid coords");

	return *(F + Y * WIDTH + X);
}

void SetColor(byte* F, uint16 X, uint16 Y, byte C)
{
	if (X >= WIDTH || Y >= HEIGHT)
		throw std::out_of_range("invalid coords");

	*(F + Y * WIDTH + X) = C;
}

void fill_loop(byte* FIELD, uint16 X, uint16 Y, byte C)
{
	Stack_Set<Chunk> fillstack;
	fillstack.Push(Chunk(X, Y));

//	size_t nMaxStackSize(0);

	while (!fillstack.IsEmpty())
	{
		Chunk c = fillstack.Get();
		fillstack.Pop();

//		Chunk c = fillstack.GetPop();

		if (GetColor(FIELD, c.x, c.y) != C)
			SetColor(FIELD, c.x, c.y, C);

		if (c.y - 1 >= 0)
		{
			if (GetColor(FIELD, c.x, c.y - 1) != C)
				fillstack.Push(Chunk(c.x, c.y - 1));
		}

		if (c.y + 1 < HEIGHT)
		{
			if (GetColor(FIELD, c.x, c.y + 1) != C)
				fillstack.Push(Chunk(c.x, c.y + 1));
		}

		if (c.x - 1 >= 0)
		{
			if (GetColor(FIELD, c.x - 1, c.y) != C)
				fillstack.Push(Chunk(c.x - 1, c.y));
// 			if (c.y - 1 >= 0)
// 				fillstack.push_back(Chunk(c.x - 1, c.y - 1));
// 
// 			if (c.y + 1 < HEIGHT)
// 				fillstack.push_back(Chunk(c.x - 1, c.y + 1));
		}

		if (c.x + 1 < WIDTH)
		{
			if (GetColor(FIELD, c.x + 1, c.y) != C)
				fillstack.Push(Chunk(c.x + 1, c.y));
// 			if (c.y - 1 >= 0)
// 				fillstack.push_back(Chunk(c.x + 1, c.y - 1));
// 
// 			if (c.y + 1 < HEIGHT)
// 				fillstack.push_back(Chunk(c.x + 1, c.y + 1));
		}

//		nMaxStackSize = max(nMaxStackSize, fillstack.Size());
	}

//	std::cout << "max stack size: " << nMaxStackSize << std::endl;
}

struct Field
{
	byte* FIELD;

//	std::mutex FIELD_MUTEX;

	Field(byte* F)
		: FIELD(F)
	{}

	Field(const Field& f)
		: FIELD(f.FIELD)
	{}

	byte get(uint16 x, uint16 y)
	{
	//	std::lock_guard<std::mutex> lg(FIELD_MUTEX);
		return GetColor(FIELD, x, y);
	}

	void set(uint16 x, uint16 y, byte c)
	{
		//std::lock_guard<std::mutex> lg(FIELD_MUTEX);
		SetColor(FIELD, x, y, c);
	}
};

struct Quad
{
	uint16 x0, y0, x1, y1;
	Field& FIELD;

	Quad(const Quad& q)
		: x0(q.x0)
		, x1(q.x1)
		, y0(q.y0)
		, y1(q.y1)
		, FIELD(q.FIELD)
	{}

	Quad(Field& F, uint16 X0, uint16 Y0, uint16 X1, uint16 Y1)
		: x0(X0)
		, x1(X1)
		, y0(Y0)
		, y1(Y1)
		, FIELD(F)
	{}
};

template<class T>
class Stack_Set_M
{
	typedef std::unordered_set<T> _Stack;

	_Stack _stack;

	mutable std::mutex MUTEX;

public:
	Stack_Set_M(const Stack_Set_M&) = delete;
	Stack_Set_M() {}

	T GetPop()
	{
		std::lock_guard<std::mutex> lg(MUTEX);
		if (!_stack.empty())
		{
			T ret = *_stack.begin();
			_stack.erase(_stack.begin());
			return ret;
		}

		return T();
	}

	void Push(const T& v)
	{
		std::lock_guard<std::mutex> lg(MUTEX);
		_stack.insert(v);
	}

	void Clear()
	{
		std::lock_guard<std::mutex> lg(MUTEX);
		_stack.clear();
	}

	bool IsEmpty() const
	{
		std::lock_guard<std::mutex> lg(MUTEX);
		return _stack.empty();
	}

	size_t Size() const
	{
		std::lock_guard<std::mutex> lg(MUTEX);
		return _stack.size();
	}
};

Stack_Set_M<Chunk> fs_mult;

/*
void fill_inner(Quad& q, uint16 X, uint16 Y, byte C)
{
	int boundX0 = min(q.x0, q.x1);
	int boundY0 = min(q.y0, q.y1);

	int boundX1 = max(q.x0, q.x1);
	int boundY1 = max(q.y0, q.y1);

	while (!fs_mult.IsEmpty())
	{
		// 		Chunk c = fs_mult.Get();
		// 		fs_mult.Pop();

		Chunk c = fs_mult.GetPop();

		if (q.FIELD.get(X, Y) != C)
			q.FIELD.set(c.x, c.y, C);

		if (c.y - 1 >= boundY0 && q.FIELD.get(c.x, c.y - 1) != C)
		{
			fs_mult.Push(Chunk(c.x, c.y - 1));
		}

		if (c.y + 1 < boundY1 && q.FIELD.get(c.x, c.y + 1) != C)
		{
			fs_mult.Push(Chunk(c.x, c.y + 1));
		}

		if (c.x - 1 >= boundX0 && q.FIELD.get(c.x - 1, c.y) != C)
		{
			fs_mult.Push(Chunk(c.x - 1, c.y));
		}

		if (c.x + 1 < boundX1 && q.FIELD.get(c.x + 1, c.y) != C)
		{
			fs_mult.Push(Chunk(c.x + 1, c.y));
		}
	}
}

// function for single quadrant and thread func
void fill_func(Quad r, int dX, int dY, byte C)
{
	if (dX > 0)
	{
		if (dY > 0)
			fill_inner(r, (r.x1 - r.x0) / 2 + r.x0, (r.y1 - r.y0) / 2 + r.y0, C);
		else
			fill_inner(r, (r.x1 - r.x0) / 2 + r.x0, (r.y0 - r.y1) / 2 + r.y1, C);
	}
	else
	{
		if (dY > 0)
			fill_inner(r, (r.x0 - r.x1) / 2 + r.x0, (r.y1 - r.y0) / 2 + r.y0, C);
		else
			fill_inner(r, (r.x0 - r.x1) / 2 + r.x0, (r.y0 - r.y1) / 2 + r.y1, C);
	}
	std::cout << "Quadrant [" << dX << "," << dY << "] finished" << std::endl;
}
*/

// function for single quadrant and thread func
void fill_func(Quad r, int dX, int dY, byte C)
{
	for (int x = r.x0; x != r.x1 + dX; x += dX)
	{
		for (int y = r.y0; y != r.y1 + dY; y += dY)
		{
//			std::cout << "<" << std::this_thread::get_id() << "> added chunk [" << x << "," << y << "]" << std::endl;
			fs_mult.Push(Chunk(x, y));
		}
	}

	while (!fs_mult.IsEmpty())
	{
		Chunk c = fs_mult.GetPop();
//		std::cout << "<" << std::this_thread::get_id() << "> popped chunk [" << c.x << "," << c.y << "]" << std::endl;
		if (r.FIELD.get(c.x, c.y) != C)
			r.FIELD.set(c.x, c.y, C);
	}

//	std::cout << "Quadrant #" << dX << "," << dY << "# finished" << std::endl;
}

void fill_multi(byte* FIELD, uint16 X, uint16 Y, byte C)
{
	Field F(FIELD);
	// create 4 quadrants and send them to thread for filling stack
	for (int x = 0; x < WIDTH; x++)
	{
//		std::cout << "<" << std::this_thread::get_id() << "> added chunk [" << x << "," << Y << "]" << std::endl;
		fs_mult.Push(Chunk(x, Y));
	}

	for (int y = 0; y < HEIGHT; y++)
	{
//		std::cout << "<" << std::this_thread::get_id() << "> added chunk [" << X << "," << y << "]" << std::endl;
		fs_mult.Push(Chunk(X, y));
	}

	// start threads
	Quad qqq1(F, X + 1, Y + 1, WIDTH - 1, HEIGHT - 1);
	std::thread ft1(fill_func, qqq1, 1, 1, C);
//	ft1.detach();

	Quad qqq2(F, X - 1, Y + 1, 0, HEIGHT - 1);
	std::thread ft2(fill_func, qqq2, -1, 1, C);
//	ft2.detach();

	Quad qqq3(F, X - 1, Y - 1, 0, 0);
	std::thread ft3(fill_func, qqq3, -1, -1, C);
//	ft3.detach();

	Quad qqq4(F, X + 1, Y - 1, WIDTH - 1, 0);
	std::thread ft4(fill_func, qqq4, 1, -1, C);
//	ft4.detach();

	// fill
	while (!fs_mult.IsEmpty())
	{
		Chunk c = fs_mult.GetPop();
		//fs_mult.Pop();
//		std::cout << "<" << std::this_thread::get_id() << "> popped chunk [" << c.x << "," << c.y << "]" << std::endl;

		if (F.get(c.x, c.y) != C)
			F.set(c.x, c.y, C);
	}

	if (ft1.joinable())
		ft1.join();

	if (ft2.joinable())
		ft2.join();

	if (ft3.joinable())
		ft3.join();

	if (ft4.joinable())
		ft4.join();
}

bool check_field(byte* F, byte C)
{
	size_t x(WIDTH * HEIGHT);
	while (x--)
	{
		if (*(F + x) != C)
			return false;
	}

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
//	byte* FIELD = new byte[WIDTH * HEIGHT]{0}; // too slow initialization

	int ITER = 10;

	if (argc > 1)
	{
		if (argc < 4)
		{
			std::cout << "Please provide all parameters. Width Height Interactions";
			return 0;
		}

		WIDTH = _ttoi(argv[1]);
		HEIGHT = _ttoi(argv[2]);
		ITER = _ttoi(argv[3]);
	}

	byte* FIELD = new byte[WIDTH * HEIGHT];
	memset(FIELD, 0xFF, WIDTH * HEIGHT);

	LARGE_INTEGER previous;
	LARGE_INTEGER current;

	{
		QueryPerformanceCounter(&previous);
		for (byte C = 0; C < ITER; C++)
		{
			fill_loop(FIELD, WIDTH / 2, HEIGHT / 2, C);
//			std::cout << "test " << (check_field(FIELD, C) ? "passed" : "failed") << std::endl;
		}
		QueryPerformanceCounter(&current);
		std::cout << "Single thread fill " << static_cast<double>(current.QuadPart - previous.QuadPart) / ITER << std::endl;
	}

	{
		QueryPerformanceCounter(&previous);
		for (byte C = 0; C < ITER; C++)
		{
			fill_multi(FIELD, WIDTH / 2, HEIGHT / 2, C);
//			std::cout << "test " << (check_field(FIELD, C) ? "passed" : "failed") << std::endl;
		}
		QueryPerformanceCounter(&current);
		std::cout << "Multi-thread fill " << static_cast<double>(current.QuadPart - previous.QuadPart) / ITER << std::endl;
	}

	delete[] FIELD;

	return 0;
}
