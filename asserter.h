#ifndef _UNITPP_ASSERTER_H
#define _UNITPP_ASSERTER_H
#include <iostream>
#include <stdexcept>
/// @name unitpp
namespace unitpp {
// The basic for all failed assert statements.
class assertion_error : public std::exception
{
	string msg;
public:
	assertion_error(const string& msg) : msg(msg) {}
	string message() const { return msg; }
	virtual ~assertion_error() {}
	/**
	 * @name assertion_error::out
	 * The virtual method used for operator>>.
	 */
	virtual void out(std::ostream& os) const;
};
// A value comparison has failed, exception keeps expected/got.
template<class T1, class T2>
class assert_value_error : public assertion_error
{
	T1 exp;
	T2 got;
public:
	assert_value_error(const string& msg, T1& exp, T2& got)
	: assertion_error(msg), exp(exp), got(got)
	{
	}
	virtual void out(std::ostream& os) const
	{
		os << message() << " [expected: `" << exp << "' got: `" << got << "']";
	}
};
/// The test was not succesful.
inline void fail(const string& msg)
{
	throw assertion_error(msg);
}
/// Assert that the assertion is true, that is fail #if (!assertion) ...#
template<class A> inline void assert_true(const string& msg, A assertion)
{
	if (!assertion)
		throw assertion_error(msg);
}
/// Assert that the two arguments are equal in the #==# sense.
template<class T1, class T2>
	inline void assert_eq(const string& msg, T1 exp, T2 got)
{
	if (!(exp == got))
		throw assert_value_error<T1,T2>(msg, exp, got);
}
/*
 * Put an assertion error to a stream, using the out method.
 * \Ref{assertion_error::out}
 */
inline std::ostream& operator<<(std::ostream& os, const unitpp::assertion_error& a)
{
	a.out(os);
	return os;
}

}
#endif
