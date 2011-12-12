#ifdef __GNUC__
	//gcc
	#define MY_ASSERT(f) \
		if (!(f) ) \
		{ \
			asm ("int $3") ; \
		} 
#else
	#include <windows.h>
	#include <crtdbg.h>
	//msvc
	#define MY_ASSERT(f) \
		if (!(f) ) \
		{ \
			MSG msg;	\
			BOOL bQuit = PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);	\
			if (bQuit)	PostQuitMessage((int)msg.wParam);	\
			DebugBreak(); \
		} 
#endif

#include <iostream>
#include "xreturn.h"





xreturn::r<int> fizzbuzz(int a)
{
	if (((a % 3) == 0) && ((a % 5) == 0))
	{
		return xreturn::error("3と5で割り切れますがな",35);
	}
	if ((a % 3) == 0)
	{
		return xreturn::error("3で割り切れますがな",3);
	}
	if ((a % 5) == 0)
	{
		return xreturn::error("5で割り切れますがな",5);
	}
	return a;
}

void test1()
{
	std::cout << "=== test1 === ふつーに使ってみよう " << std::endl;
	//fizzbuzz の変化形に出会うとエラーになります。
	//それ以外は入力した値が帰ります。
	int a1 = fizzbuzz(1);
	std::cout << a1 << std::endl;	//1が帰る

	int a2 = fizzbuzz(2);
	std::cout << a2 << std::endl;	//2が帰る

	try
	{
		int a3 = fizzbuzz(3);
		std::cout << a1 << std::endl;	//ココには来ない.
										//3はfizz でエラーになるので int にキャストした瞬間に殺される。
		MY_ASSERT(0);
	}
	catch(xreturn::error &e )
	{
		std::cout << e.what() << std::endl;	//エラーが表示される
	}

	//エラー判別をもうちょっと楽に行うことも出来ます。
	auto b1 = fizzbuzz(1);
	std::cout << b1 << std::endl;	//1が帰る

	auto b2 = fizzbuzz(2);
	std::cout << static_cast<int>(b2) << std::endl;	//2が帰る  明確的にキャストしてみる.

	auto b3 = fizzbuzz(3);
	if (!b3) //エラーかどうか楽に判別できます。
	{
		std::cout << "エラーメッセージ " << b3.getErrorMessage() << " エラーコード "  << b3.getErrorCode() << std::endl;	//エラー
	}
	else
	{
		MY_ASSERT(0);
	}


	//エラーを自前でexceptionに変えて飛ばせたりもします。
	auto c3 = fizzbuzz(3);
	if (!c3)
	{
		try
		{
			//保持しているエラーを exceptionに変換して飛ばす.
			c3.throwException();
			MY_ASSERT(0);
		}
		catch(xreturn::error &e)
		{
			std::cout << e.what() << std::endl;	//エラーが表示される
		}
	}
	else
	{
		MY_ASSERT(0);
	}


	//エラーをチェックしないずぼらな人も安心して使えます。

	//エラーじゃないので無視してもOK
	fizzbuzz(1);
	//エラーじゃないので無視してもOK
	fizzbuzz(2);

	//エラー
	try
	{
		//fizzなのでエラーです。無視すると殺します.
		fizzbuzz(3);
		MY_ASSERT(0);
	}
	catch(xreturn::error &e)
	{
		std::cout << e.what() << std::endl;	//エラーが表示される
	}




	//明示的にチェックする場合
	//.check() を呼ぶと、エラーだったら、例外を投げて、そうでなければ、値を返してくれます。
	//ワンライナーにオススメです。
	int d1 = fizzbuzz(1).check();
	std::cout << d1 << std::endl;	//trueが帰る

	int d2 = fizzbuzz(2).check();
	std::cout << d2 << std::endl;	//trueが帰る

	try
	{
		int d3 = fizzbuzz(3).check();
		std::cout << d3 << std::endl;	//ココには来ない.
										//3はfizz でエラーになるので bool にキャストした瞬間に殺される。
		MY_ASSERT(0);
	}
	catch(xreturn::error &e )
	{
		std::cout << e.what() << std::endl;	//エラーが表示される
	}
}


