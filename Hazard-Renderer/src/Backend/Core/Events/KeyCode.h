#pragma once

#include <iostream>
#include <UtilityCore.h>

namespace Key
{

	using KeyCode = uint16_t;

	enum : KeyCode
	{
		None = 0,
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		NumPad0 = 320,
		NumPad1 = 321,
		NumPad2 = 322,
		NumPad3 = 323,
		NumPad4 = 324,
		NumPad5 = 325,
		NumPad6 = 326,
		NumPad7 = 327,
		NumPad8 = 328,
		NumPad9 = 329,
		NumPadDecimal = 330,
		NumPadDivide = 331,
		NumPadMultiply = 332,
		NumPadSubtract = 333,
		NumPadAdd = 334,
		NumPadEnter = 335,
		NumPadEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	};

	static const char* ToString(KeyCode key)
	{
		switch (key)
		{
			TO_STRING(None);
			TO_STRING(Space);
			TO_STRING(Apostrophe);
			TO_STRING(Comma);
			TO_STRING(Minus);
			TO_STRING(Period);
			TO_STRING(Slash);

			case D0: return "0";
			case D1: return "1";
			case D2: return "2";
			case D3: return "3";
			case D4: return "4";
			case D5: return "5";
			case D6: return "6";
			case D7: return "7";
			case D8: return "8";
			case D9: return "9";

				TO_STRING(Semicolon);
				TO_STRING(Equal);

				TO_STRING(A);
				TO_STRING(B);
				TO_STRING(C);
				TO_STRING(D);
				TO_STRING(E);
				TO_STRING(F);
				TO_STRING(G);
				TO_STRING(H);
				TO_STRING(I);
				TO_STRING(J);
				TO_STRING(K);
				TO_STRING(L);
				TO_STRING(M);
				TO_STRING(N);
				TO_STRING(O);
				TO_STRING(P);
				TO_STRING(Q);
				TO_STRING(R);
				TO_STRING(S);
				TO_STRING(T);
				TO_STRING(U);
				TO_STRING(V);
				TO_STRING(W);
				TO_STRING(X);
				TO_STRING(Y);
				TO_STRING(Z);

				TO_STRING(LeftBracket);
				TO_STRING(Backslash);
				TO_STRING(RightBracket);
				TO_STRING(GraveAccent);

				TO_STRING(World1);
				TO_STRING(World2);
				TO_STRING(Escape);
				TO_STRING(Enter);
				TO_STRING(Tab);
				TO_STRING(Backspace);
				TO_STRING(Insert);
				TO_STRING(Delete);
				TO_STRING(Right);
				TO_STRING(Left);
				TO_STRING(Down);
				TO_STRING(Up);
				TO_STRING(PageUp);
				TO_STRING(PageDown);
				TO_STRING(Home);
				TO_STRING(End);
				TO_STRING(CapsLock);
				TO_STRING(ScrollLock);
				TO_STRING(NumLock);
				TO_STRING(PrintScreen);
				TO_STRING(Pause);
				TO_STRING(F1);
				TO_STRING(F2);
				TO_STRING(F3);
				TO_STRING(F4);
				TO_STRING(F5);
				TO_STRING(F6);
				TO_STRING(F7);
				TO_STRING(F8);
				TO_STRING(F9);
				TO_STRING(F10);
				TO_STRING(F11);
				TO_STRING(F12);
				TO_STRING(F13);
				TO_STRING(F14);
				TO_STRING(F15);
				TO_STRING(F16);
				TO_STRING(F17);
				TO_STRING(F18);
				TO_STRING(F19);
				TO_STRING(F20);
				TO_STRING(F21);
				TO_STRING(F22);
				TO_STRING(F23);
				TO_STRING(F24);
				TO_STRING(F25);


				TO_STRING(NumPad0);
				TO_STRING(NumPad1);
				TO_STRING(NumPad2);
				TO_STRING(NumPad3);
				TO_STRING(NumPad4);
				TO_STRING(NumPad5);
				TO_STRING(NumPad6);
				TO_STRING(NumPad7);
				TO_STRING(NumPad8);
				TO_STRING(NumPad9);
				TO_STRING(NumPadDecimal);
				TO_STRING(NumPadDivide);
				TO_STRING(NumPadMultiply);
				TO_STRING(NumPadSubtract);
				TO_STRING(NumPadAdd);
				TO_STRING(NumPadEnter);
				TO_STRING(NumPadEqual);

				TO_STRING(LeftShift);
				TO_STRING(LeftControl);
				TO_STRING(LeftAlt);
				TO_STRING(LeftSuper);
				TO_STRING(RightShift);
				TO_STRING(RightControl);
				TO_STRING(RightAlt);
				TO_STRING(RightSuper);
				TO_STRING(Menu);
		}
	}
}
