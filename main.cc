#include "main.h"
#include <algorithm>
using namespace std;
using namespace unitpp;
namespace { 
	bool verbose;
}
int main(int argc, const char* argv[])
{
	options_utils::optmap opts(argc, argv, "[ testids... ]");
	opts.add("v", new options_utils::opt_flag(verbose));
	opts.alias("verbose", "v");
	if (!opts.parse())
		opts.usage();
	bool res = true;
	if (opts.n() < argc)
		for (int i = opts.n(); i < argc; ++i)
			res = res && run_test(argv[i]);
	else
		res = run_test();
	return 0;
}

namespace unitpp {
vector<string> vectorize(const string& str, char c)
{
	vector<string> res;
	string::const_iterator s, p;
	for (s = str.begin(); s != str.end(); ) {
		p = find(s, str.end(), c);
		res.push_back(string(s, p));
		s = (p == str.end()) ? p : p + 1;
	}
	return res;
}
test* find_test(const string& id)
{
	vector<string> ss(vectorize(id, '.'));
	test* tp = &static_cast<test&>(suite::main());
	for (vector<string>::iterator p = ss.begin(); p != ss.end(); ++p) {
		suite* sp = dynamic_cast<suite*>(tp);
		if (!sp) {
			return 0;
		}
		cout << "getting child: " << *p << endl;
		tp = sp->get_child(*p);
	}
	return tp;
}
bool run_test(const string& id)
{
	test* tp = find_test(id);
	if (!tp) {
		return false;
	}
	return run_test(tp);
}
bool run_test(test* tp)
{
	tester tst(cout, verbose);
	tp->visit(&tst);
	tst.summary();
	res_cnt res(tst.res_tests());
	return res.n_err() == 0 && res.n_fail() == 0;
}

}