//エラーを型で表現することもできます。 C++に不可能はありません。
class error_fizz : public xreturn::error
{
public:
	error_fizz(){}
	error_fizz(const std::string & message , int code) : xreturn::error(message,code){}
	virtual ~error_fizz(){}
};
class error_buzz : public xreturn::error
{
public:
	error_buzz(){}
	error_buzz(const std::string & message , int code) : xreturn::error(message,code){}
	virtual ~error_buzz(){}
};
class error_fizzbuzz : public xreturn::error
{
public:
	error_fizzbuzz(){}
	error_fizzbuzz(const std::string & message , int code) : xreturn::error(message,code){}
	virtual ~error_fizzbuzz(){}
};

xreturn::r<int,error_fizz,error_buzz,error_fizzbuzz> fizzbuzz_for_type(int a)
{
	if (((a % 3) == 0) && ((a % 5) == 0))
	{
		return error_fizzbuzz("3と5で割り切れますがな",35);
	}
	if ((a % 3) == 0)
	{
		return error_fizz("3で割り切れますがな",3);
	}
	if ((a % 5) == 0)
	{
		return error_buzz("5で割り切れますがな",5);
	}
	return a;
}


void test2()
{
	std::cout << "=== test2 === 型でエラーを返してみよう " << std::endl;

	//型でエラーを返すのでこんなこともできます。
	auto z1 = fizzbuzz_for_type(1);
	std::cout << z1 << std::endl;	//1が帰る

	auto z2 = fizzbuzz_for_type(2);
	std::cout << z2 << std::endl;	//2が帰る

	auto z3 = fizzbuzz_for_type(3);
	if (z3.type() == typeid(int) ) //if (!z3) や z3.isOK() と同じ意味です。
	{
		std::cout << z3 << std::endl;	//3が帰るが、エラーなのでココには来ない
		MY_ASSERT(0);
	}
	else if (z3.type() == typeid(error_fizz) )
	{
		std::cout << "fizz" << std::endl;
	}
	else if (z3.type() == typeid(error_buzz) )
	{
		std::cout << "buzz" << std::endl;
		MY_ASSERT(0); //今回は3 fizzなのでここにはこない
	}
	else if (z3.type() == typeid(error_fizzbuzz) )
	{
		std::cout << "fizzbuzz" << std::endl;
		MY_ASSERT(0); //今回は3 fizzなのでここにはこない
	}
	else
	{
		MY_ASSERT(0);
	}

	//仮に型でエラーを返したとしても、今まで通り使えます。

	//fizzbuzz の変化形に出会うとエラーになります。
	//それ以外は入力した値が帰ります。
	int a1 = fizzbuzz_for_type(1);
	std::cout << a1 << std::endl;	//1が帰る

	int a2 = fizzbuzz_for_type(2);
	std::cout << a2 << std::endl;	//2が帰る

	try
	{
		int a3 = fizzbuzz_for_type(3);
		std::cout << a3 << std::endl;	//ココには来ない.
										//3はfizz でエラーになるので int にキャストした瞬間に殺される。
		MY_ASSERT(0);
	}
	catch(xreturn::error &e )
	{
		std::cout << e.what() << std::endl;	//エラーが表示される
	}

	//エラー判別をもうちょっと楽に行うことも出来ます。
	auto b1 = fizzbuzz_for_type(1);
	std::cout << b1 << std::endl;	//1が帰る

	auto b2 = fizzbuzz_for_type(2);
	std::cout << static_cast<int>(b2) << std::endl;	//2が帰る  明確的にキャストしてみる.

	auto b3 = fizzbuzz_for_type(3);
	if (!b3) //エラーかどうか楽に判別できます。
	{
		std::cout << "エラーメッセージ " << b3.getErrorMessage() << " エラーコード "  << b3.getErrorCode() << std::endl;	//エラー
	}
	else
	{
		MY_ASSERT(0);
	}


	//エラーを自前でexceptionに変えて飛ばせたりもします。
	auto c3 = fizzbuzz_for_type(3);
	if (!c3)
	{
		try
		{
			//保持しているエラーを exceptionに変換して飛ばす.
			c3.throwException();
			MY_ASSERT(0);
		}
		catch(xreturn::error &e)
		{
			std::cout << e.what() << std::endl;	//エラーが表示される
		}
	}
	else
	{
		MY_ASSERT(0);
	}


	//エラーをチェックしないずぼらな人も安心して使えます。

	//エラーじゃないので無視してもOK
	fizzbuzz_for_type(1);
	//エラーじゃないので無視してもOK
	fizzbuzz_for_type(2);

	//エラー
	try
	{
		//fizzなのでエラーです。無視すると殺します.
		fizzbuzz_for_type(3);
		MY_ASSERT(0);
	}
	catch(xreturn::error &e)
	{
		std::cout << e.what() << std::endl;	//エラーが表示される
	}



	//明示的にチェックする場合
	//.check() を呼ぶと、エラーだったら、例外を投げて、そうでなければ、値を返してくれます。
	//ワンライナーにオススメです。
	int d1 = fizzbuzz_for_type(1).check();
	std::cout << d1 << std::endl;	//trueが帰る

	int d2 = fizzbuzz_for_type(2).check();
	std::cout << d2 << std::endl;	//trueが帰る

	try
	{
		int d3 = fizzbuzz_for_type(3).check();
		std::cout << d3 << std::endl;	//ココには来ない.
										//3はfizz でエラーになるので bool にキャストした瞬間に殺される。
		MY_ASSERT(0);
	}
	catch(xreturn::error &e )
	{
		std::cout << e.what() << std::endl;	//エラーが表示される
	}
}


