#include <algorithm>
#include <functional>
#include "unit++.h"
#include "asserter.h"

using namespace unitpp;

void test::visit(visitor* v)
{
	v->visit(*this);
}
testcase::testcase(test* t)
: cnt(new size_t(1)), tst(t)
{
}
testcase::testcase(const testcase& t)
: cnt(t.cnt), tst(t.tst)
{
	(*cnt)++;
}
void testcase::dec_cnt()
{
	if (--(*cnt) == 0) {
		delete cnt;
		delete tst;
	}
}
testcase::~testcase()
{
	dec_cnt();
}
testcase& testcase::operator=(const testcase& t)
{
	++*(t.cnt);
	dec_cnt();
	cnt = t.cnt;
	tst = t.tst;
	return *this;
}
suite& suite::main()
{
	static suite instance("top");
	return instance;
}
test* suite::get_child(const string& id)
{
	vector<string>::iterator p = find(ids.begin(), ids.end(), id);
	if (p != ids.end())
		return &(static_cast<test&>(tests[p - ids.begin()]));
	return 0;
}
void suite::add(const string& id, const testcase& t)
{
	ids.push_back(id);
	tests.push_back(t);
}
void suite::visit(visitor* v)
{
	v->visit(*this);
	for_each(tests.begin(), tests.end(),
		bind2nd(mem_fun_ref(&testcase::visit), v));
	v->visit(*this, 0);
}
