#include "form.h"
#include "SQLiteDB.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Form form;
    form.setWindowTitle("SQLite_Viewer");
    form.show();

    return app.exec();
}

// P.S. I know you're reading this guys at LUXOFT, sorry that I was making this for so long (about a week)
// but I tried as hard as I could, I studied all QT stuff from focking SCRATCH (I knew absolutely nothing
// about it) and here it is - this sqlite db viewer, and I think it's good and it shows that I can
// make everything i want, i just need time.

// Thank you
// Rybalsky Maxim
// :D

// P.P.S
// There are still some bugs that i didn't have time to fix, such as access violations when you try to
// delete row in tableview that you add recently, or when you try to create table which name is consist
// of more then one word and such.