//3で割り切れる時は xreturn::error
//5で割り切れる時は xreturn::error
//3と5で割り切れる時は false を返す。
//それ以外は、 true を返す.
xreturn::r<bool> fizzbuzz_bool(int a)
{
	if (((a % 3) == 0) && ((a % 5) == 0))
	{
		return false;
	}
	if ((a % 3) == 0)
	{
		return xreturn::error("3で割り切れますがな",3);
	}
	if ((a % 5) == 0)
	{
		return xreturn::error("5で割り切れますがな",5);
	}
	return true;
}

void test3()
{
	std::cout << "=== test2 === bool型も自然に使えるよ " << std::endl;

	//型でエラーを返すのでこんなこともできます。
	auto z1 = fizzbuzz_bool(1);
	std::cout << z1 << std::endl;	//trueが帰る

	auto z2 = fizzbuzz_bool(2);
	std::cout << z2 << std::endl;	//trueが帰る

	auto z3 = fizzbuzz_bool(3);
	if (z3.type() == typeid(bool) ) //if (!z3) や z3.isOK() と同じ意味です。
	{
		std::cout << z3 << std::endl;	//エラーなのでココには来ない
		MY_ASSERT(0);
	}
	else if (z3.type() == typeid(xreturn::error) )
	{
		std::cout << "xreturn::error" << std::endl;
	}
	else
	{
		MY_ASSERT(0);
	}

	//fizzbuzz の変化形に出会うとエラーになります。
	//それ以外は入力した値が帰ります。
	bool a1 = fizzbuzz_bool(1);
	std::cout << a1 << std::endl;	//trueが帰る

	bool a2 = fizzbuzz_bool(2);
	std::cout << a2 << std::endl;	//trueが帰る

	try
	{
		bool a3 = fizzbuzz_bool(3);
		std::cout << a3 << std::endl;	//ココには来ない.
										//3はfizz でエラーになるので bool にキャストした瞬間に殺される。
		MY_ASSERT(0);
	}
	catch(xreturn::error &e )
	{
		std::cout << e.what() << std::endl;	//エラーが表示される
	}

	//エラー判別をもうちょっと楽に行うことも出来ます。
	auto b1 = fizzbuzz_bool(1);
	std::cout << b1 << std::endl;	//trueが帰る

	auto b2 = fizzbuzz_bool(2);
	std::cout << static_cast<bool>(b2) << std::endl;	//trueが帰る  明確的にキャストしてみる.

	auto b3 = fizzbuzz_bool(3);
	if (!b3) //エラーかどうか楽に判別できます。
	{
		std::cout << "エラーメッセージ " << b3.getErrorMessage() << " エラーコード "  << b3.getErrorCode() << std::endl;	//エラー
	}
	else
	{
		MY_ASSERT(0);
	}


	//エラーを自前でexceptionに変えて飛ばせたりもします。
	auto c3 = fizzbuzz_bool(3);
	if (!c3)
	{
		try
		{
			//保持しているエラーを exceptionに変換して飛ばす.
			c3.throwException();
			MY_ASSERT(0);
		}
		catch(xreturn::error &e)
		{
			std::cout << e.what() << std::endl;	//エラーが表示される
		}
	}
	else
	{
		MY_ASSERT(0);
	}


	//エラーをチェックしないずぼらな人も安心して使えます。

	//エラーじゃないので無視してもOK
	fizzbuzz_bool(1);
	//エラーじゃないので無視してもOK
	fizzbuzz_bool(2);

	//エラー
	try
	{
		//fizzなのでエラーです。無視すると殺します.
		fizzbuzz_bool(3);
		MY_ASSERT(0);
	}
	catch(xreturn::error &e)
	{
		std::cout << e.what() << std::endl;	//エラーが表示される
	}


	//明示的にチェックする場合
	//.check() を呼ぶと、エラーだったら、例外を投げて、そうでなければ、値を返してくれます。
	//ワンライナーにオススメです。
	bool d1 = fizzbuzz_bool(1).check();
	std::cout << d1 << std::endl;	//trueが帰る

	bool d2 = fizzbuzz_bool(2).check();
	std::cout << d2 << std::endl;	//trueが帰る

	try
	{
		bool d3 = fizzbuzz_bool(3).check();
		std::cout << d3 << std::endl;	//ココには来ない.
										//3はfizz でエラーになるので bool にキャストした瞬間に殺される。
		MY_ASSERT(0);
	}
	catch(xreturn::error &e )
	{
		std::cout << e.what() << std::endl;	//エラーが表示される
	}

}

