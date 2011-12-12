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
		return xreturn::error("3��5�Ŋ���؂�܂�����",35);
	}
	if ((a % 3) == 0)
	{
		return xreturn::error("3�Ŋ���؂�܂�����",3);
	}
	if ((a % 5) == 0)
	{
		return xreturn::error("5�Ŋ���؂�܂�����",5);
	}
	return a;
}

void test1()
{
	std::cout << "=== test1 === �ӂ[�Ɏg���Ă݂悤 " << std::endl;
	//fizzbuzz �̕ω��`�ɏo��ƃG���[�ɂȂ�܂��B
	//����ȊO�͓��͂����l���A��܂��B
	int a1 = fizzbuzz(1);
	std::cout << a1 << std::endl;	//1���A��

	int a2 = fizzbuzz(2);
	std::cout << a2 << std::endl;	//2���A��

	try
	{
		int a3 = fizzbuzz(3);
		std::cout << a1 << std::endl;	//�R�R�ɂ͗��Ȃ�.
										//3��fizz �ŃG���[�ɂȂ�̂� int �ɃL���X�g�����u�ԂɎE�����B
		MY_ASSERT(0);
	}
	catch(xreturn::error &e )
	{
		std::cout << e.what() << std::endl;	//�G���[���\�������
	}

	//�G���[���ʂ�����������Ɗy�ɍs�����Ƃ��o���܂��B
	auto b1 = fizzbuzz(1);
	std::cout << b1 << std::endl;	//1���A��

	auto b2 = fizzbuzz(2);
	std::cout << static_cast<int>(b2) << std::endl;	//2���A��  ���m�I�ɃL���X�g���Ă݂�.

	auto b3 = fizzbuzz(3);
	if (!b3) //�G���[���ǂ����y�ɔ��ʂł��܂��B
	{
		std::cout << "�G���[���b�Z�[�W " << b3.getErrorMessage() << " �G���[�R�[�h "  << b3.getErrorCode() << std::endl;	//�G���[
	}
	else
	{
		MY_ASSERT(0);
	}


	//�G���[�����O��exception�ɕς��Ĕ�΂���������܂��B
	auto c3 = fizzbuzz(3);
	if (!c3)
	{
		try
		{
			//�ێ����Ă���G���[�� exception�ɕϊ����Ĕ�΂�.
			c3.throwException();
			MY_ASSERT(0);
		}
		catch(xreturn::error &e)
		{
			std::cout << e.what() << std::endl;	//�G���[���\�������
		}
	}
	else
	{
		MY_ASSERT(0);
	}


	//�G���[���`�F�b�N���Ȃ����ڂ�Ȑl�����S���Ďg���܂��B

	//�G���[����Ȃ��̂Ŗ������Ă�OK
	fizzbuzz(1);
	//�G���[����Ȃ��̂Ŗ������Ă�OK
	fizzbuzz(2);

	//�G���[
	try
	{
		//fizz�Ȃ̂ŃG���[�ł��B��������ƎE���܂�.
		fizzbuzz(3);
		MY_ASSERT(0);
	}
	catch(xreturn::error &e)
	{
		std::cout << e.what() << std::endl;	//�G���[���\�������
	}




	//�����I�Ƀ`�F�b�N����ꍇ
	//.check() ���ĂԂƁA�G���[��������A��O�𓊂��āA�����łȂ���΁A�l��Ԃ��Ă���܂��B
	//�������C�i�[�ɃI�X�X���ł��B
	int d1 = fizzbuzz(1).check();
	std::cout << d1 << std::endl;	//true���A��

	int d2 = fizzbuzz(2).check();
	std::cout << d2 << std::endl;	//true���A��

	try
	{
		int d3 = fizzbuzz(3).check();
		std::cout << d3 << std::endl;	//�R�R�ɂ͗��Ȃ�.
										//3��fizz �ŃG���[�ɂȂ�̂� bool �ɃL���X�g�����u�ԂɎE�����B
		MY_ASSERT(0);
	}
	catch(xreturn::error &e )
	{
		std::cout << e.what() << std::endl;	//�G���[���\�������
	}
}


