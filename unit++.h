// Copyright (C) 2001 Claus Dræby
// Terms of use are in the file COPYING
#ifndef __TEST_FW_H_
#define __TEST_FW_H_
#include <vector>
#include <string>
#include <map>
#include "optmap.h"
/**
 * The unitpp name space holds all the stuff needed to use the unit++ testing
 * framework.
 *
 * The normal way to make a test is like this:
 *
\begin{verbatim}
#include<unit++.h>
using namespace unitpp;
// use anonymous namespace so all test clas
namespace {
class Test : public suite {
	void test1()
	{
		// do test stuff
		assert_true("message", exp1); // exp1 should be true
		assert_eq("another msg", 123456, exp2); // exp2 should be 123456
		// ...
	}
	void test2()
	{
		// do something that provokes exception out_of_range
	}
public:
	Test() : suite("appropriate name for test suite")
	{
		// any setup you need
		add("id1", testcase(this, "Test 1", &Test::test1));
		// make a testcase from the method
		testcase tc(this, "Test 2", &Test::test2);
		// add a testcase that expects the exception
		add("id2", exception_case<out_of_range>(tc));
		// add the suite to the global test suite
		suite::main().add("id", this);
	}
} * theTest = new Test();  // by new, since testcase claims ownership
}
\end{verbatim}
 *
 * In order to make an executable test, simply link the above code against
 * libunit++, something like
 *
 * #g++ -o test++ mytest.cc -L <location of libunit++> -lunit++#
 *
 * This will generate a test called #test++# and the standard behaviour for a
 * test. Note that most shells have #test# defined as a shell builtin which
 * makes it a moderately bad name for a program, since it is rather hard to
 * get executed, hence #test++#.
 * @see main
 */
namespace unitpp {

class visitor;
/**
 * The heart of a test system: A test. The test is meant as a base class for
 * the tests that a clinet want performed. This means that all tests are to
 * be pointers dynamically allocated. However, the test system takes
 * responsibilities for freeing them again.
 */
class test {
	std::string nam;
public:
	test(const std::string& name) : nam(name) {}
	virtual ~test() {}
	virtual void operator()() = 0;
	virtual void visit(visitor*);
	virtual test* get_child(const std::string& id) { return 0; }
	std::string name() const { return nam; }
};

/** 
 * A test that is implemented by a member function.
 */
template<typename C>
class test_mfun : public test {
public:
	typedef void (C::*mfp)();
	test_mfun(C* par, const std::string& name, mfp fp)
		: test(name), par(par), fp(fp)
	{}
	virtual void operator()()
	{
		(par->*fp)();
	}
private:
	C* par;
	mfp fp;
};

/**
 * A ref counted reference to a test. This is what test suites are composed
 * of, and what ensures destruction.
 */
class testcase {
	std::size_t* cnt;
	test* tst;
	void dec_cnt();
public:
	/// Simply wrap -- and own -- a test.
	testcase(test* t);
	/// Keep the ref count
	testcase(const testcase& tr);
	/**
	 * Make a testcase from a class and a member function.
	 *
	 * The normal usage is inside some test suite class Test:
	 *
	 * #add("id", testcase(this, "Testing this and that", &Test::test))#
	 *
	 * to make a test that invokes the test method on the instance of the
	 * suite class.
	 * \Ref{test_mfun}
	 */
	template<typename C>
		testcase(C* par, const std::string& name, test_mfun<C>::mfp fp)
		: cnt(new size_t(1)), tst(new test_mfun<C>(par, name, fp))
		{ }
	~testcase();
	testcase& operator=(const testcase&);
	void visit(visitor* vp) const { tst->visit(vp); }
	operator test& () { return *tst; }
	operator const test& () const { return *tst; }
};

/**
 * A wrapper class for the testcase class that succedes if the correct
 * exception is generated.
 */
template<typename E>
class exception_test : public test {
public:
	exception_test(const testcase& tc)
		: test(static_cast<const test&>(tc).name()), tc(tc) {}
	~exception_test() {}
	virtual void operator()()
	{
		try {
			(static_cast<test&>(tc))();
			fail("unexpected lack of exception");
		} catch (E& e) {
			// fine!
		}
	}
private:
	testcase tc;
};
/**
 * Generate a testcase that expects a specific exception from the testcase it
 * wraps. It can be used something like
 *
 * #testcase tc(this, "Test name", &Test::test);#
 *
 * #add("ex", exception_case<out_of_range>(tc));
 *
 * The name of the exception_case is copied from the wrapped testcase, and
 * the exception_case will execute the tc test case and report a failure
 * unless the #out_of_range# exception is generated.
 */
template<typename E>
testcase exception_case(const testcase& tc)
{
	return testcase(new exception_test<E>(tc));
}

/**
 * Splits the string by char c. Each c will generate a new element in the
 * vector, including leading and trailing c.
 */
extern std::vector<std::string> vectorize(const std::string& str, char c);

/**
 * A suite is a test that happens to be a collection of tests. This is an
 * implementation of the Composite pattern.
 */
class suite : public test {
	std::vector<std::string> ids;
	std::vector<testcase> tests;
public:
	suite(const std::string& name) : test(name) {}
	virtual ~suite() {};
	void add(const std::string& id, const testcase& t);
	virtual test* get_child(const std::string& id);
	virtual void operator()() {}
	void visit(visitor*);
	// A singleton instance of the suite class
	static suite& main();
	/// splits the string by `.' and uses each token as a test id
	test* find(const std::string& id);
};

/**
 * The visitor class is a base class for classes that wants to participate in
 * the visitor pattern with the test hierarchi.
 *
 * This is a slightly extended visitor pattern implementation, intended for
 * collaboration with the Composite pattern. The aggregate node (here the
 * suite node) is visited twice, before and after the children are visited.
 * This allows different algorithms to be implemented. @see{visit}
 * @see{post-visit}
 */
class visitor {
public:
	virtual ~visitor() {}
	/// Visit a test case, that is not a suite.
	virtual void visit(test&) = 0;
	/// Visit a suite node before the children are visited.
	virtual void visit(suite&) {};
	/// @name{post-visit} Visit a suite after the children are visited
	virtual void visit(suite&, int dummy) = 0; // post childs
};

// The basic for all failed assert statements.
class assertion_error : public std::exception
{
	std::string msg;
public:
	assertion_error(const std::string& msg) : msg(msg) {}
	std::string message() const { return msg; }
	virtual ~assertion_error() throw () {}
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
	assert_value_error(const std::string& msg, T1& exp, T2& got)
	: assertion_error(msg), exp(exp), got(got)
	{
	}
	virtual ~assert_value_error() throw () {}
	virtual void out(std::ostream& os) const
	{
		os << message() << " [expected: `" << exp << "' got: `" << got << "']";
	}
};
/// The test was not succesful.
inline void fail(const std::string& msg)
{
	throw assertion_error(msg);
}
/// Assert that the assertion is true, that is fail #if (!assertion) ...#
template<class A> inline void assert_true(const std::string& msg, A assertion)
{
	if (!assertion)
		throw assertion_error(msg);
}
/// Assert that the two arguments are equal in the #==# sense.
template<class T1, class T2>
	inline void assert_eq(const std::string& msg, T1 exp, T2 got)
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

/**
 * The singleton instance of the option handler of main.
 *
 * This allows a test to add its own flags to the resulting test program, in
 * the following way.
 * 
 * #bool x_flg = false;#
 * #unitpp::options().add("x", new options_utils::opt_flag(x_flg));#
 *
 * If a -x is now given to the resulting test it will set the #x_flg#
 * variable;
 */
options_utils::optmap& options();

}

#endif
