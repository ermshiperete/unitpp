#include <sstream>
#include <cstdlib>
#include "optmap.h"

using namespace std;
using namespace options_utils;

optmap::optmap(int argc, const char** argv, const char* usage)
: i(1), argc(argc), argv(argv), tail(usage)
{
}

optmap::~optmap()
{
	for (group_t::iterator p = group.begin(); p != group.end(); ++p)
		delete p->first;
}
optmap& optmap::add(const char* c, cmd* h)
{
	if (cmds.find(c) != cmds.end())
		throw invalid_argument(string("duplicated option: ")+c);
	cmds[c] = h;
	group[h].push_back(c);
	return *this;
}

optmap& optmap::alias(const char* new_opt, const char* old_opt)
{
	cmd* h = cmds[old_opt];
	if (!h)
		throw invalid_argument(string("no alias: ")+old_opt);
	return add(new_opt, h);
}
bool optmap::parse()
{
	for (; i < argc; ++i) {
		multichar = false;
		const char* s = argv[i];
		size_t l = strlen(s);
		if (*s != '-' || l == 1)
			return true;
		if (s[1] == '-') {
			if (l == 2) { // end of options marker `--'
				++i;
				return true;
			}
			if (!do_cmd(s+2))
				return false;
		} else {
			char cmd[2];
			cmd[1] = '\0';
			multichar = l > 2;
			first_multi = true;
			for (const char* p = s+1; *p; ++p) {
				cmd[0] = *p;
				if (!do_cmd(cmd))
					return false;;
				first_multi = false;
				if (!multichar) // get_arg used it
					break;
			}
		}
	}
	return true;
}

const char* optmap::get_arg()
{
	if (multichar) {
		if (!first_multi) {
			cerr << "internal option requires argument " << argv[i] << endl;
			return 0;
		}
		multichar = false;
		return argv[i]+2;
	}
	return i < argc - 1 ? argv[++i] : 0;
}

void optmap::usage(bool abort)
{
	ostringstream os;
	for (group_t::iterator p = group.begin(); p != group.end(); ++p) {
		vector<string>& v(p->second);
		cmd* h = p->first;
		string arg = h->arg();
		bool need_par = arg.size() > 0 && v.size() > 1;
		bool first = true;
		os << " [";
		if (need_par)
			os << "(";
		for (vector<string>::iterator s = v.begin(); s != v.end(); ++s) {
			os << (first ? " " : " | ") << (s->size() != 1 ? "--" : "-") << *s;
			first = false;
		}
		if (need_par)
			os << ")";
		if (arg.size())
			os << ' ' << arg;
		os << " ]";
	}
	cerr << "usage: " << argv[0] << os.str()
	<< (tail.size() ? " " : "") << tail << endl;
	if (abort)
		exit(1);
}

bool optmap::do_cmd(const string& opt)
{
	cmd* c = cmds[opt];
	if (!c) {
		cerr << "unknown option: " << opt << endl;
		return false;
	}
	return c->do_cmd(this);
}

bool opt_int::do_cmd(optmap* om)
{
	const char* arg = om->get_arg();
	if (!arg)
		return false;
	char* end;
	int v = strtol(arg, &end, 10);
	if (*end) {
		cerr << "failed to parse int argument: " << arg << endl;
		return false;
	}
	val = v;
	return true;
}
bool opt_string::do_cmd(optmap* om)
{
	const char* arg = om->get_arg();
	if (!arg)
		return false;
	val = arg;
	return true;
}