//�G���[���^�ŕ\�����邱�Ƃ��ł��܂��B C++�ɕs�\�͂���܂���B
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
		return error_fizzbuzz("3��5�Ŋ���؂�܂�����",35);
	}
	if ((a % 3) == 0)
	{
		return error_fizz("3�Ŋ���؂�܂�����",3);
	}
	if ((a % 5) == 0)
	{
		return error_buzz("5�Ŋ���؂�܂�����",5);
	}
	return a;
}


void test2()
{
	std::cout << "=== test2 === �^�ŃG���[��Ԃ��Ă݂悤 " << std::endl;

	//�^�ŃG���[��Ԃ��̂ł���Ȃ��Ƃ��ł��܂��B
	auto z1 = fizzbuzz_for_type(1);
	std::cout << z1 << std::endl;	//1���A��

	auto z2 = fizzbuzz_for_type(2);
	std::cout << z2 << std::endl;	//2���A��

	auto z3 = fizzbuzz_for_type(3);
	if (z3.type() == typeid(int) ) //if (!z3) �� z3.isOK() �Ɠ����Ӗ��ł��B
	{
		std::cout << z3 << std::endl;	//3���A�邪�A�G���[�Ȃ̂ŃR�R�ɂ͗��Ȃ�
		MY_ASSERT(0);
	}
	else if (z3.type() == typeid(error_fizz) )
	{
		std::cout << "fizz" << std::endl;
	}
	else if (z3.type() == typeid(error_buzz) )
	{
		std::cout << "buzz" << std::endl;
		MY_ASSERT(0); //�����3 fizz�Ȃ̂ł����ɂ͂��Ȃ�
	}
	else if (z3.type() == typeid(error_fizzbuzz) )
	{
		std::cout << "fizzbuzz" << std::endl;
		MY_ASSERT(0); //�����3 fizz�Ȃ̂ł����ɂ͂��Ȃ�
	}
	else
	{
		MY_ASSERT(0);
	}

	//���Ɍ^�ŃG���[��Ԃ����Ƃ��Ă��A���܂Œʂ�g���܂��B

	//fizzbuzz �̕ω��`�ɏo��ƃG���[�ɂȂ�܂��B
	//����ȊO�͓��͂����l���A��܂��B
	int a1 = fizzbuzz_for_type(1);
	std::cout << a1 << std::endl;	//1���A��

	int a2 = fizzbuzz_for_type(2);
	std::cout << a2 << std::endl;	//2���A��

	try
	{
		int a3 = fizzbuzz_for_type(3);
		std::cout << a3 << std::endl;	//�R�R�ɂ͗��Ȃ�.
										//3��fizz �ŃG���[�ɂȂ�̂� int �ɃL���X�g�����u�ԂɎE�����B
		MY_ASSERT(0);
	}
	catch(xreturn::error &e )
	{
		std::cout << e.what() << std::endl;	//�G���[���\�������
	}

	//�G���[���ʂ�����������Ɗy�ɍs�����Ƃ��o���܂��B
	auto b1 = fizzbuzz_for_type(1);
	std::cout << b1 << std::endl;	//1���A��

	auto b2 = fizzbuzz_for_type(2);
	std::cout << static_cast<int>(b2) << std::endl;	//2���A��  ���m�I�ɃL���X�g���Ă݂�.

	auto b3 = fizzbuzz_for_type(3);
	if (!b3) //�G���[���ǂ����y�ɔ��ʂł��܂��B
	{
		std::cout << "�G���[���b�Z�[�W " << b3.getErrorMessage() << " �G���[�R�[�h "  << b3.getErrorCode() << std::endl;	//�G���[
	}
	else
	{
		MY_ASSERT(0);
	}


	//�G���[�����O��exception�ɕς��Ĕ�΂���������܂��B
	auto c3 = fizzbuzz_for_type(3);
	if (!c3)
	{
		try
		{
			//�ێ����Ă���G���[�� exception�ɕϊ����Ĕ�΂�.
			c3.throwException();
			MY_ASSERT(0);
		}
		catch(xreturn::error &e)
		{
			std::cout << e.what() << std::endl;	//�G���[���\�������
		}
	}
	else
	{
		MY_ASSERT(0);
	}


	//�G���[���`�F�b�N���Ȃ����ڂ�Ȑl�����S���Ďg���܂��B

	//�G���[����Ȃ��̂Ŗ������Ă�OK
	fizzbuzz_for_type(1);
	//�G���[����Ȃ��̂Ŗ������Ă�OK
	fizzbuzz_for_type(2);

	//�G���[
	try
	{
		//fizz�Ȃ̂ŃG���[�ł��B��������ƎE���܂�.
		fizzbuzz_for_type(3);
		MY_ASSERT(0);
	}
	catch(xreturn::error &e)
	{
		std::cout << e.what() << std::endl;	//�G���[���\�������
	}



	//�����I�Ƀ`�F�b�N����ꍇ
	//.check() ���ĂԂƁA�G���[��������A��O�𓊂��āA�����łȂ���΁A�l��Ԃ��Ă���܂��B
	//�������C�i�[�ɃI�X�X���ł��B
	int d1 = fizzbuzz_for_type(1).check();
	std::cout << d1 << std::endl;	//true���A��

	int d2 = fizzbuzz_for_type(2).check();
	std::cout << d2 << std::endl;	//true���A��

	try
	{
		int d3 = fizzbuzz_for_type(3).check();
		std::cout << d3 << std::endl;	//�R�R�ɂ͗��Ȃ�.
										//3��fizz �ŃG���[�ɂȂ�̂� bool �ɃL���X�g�����u�ԂɎE�����B
		MY_ASSERT(0);
	}
	catch(xreturn::error &e )
	{
		std::cout << e.what() << std::endl;	//�G���[���\�������
	}
}


