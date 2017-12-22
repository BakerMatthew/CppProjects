#include <exception>
#include <functional>

template<typename T>
class Expected
{
	public:
		Expected(T t)
		{
			state.t = t;
			valid = true;
		}
		Expected(std::exception_ptr e)
		{
			state.e = e;
			valid = false;
		}
		Expected(std::exception e)
		{
			state.e = std::make_exception_ptr(e);
			valid = false;
		}
		
		T value() const
		{
			if(valid)
			{
				return state.t;
			}
			std::rethrow_exception(state.e);
		}

		operator T()
		{
			return value();
		}

		template<typename U>
		Expected<U> apply(std::function<U(T)> f)
		{
			if(!valid)
			{
				return state.e;
			}
			try
			{
				return f(state.t);
			}
			catch (...)
			{
				return std::current_exception();
			}
		}
	private:
		struct
		{
			T t;
			std::exception_ptr e;
		} state;
		bool valid;
};

template<typename T, typename U>
auto operator+(Expected<T> t, U u)
{
	std::function<U(T)> f = [u](T t){return t+u;};
	return t.apply(f);
}
template<typename T, typename U>
auto operator+(T t, Expected<U> u)
{
	std::function<U(T)> f = [t](U u){return t+u;};
	return u.apply(f);
}

template<typename T, typename U>
auto operator-(Expected<T> t, U u)
{
	std::function<U(T)> f = [u](T t){return t-u;};
	return t.apply(f);
}
template<typename T, typename U>
auto operator-(T t, Expected<U> u)
{
	std::function<U(T)> f = [t](U u){return t-u;};
	return u.apply(f);
}

template<typename T, typename U>
auto operator/(Expected<T> t, U u)
{
	std::function<U(T)> f = [u](T t){return t/u;};
	return t.apply(f);
}
template<typename T, typename U>
auto operator/(T t, Expected<U> u)
{
	std::function<U(T)> f = [t](U u){return t/u;};
	return u.apply(f);
}

template<typename T, typename U>
auto operator*(Expected<T> t, U u)
{
	std::function<U(T)> f = [u](T t){return t*u;};
	return t.apply(f);
}
template<typename T, typename U>
auto operator*(T t, Expected<U> u)
{
	std::function<U(T)> f = [t](U u){return t*u;};
	return u.apply(f);
}

template<typename T, typename U>
auto operator%(Expected<T> t, U u)
{
	std::function<U(T)> f = [u](T t){return t%u;};
	return t.apply(f);
}
template<typename T, typename U>
auto operator%(T t, Expected<U> u)
{
	std::function<U(T)> f = [t](U u){return t%u;};
	return u.apply(f);
}

template<typename T, typename U>
auto operator<(Expected<T> t, U u)
{
	std::function<U(T)> f = [u](T t){return t<u;};
	return t.apply(f);
}
template<typename T, typename U>
auto operator<(T t, Expected<U> u)
{
	std::function<U(T)> f = [t](U u){return t<u;};
	return u.apply(f);
}

template<typename T, typename U>
auto operator==(Expected<T> t, U u)
{
	std::function<U(T)> f = [u](T t){return t==u;};
	return t.apply(f);
}
template<typename T, typename U>
auto operator==(T t, Expected<U> u)
{
	std::function<U(T)> f = [t](U u){return t==u;};
	return u.apply(f);
}

template<typename T, typename U>
auto operator>(Expected<T> t, U u)
{
	std::function<U(T)> f = [u](T t){return t>u;};
	return t.apply(f);
}
template<typename T, typename U>
auto operator>(T t, Expected<U> u)
{
	std::function<U(T)> f = [t](U u){return t>u;};
	return u.apply(f);
}

template<typename T, typename U>
auto operator<=(Expected<T> t, U u)
{
	std::function<U(T)> f = [u](T t){return t<=u;};
	return t.apply(f);
}
template<typename T, typename U>
auto operator<=(T t, Expected<U> u)
{
	std::function<U(T)> f = [t](U u){return t<=u;};
	return u.apply(f);
}

template<typename T, typename U>
auto operator>=(Expected<T> t, U u)
{
	std::function<U(T)> f = [u](T t){return t>=u;};
	return t.apply(f);
}
template<typename T, typename U>
auto operator>=(T t, Expected<U> u)
{
	std::function<U(T)> f = [t](U u){return t>=u;};
	return u.apply(f);
}

template<typename T, typename U>
auto operator!=(Expected<T> t, U u)
{
	std::function<U(T)> f = [u](T t){return t!=u;};
	return t.apply(f);
}
template<typename T, typename U>
auto operator!=(T t, Expected<U> u)
{
	std::function<U(T)> f = [t](U u){return t!=u;};
	return u.apply(f);
}

/********************************************************/
/********************************************************/
/********************************************************/

#include <iostream>
#include <math.h>
#include <stdexcept>

Expected<int> isEven(int x)
{
	if (x % 2 != 0)
	{
		return std::domain_error("Value is not even");
	}
	return x;
}

void test_add(int a, Expected<int> b)
{
	std::cout << "even + even = " << (a + a) << std::endl;

	std::cout << "even + odd  = ";
	try
	{
		std::cout << (a + b);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}

	std::cout << "odd  + even = ";
	try
	{
		std::cout << (b + a);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}
	std::cout << "Press 'enter' to continue to the next test: ";
	std::cin.get();
	std::cout << std::endl;
}

