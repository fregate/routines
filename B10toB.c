// according to ANSI C standart this usage of union is not recommended... but its beautiful )
// base to base number conversion without any additional variables and memory allocation... wonderful isn't it?

struct toO
{
  union {
		unsigned __int64 x;
		struct B64 { // 10x6 + 1x4 = 64
			unsigned __int64 o00:6;
			unsigned __int64 o01:6;
			unsigned __int64 o02:6;
			unsigned __int64 o03:6;
			unsigned __int64 o04:6;
			unsigned __int64 o05:6;
			unsigned __int64 o06:6;
			unsigned __int64 o07:6;
			unsigned __int64 o08:6;
			unsigned __int64 o09:6;
			unsigned __int64 o10:4;
		} b64;

		struct B32 { // 12x5 + 1x4 = 64
			unsigned __int64 o00:5;
			unsigned __int64 o01:5;
			unsigned __int64 o02:5;
			unsigned __int64 o03:5;
			unsigned __int64 o04:5;
			unsigned __int64 o05:5;
			unsigned __int64 o06:5;
			unsigned __int64 o07:5;
			unsigned __int64 o08:5;
			unsigned __int64 o09:5;
			unsigned __int64 o10:5;
			unsigned __int64 o11:5;
			unsigned __int64 o12:4;
		} b32;

		struct B16 { // 16x4 = 64
			unsigned __int64 o00:4;
			unsigned __int64 o01:4;
			unsigned __int64 o02:4;
			unsigned __int64 o03:4;
			unsigned __int64 o04:4;
			unsigned __int64 o05:4;
			unsigned __int64 o06:4;
			unsigned __int64 o07:4;
			unsigned __int64 o08:4;
			unsigned __int64 o09:4;
			unsigned __int64 o10:4;
			unsigned __int64 o11:4;
			unsigned __int64 o12:4;
			unsigned __int64 o13:4;
			unsigned __int64 o14:4;
			unsigned __int64 o15:4;
		} b16;

		struct B8 { // 21x3 + 1x1 = 64
			unsigned __int64 o00:3;
			unsigned __int64 o01:3;
			unsigned __int64 o02:3;
			unsigned __int64 o03:3;
			unsigned __int64 o04:3;
			unsigned __int64 o05:3;
			unsigned __int64 o06:3;
			unsigned __int64 o07:3;
			unsigned __int64 o08:3;
			unsigned __int64 o09:3;
			unsigned __int64 o10:3;
			unsigned __int64 o11:3;
			unsigned __int64 o12:3;
			unsigned __int64 o13:3;
			unsigned __int64 o14:3;
			unsigned __int64 o15:3;
			unsigned __int64 o16:3;
			unsigned __int64 o17:3;
			unsigned __int64 o18:3;
			unsigned __int64 o19:3;
			unsigned __int64 o20:3;
			unsigned __int64 o21:1;
		} b8;
	};
};

int main(int argc, char* argv[])
{
	int s = sizeof(toO);
	toO X;
	X.x = 0x0123456701234567;
	
	X.b64.o00 = 0x3F; // or 0xFF
	X.b64.o01 = 0x3F;
	X.b64.o02 = 0x3F;
	X.b64.o03 = 0x3F;
	X.b64.o04 = 0x3F;
	X.b64.o05 = 0x3F;
	X.b64.o06 = 0x3F;
	X.b64.o07 = 0x3F;
	X.b64.o08 = 0x3F;
	X.b64.o09 = 0x3F;
	X.b64.o10 = 0x3F;
	
	// X.x == MAX_UINT64
}
