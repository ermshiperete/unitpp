#include "gui.h"
#include "unit++.h"
using namespace unitpp;

#ifdef GUI
static const char* const img_error[] = { 
"14 14 2 1",
". c #ff0000",
"# c #ffffff",
"...########...",
"....######....",
".....####.....",
"#.....##.....#",
"##..........##",
"###........###",
"####......####",
"####......####",
"###........###",
"##..........##",
"#.....##.....#",
".....####.....",
"....######....",
"...########..."};

static const char* const img_fail[] = { 
"14 14 2 1",
"# c #ff0000",
". c #ffffff",
"..............",
".##........##.",
".###......###.",
"..###....###..",
"...###..###...",
"....######....",
".....####.....",
".....####.....",
"....######....",
"...###..###...",
"..###....###..",
".###......###.",
".##........##.",
".............."};

static const char* const img_ok[] = { 
"14 14 3 1",
"# c #00ff00",
"a c #00ff6a",
". c #ffffff",
"..........###.",
".........###..",
"........#a#...",
"........aa#...",
".......#aa....",
".......#a.....",
"......#aa.....",
"#.....aa#.....",
"###..#a#......",
".##.##a.......",
"..#aaa#.......",
"...#aa#.......",
"....##........",
".....#........"};

static const char* const img_empty[] = { 
"14 14 2 1",
". c #f0f0f0",
"# c #ffffff",
"..............",
".############.",
".############.",
".############.",
".############.",
".############.",
".############.",
".############.",
".############.",
".############.",
".############.",
".############.",
".############.",
".............."};


cnt_item::cnt_item(QWidget* par, const QString& txt, const QColor& col,
		const char* name)
	: QHBox(par, name)
{
	setSpacing(3);
	setMargin(5);
	val = new QLabel("0", this);
	val->setAlignment(QLabel::AlignVCenter | QLabel::AlignRight);
	QPalette pal(val->palette());
	QColorGroup cg(pal.active());
	cg.setColor(QColorGroup::Foreground, col);
	pal.setActive(cg);
	val->setPalette(pal);
	label = new QLabel(txt, this);
}
void cnt_item::value(int v)
{
	val->setNum(v);
}

cnt_line::cnt_line(const QString& txt, QWidget* par, const char* name)
	: QHBox(par, name)
{
	label = new QLabel(txt, this);
	QFont font(label->font());
	font.setBold(true);
	label->setFont(font);
	cnts[0] = new cnt_item(this, "Total");
	cnts[1] = new cnt_item(this, "OK", green);
	cnts[2] = new cnt_item(this, "Fail", red);
	cnts[3] = new cnt_item(this, "Error", red);
	setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
}

void cnt_line::max(int m)
{
	cnts[0]->value(m);
}

res_stack::res_stack(const QString& txt, QWidget* par, const char* name)
	: QVBox(par, name)
{
	cnts = new cnt_line(txt, this, name);
	bar = new QProgressBar(this);
	setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
}
void res_stack::max(int v)
{
	cnts->max(v);
	bar->setTotalSteps(v);
}

QHBox* behave(QHBox* box, bool x_expand, bool y_expand)
{
	box->setSizePolicy(QSizePolicy(
				x_expand ? QSizePolicy::Preferred : QSizePolicy::Maximum,
				y_expand ? QSizePolicy::Preferred : QSizePolicy::Maximum));
	box->setFrameShape(QFrame::StyledPanel);
	box->setFrameShadow(QFrame::Raised);
	box->setMargin(11);
	box->setSpacing(6);
	return box;
}

void node::setImg()
{
	static QPixmap i_emp((const char**)img_empty);
	static QPixmap i_ok((const char**)img_ok);
	static QPixmap i_fail((const char**)img_fail);
	static QPixmap i_err((const char**)img_error);
	static QPixmap* imgs[] = { &i_emp, &i_ok, &i_fail, &i_err };
	item->setPixmap(0, *(imgs[st]));
};

node::node(node* par, test& t)
	: item(new QListViewItem(par->lvi(), t.name().c_str())), t(t), st(none)
{
	setImg();
}
node::node(gui* gp, test& t)
	: item(new QListViewItem(gp->test_tree(), t.name().c_str())), t(t), st(none)
{
	setImg();
	item->setOpen(true);
}
gui::gui(QApplication& app, QWidget* par, const char* name)
	: QVBox(par, name), app(app)
{
	setMargin(6);
	setSpacing(3);
	tree = new QListView(behave(new QVBox(this), true, true), "tree");
	tree->addColumn("Test");
	tree->setFrameShadow(QListView::Sunken);
	tree->setResizePolicy(QScrollView::Manual);
	tree->setRootIsDecorated(true);
	QVBox* f_cnts = new QVBox(this);
	behave(f_cnts, true, false);
	suites = new res_stack("Suites", f_cnts);
	tests = new res_stack("Tests", f_cnts);
	QHBox* hbox = behave(new QHBox(this), true, false);
	run = new QPushButton("Run", hbox, "run");
	stop = new QPushButton("Stop", hbox, "stop");
	quit = new QPushButton("Quit", hbox, "quit");
	connect(quit, SIGNAL(clicked()), &app, SLOT(quit()));
}

gui::~gui() { }

void gui::processEvents(int t)
{
	app.processEvents(t);
}

void gui::totSuites(int v) { suites->max(v); }
void gui::totTests(int v) { tests->max(v); }

#endif