void test_subtract(int a, Expected<int> b)
{
	std::cout << "even - even = " << (a - a) << std::endl;

	std::cout << "even - odd  = ";
	try
	{
		std::cout << (a - b);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}

	std::cout << "odd  - even = ";
	try
	{
		std::cout << (b - a);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}
	std::cout << "Press 'enter' to continue to the next test: ";
	std::cin.get();
	std::cout << std::endl;
}

void test_divide(int a, Expected<int> b)
{
	std::cout << "even / even = " << (a / a) << std::endl;

	std::cout << "even / odd  = ";
	try
	{
		std::cout << (a / b);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}

	std::cout << "odd  / even = ";
	try
	{
		std::cout << (b / a);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}
	std::cout << "Press 'enter' to continue to the next test: ";
	std::cin.get();
	std::cout << std::endl;
}

void test_multiply(int a, Expected<int> b)
{
	std::cout << "even * even = " << (a * a) << std::endl;

	std::cout << "even * odd  = ";
	try
	{
		std::cout << (a * b);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}

	std::cout << "odd  * even = ";
	try
	{
		std::cout << (b * a);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}
	std::cout << "Press 'enter' to continue to the next test: ";
	std::cin.get();
	std::cout << std::endl;
}

void test_mod(int a, Expected<int> b)
{
	std::cout << "even % even = " << (a % a) << std::endl;

	std::cout << "even % odd  = ";
	try
	{
		std::cout << (a % b);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}

	std::cout << "odd  % even = ";
	try
	{
		std::cout << (b % a);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}
	std::cout << "Press 'enter' to continue to the next test: ";
	std::cin.get();
	std::cout << std::endl;
}

void test_lessThan(int a, Expected<int> b)
{
	std::cout << "even < even = " << (a < a) << std::endl;

	std::cout << "even < odd  = ";
	try
	{
		std::cout << (a < b);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}

	std::cout << "odd  < even = ";
	try
	{
		std::cout << (b < a);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}
	std::cout << "Press 'enter' to continue to the next test: ";
	std::cin.get();
	std::cout << std::endl;
}

void test_equalTo(int a, Expected<int> b)
{
	std::cout << "even == even = " << (a == a) << std::endl;

	std::cout << "even == odd  = ";
	try
	{
		std::cout << (a == b);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}

	std::cout << "odd  == even = ";
	try
	{
		std::cout << (b == a);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}
	std::cout << "Press 'enter' to continue to the next test: ";
	std::cin.get();
	std::cout << std::endl;
}

void test_greaterThan(int a, Expected<int> b)
{
	std::cout << "even > even = " << (a > a) << std::endl;

	std::cout << "even > odd  = ";
	try
	{
		std::cout << (a > b);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}

	std::cout << "odd  > even = ";
	try
	{
		std::cout << (b > a);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}
	std::cout << "Press 'enter' to continue to the next test: ";
	std::cin.get();
	std::cout << std::endl;
}

void test_lessThanOrEqual(int a, Expected<int> b)
{
	std::cout << "even <= even = " << (a <= a) << std::endl;

	std::cout << "even <= odd  = ";
	try
	{
		std::cout << (a <= b);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}

	std::cout << "odd  <= even = ";
	try
	{
		std::cout << (b <= a);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}
	std::cout << "Press 'enter' to continue to the next test: ";
	std::cin.get();
	std::cout << std::endl;
}

void test_greaterThanOrEqual(int a, Expected<int> b)
{
	std::cout << "even >= even = " << (a >= a) << std::endl;

	std::cout << "even >= odd  = ";
	try
	{
		std::cout << (a >= b);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}

	std::cout << "odd  >= even = ";
	try
	{
		std::cout << (b >= a);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}
	std::cout << "Press 'enter' to continue to the last test: ";
	std::cin.get();
	std::cout << std::endl;
}

void test_notEqual(int a, Expected<int> b)
{
	std::cout << "even != even = " << (a != a) << std::endl;

	std::cout << "even != odd  = ";
	try
	{
		std::cout << (a != b);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}

	std::cout << "odd  != even = ";
	try
	{
		std::cout << (b != a);
	}
	catch (...)
	{
		std::cout << "odd is invalid." << std::endl;
	}
	std::cout << "DONE WITH TEST CASES.";
	std::cout << std::endl;
}

int main()
{
	std::cout << std::endl << "To simplify testing, all even values are valid and all odd values are not valid." << std::endl;
	std::cout << "NOTE: To step through each test case, press 'enter'." << std::endl << std::endl;

	Expected<int> even_val = isEven(2.0);
	Expected<int> odd_val = isEven(3.0);
	
	test_add(even_val, odd_val);
	test_subtract(even_val, odd_val);
	test_divide(even_val, odd_val);
	test_multiply(even_val, odd_val);
	test_mod(even_val, odd_val);
	test_lessThan(even_val, odd_val);
	test_equalTo(even_val, odd_val);
	test_greaterThan(even_val, odd_val);
	test_lessThanOrEqual(even_val, odd_val);
	test_greaterThanOrEqual(even_val, odd_val);
	test_notEqual(even_val, odd_val);

	return EXIT_SUCCESS;
}