#ifndef __UNITPP__OPTMAP_H
#define __UNITPP__OPTMAP_H
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
/**
 * The home of \Ref{optmap}. This is a unified way of handling commang line
 * arguments, and suitable usage replies when things goes badly.
 */
namespace options_utils {
/**
 * A argument option handler, based on the Command pattern. Each option is
 * associated with a handling command object.
 *
 * The map keeps the reverse association as well, to allow printing of a
 * using line.
 */
class optmap {
public:
	/**
	 * The base for all option handlers.
	 *
	 * To handle a new kind of commands, just derive a class from this an
	 * overload the methods to do the work.
	 */
	struct cmd {
		/**
		 * Performing the work associated with the option.
		 *
		 * To obtain an argument see \Ref{get_arg}
		 * @return true if the parsing was succesfull
		 */
		virtual bool do_cmd(optmap* om) = 0;
		/// return the name of the argument needed by \Ref{usage}
		virtual string arg() { return string(); }
		virtual ~cmd() {}
	};
	/**
	 * The very simple constructor for an optmap object. Please note that
	 * copying of optmaps are not supported.
	 * @param usage The tail of the usage string to follow the options
	 */
	optmap(int argc, const char** argv, const char* usage = "");
	~optmap();
	/**
	 * Adds an option that invokes the command handler.
	 *
	 * This is the low level work horse for all the add methods. All the
	 * other adds works by adding appropriate cmd objects for doing the work.
	 *
	 * @param o
	 *	The option name. If the length of the option is 1 it will be an old
	 *	fashion option, otherwise it will be a GNU stype long option
	 *	(--long-opt).
	 * @param h
	 *	The pointer to the handler. The optmap assumes ownership of the
	 *	pointer and delete it in the destructor.
	 *
	 * @return *this
	 */
	optmap& add(const char* o, cmd* h);
	/// Adds an alias for an option.
	optmap& alias(const char* new_opt, const char* old_opt);
	/**
	 * Parse the command line.
	 * @return true if the parse is valid, false otherwise.
	 */
	bool parse();
	/// Gets the index for the first non option argument
	int n() { return i; }
	/// display a usage string and abort()
	void usage(bool abort = true);
	/// get the argument of an option; mainly for cmd implementations.
	const char* get_arg();
private:
	int i;
	int argc;
	const char** argv;
	bool multichar;	// doing a multichar, -pdf --> -p -d -f
	bool first_multi;
	optmap(const optmap& o);
	optmap& operator=(const optmap&);
	bool do_cmd(const string& opt);
	std::string tail;
	typedef std::map<std::string, cmd*> cmds_t;
	cmds_t cmds;
	typedef std::map<cmd*, vector<string> > group_t;
	group_t group;
};

/// A cmd handler that can set a boolean flag.
class opt_flag : public optmap::cmd {
	bool& flg;
public:
	opt_flag(bool& flg) : flg(flg) {}
	virtual bool do_cmd(optmap*) { flg = true; return true; }
};

/// A cmd handler that can get an integer value.
class opt_int : public optmap::cmd {
	int& val;
public:
	opt_int(int& val) : val(val) {}
	virtual bool do_cmd(optmap* om);
	virtual string arg() { return string("<int>"); }
};
/// A cmd handler for a string
class opt_string : public optmap::cmd {
	string& val;
public:
	opt_string(string& val) : val(val) {}
	virtual bool do_cmd(optmap* om);
	virtual string arg() { return string("<string>"); }
};
}
#endif