//3�Ŋ���؂�鎞�� xreturn::error
//5�Ŋ���؂�鎞�� xreturn::error
//3��5�Ŋ���؂�鎞�� false ��Ԃ��B
//����ȊO�́A true ��Ԃ�.
xreturn::r<bool> fizzbuzz_bool(int a)
{
	if (((a % 3) == 0) && ((a % 5) == 0))
	{
		return false;
	}
	if ((a % 3) == 0)
	{
		return xreturn::error("3�Ŋ���؂�܂�����",3);
	}
	if ((a % 5) == 0)
	{
		return xreturn::error("5�Ŋ���؂�܂�����",5);
	}
	return true;
}

void test3()
{
	std::cout << "=== test2 === bool�^�����R�Ɏg����� " << std::endl;

	//�^�ŃG���[��Ԃ��̂ł���Ȃ��Ƃ��ł��܂��B
	auto z1 = fizzbuzz_bool(1);
	std::cout << z1 << std::endl;	//true���A��

	auto z2 = fizzbuzz_bool(2);
	std::cout << z2 << std::endl;	//true���A��

	auto z3 = fizzbuzz_bool(3);
	if (z3.type() == typeid(bool) ) //if (!z3) �� z3.isOK() �Ɠ����Ӗ��ł��B
	{
		std::cout << z3 << std::endl;	//�G���[�Ȃ̂ŃR�R�ɂ͗��Ȃ�
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

	//fizzbuzz �̕ω��`�ɏo��ƃG���[�ɂȂ�܂��B
	//����ȊO�͓��͂����l���A��܂��B
	bool a1 = fizzbuzz_bool(1);
	std::cout << a1 << std::endl;	//true���A��

	bool a2 = fizzbuzz_bool(2);
	std::cout << a2 << std::endl;	//true���A��

	try
	{
		bool a3 = fizzbuzz_bool(3);
		std::cout << a3 << std::endl;	//�R�R�ɂ͗��Ȃ�.
										//3��fizz �ŃG���[�ɂȂ�̂� bool �ɃL���X�g�����u�ԂɎE�����B
		MY_ASSERT(0);
	}
	catch(xreturn::error &e )
	{
		std::cout << e.what() << std::endl;	//�G���[���\�������
	}

	//�G���[���ʂ�����������Ɗy�ɍs�����Ƃ��o���܂��B
	auto b1 = fizzbuzz_bool(1);
	std::cout << b1 << std::endl;	//true���A��

	auto b2 = fizzbuzz_bool(2);
	std::cout << static_cast<bool>(b2) << std::endl;	//true���A��  ���m�I�ɃL���X�g���Ă݂�.

	auto b3 = fizzbuzz_bool(3);
	if (!b3) //�G���[���ǂ����y�ɔ��ʂł��܂��B
	{
		std::cout << "�G���[���b�Z�[�W " << b3.getErrorMessage() << " �G���[�R�[�h "  << b3.getErrorCode() << std::endl;	//�G���[
	}
	else
	{
		MY_ASSERT(0);
	}


	//�G���[�����O��exception�ɕς��Ĕ�΂���������܂��B
	auto c3 = fizzbuzz_bool(3);
	if (!c3)
	{
		try
		{
			//�ێ����Ă���G���[�� exception�ɕϊ����Ĕ�΂�.
			c3.throwException();
			MY_ASSERT(0);
		}
		catch(xreturn::error &e)
		{
			std::cout << e.what() << std::endl;	//�G���[���\�������
		}
	}
	else
	{
		MY_ASSERT(0);
	}


	//�G���[���`�F�b�N���Ȃ����ڂ�Ȑl�����S���Ďg���܂��B

	//�G���[����Ȃ��̂Ŗ������Ă�OK
	fizzbuzz_bool(1);
	//�G���[����Ȃ��̂Ŗ������Ă�OK
	fizzbuzz_bool(2);

	//�G���[
	try
	{
		//fizz�Ȃ̂ŃG���[�ł��B��������ƎE���܂�.
		fizzbuzz_bool(3);
		MY_ASSERT(0);
	}
	catch(xreturn::error &e)
	{
		std::cout << e.what() << std::endl;	//�G���[���\�������
	}


	//�����I�Ƀ`�F�b�N����ꍇ
	//.check() ���ĂԂƁA�G���[��������A��O�𓊂��āA�����łȂ���΁A�l��Ԃ��Ă���܂��B
	//�������C�i�[�ɃI�X�X���ł��B
	bool d1 = fizzbuzz_bool(1).check();
	std::cout << d1 << std::endl;	//true���A��

	bool d2 = fizzbuzz_bool(2).check();
	std::cout << d2 << std::endl;	//true���A��

	try
	{
		bool d3 = fizzbuzz_bool(3).check();
		std::cout << d3 << std::endl;	//�R�R�ɂ͗��Ȃ�.
										//3��fizz �ŃG���[�ɂȂ�̂� bool �ɃL���X�g�����u�ԂɎE�����B
		MY_ASSERT(0);
	}
	catch(xreturn::error &e )
	{
		std::cout << e.what() << std::endl;	//�G���[���\�������
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
		std::cout << "�G���[���b�Z�[�W " << r.getErrorMessage() << " �G���[�R�[�h "  << r.getErrorCode() << std::endl;	//�G���[
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
	FILE * fp  = fopen("����ȃt�@�C������킯�Ȃ�" , "rb");
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
		std::cout << "�G���[���b�Z�[�W " << r.getErrorMessage() << " �G���[�R�[�h "  << r.getErrorCode() << std::endl;	//�G���[
	}
	else
	{
		MY_ASSERT(0);
	}
}
#endif

#ifdef _WIN32
//�p�t�H�[�}���X��r
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
	std::cout << "�x���`�}�[�N " <<  countmax << "��Ăяo���܂�" << std::endl;

	DWORD starttime = 0;
	DWORD endtime = 0;
	errorcount = 0;
	starttime = ::GetTickCount();
	for(i = 0 ; i < countmax ; ++i )
	{
		int r = bench_normal_func(i);
		if (r == -1)
		{
			//�G���[
			errorcount++;
		}
	}
	endtime = ::GetTickCount();
	std::cout <<  "��xreturn: " << ( endtime - starttime) << "ms" << std::endl;

	errorcount = 0;
	starttime = ::GetTickCount();
	for(i = 0 ; i < countmax ; ++i )
	{
		auto r = bench_xreturn_func(i);
		if (!r)
		{
			//�G���[
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
	//�x���`�}�[�N��肽���l�͂����L���ɂ����낵.
	bench();
#endif

#if _WIN32 && _DEBUG
	//�������[���[�N�̌��o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	return 0;
}