#ifdef _WIN32
xreturn::r<DWORD,xreturn::windowsError> windowsErrortest()
{
	BOOL r = ReadFile(NULL , 0 , 0 , 0  ,0 );
	if (!r)
	{
		return xreturn::windowsError(::GetLastError());
	}

	return 0;
}

void wintest()
{
	auto r = windowsErrortest();
	if (!r)
	{
		std::cout << "エラーメッセージ " << r.getErrorMessage() << " エラーコード "  << r.getErrorCode() << std::endl;	//エラー
	}
	else
	{
		MY_ASSERT(0);
	}
}
#endif //_WIN32

#ifdef errno
xreturn::r<DWORD,xreturn::errnoError> errnoErrortest()
{
	FILE * fp  = fopen("そんなファイルあるわけない" , "rb");
	if (!fp)
	{
		return xreturn::errnoError(errno);
	}

	return 0;
}

void errnotest()
{
	auto r = errnoErrortest();
	if (!r)
	{
		std::cout << "エラーメッセージ " << r.getErrorMessage() << " エラーコード "  << r.getErrorCode() << std::endl;	//エラー
	}
	else
	{
		MY_ASSERT(0);
	}
}
#endif

#ifdef _WIN32
//パフォーマンス比較
int bench_normal_func(volatile int a)
{
	if (a%5) return -1;
	return a + 3;
}
xreturn::r<int> bench_xreturn_func(volatile int a)
{
	if (a%5) return -1;
	return a + 3;
}
void bench()
{
	volatile int errorcount = 0;
	volatile int i = 0;
	const int countmax = 10000000;
	std::cout << "ベンチマーク " <<  countmax << "回呼び出します" << std::endl;

	DWORD starttime = 0;
	DWORD endtime = 0;
	errorcount = 0;
	starttime = ::GetTickCount();
	for(i = 0 ; i < countmax ; ++i )
	{
		int r = bench_normal_func(i);
		if (r == -1)
		{
			//エラー
			errorcount++;
		}
	}
	endtime = ::GetTickCount();
	std::cout <<  "非xreturn: " << ( endtime - starttime) << "ms" << std::endl;

	errorcount = 0;
	starttime = ::GetTickCount();
	for(i = 0 ; i < countmax ; ++i )
	{
		auto r = bench_xreturn_func(i);
		if (!r)
		{
			//エラー
			errorcount++;
		}
	}
	endtime = ::GetTickCount();
	std::cout <<  "xreturn: " << ( endtime - starttime) << "ms" << std::endl;
}
#endif

int main(int argc, const char* argv[])
{
	test1();
	test2();
	test3();

#ifdef _WIN32
	wintest();
#endif
#ifdef errno
	errnotest();
#endif

#ifdef _WIN32
	//ベンチマーク取りたい人はこれを有効にするよろし.
	bench();
#endif

#if _WIN32 && _DEBUG
	//メモリーリークの検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	return 0;
}
