#include "form.h"
#include "ui_form.h"

void Menu::Create_db()
{
    if (SQLiteDB::database_opened)
    {
        DialogSave *sdlg = new DialogSave();
        if (sdlg->wnd->result() == 1)
            this->Save_db();
    }
    DialogForDBCreation *dlg = new DialogForDBCreation();
    if (dlg->wnd->result() == 1)
    {
        db.establish_connection(dlg->templ8);
        ui->tableView->setModel(db.get_model());
        ui->lineEdit->setText(dlg->templ8.tablename);
        SQLiteDB::database_opened = true;
    }
}

void Menu::Open_db()
{
    if (SQLiteDB::database_opened)
    {
        DialogSave *sdlg = new DialogSave();
        if (sdlg->wnd->result() == 1)
            this->Save_db();
    }
    QString filename = QFileDialog::getOpenFileName(0, "Открыть файл", QDir::currentPath());
    if (filename != "")
    {
        db.establish_connection(filename);
        if (db.ContainsMultipleTables())
        {
            DialogForDBOpening *dlg = new DialogForDBOpening(db.GetTablesNames());
            if (dlg->wnd->result() != 0)
            {
                db.set_table(dlg->table_name);
                ui->lineEdit->setText(dlg->table_name);
                ui->tableView->setModel(db.get_model());
                SQLiteDB::database_opened = true;
            }
        }
        else
        {
            db.set_table(db.GetTablesNames().at(0));
            ui->lineEdit->setText(db.GetTablesNames().at(0));
            ui->tableView->setModel(db.get_model());
            SQLiteDB::database_opened = true;
        }
    }
}

void Menu::Save_db()
{
    if (!SQLiteDB::database_opened)
        QMessageBox::warning(0,"Nothing to save!","Open/Create database first!");
    else
        db.submit();
}

void Menu::Add_r()
{
    if (!SQLiteDB::database_opened)
        QMessageBox::warning(0,"Can't do that!","Open/Create database first!");
    else
        db.insert_record();
}

void Menu::Del_r()
{
    if (!SQLiteDB::database_opened)
        QMessageBox::warning(0,"Can't do that!","Open/Create database first!");
    else
        if (!ui->tableView->selectionModel()->selectedIndexes().empty()) // if user selected some row/cell
            if (ui->tableView->selectionModel()->selectedIndexes().at(0).row() < db.get_model()->rowCount()) // and if such row exists in tableview
                                                                                      // 'cause when you add it to the model, it isn't added to tableview 'till you submit changes
                db.delete_record(ui->tableView->selectionModel()->selectedIndexes().at(0).row()); // mark first selected row for deletion
}

Menu::Menu(QMenuBar *menubar, Ui::Form *ui)
{

    this->ui = ui;
    QAction *Create_db = new QAction("&Create database", menubar);
    QAction *Open_db = new QAction("&Open database", menubar);
    QAction *Save_db = new QAction("&Save database", menubar);
    QAction *Add_r = new QAction("&Insert record", menubar);
    QAction *Del_r = new QAction("&Delete record", menubar);
    QAction *Exit = new QAction("&Exit", menubar);

    QMenu *file;
    QMenu *edit;

    file = menubar->addMenu("&File");
    edit = menubar->addMenu("&Edit");

    file->addAction(Create_db);
    file->addAction(Open_db);
    file->addAction(Save_db);
    file->addAction(Exit);

    edit->addAction(Add_r);
    edit->addAction(Del_r);

    connect(Create_db, SIGNAL(triggered()), this, SLOT(Create_db()));
    connect(Open_db, SIGNAL(triggered()), this, SLOT(Open_db()));
    connect(Save_db, SIGNAL(triggered()), this, SLOT(Save_db()));
    connect(Add_r, SIGNAL(triggered()), this, SLOT(Add_r()));
    connect(Del_r, SIGNAL(triggered()), this, SLOT(Del_r()));
    connect(Exit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
}

Form::Form(QWidget *parent) : QMainWindow(parent), ui(new Ui::Form)
{
    _menu=new Menu(Form::menuBar(), ui);
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

int DialogForDBCreation::SpnBxPrevVal = 1;

DialogForDBCreation::DialogForDBCreation()
{
    this->wnd = new QDialog();
    btn1 = new QPushButton();
    btn2 = new QPushButton();
    spnbx1 = new QSpinBox();
    lbl1 = new QLabel("Database name: ");
    lbl2 = new QLabel("Table name: ");
    lbl3 = new QLabel("Fields count: ");
    lbl4 = new QLabel("Field names: ");
    lnedit1 = new QLineEdit();
    lnedit2 = new QLineEdit();
    tblwdgt = new QTableWidget();

    wnd->setFixedSize(640,200);

    btn1->setFixedSize(100,20);
    btn1->setText("Create database");
    btn2->setFixedSize(50,20);
    btn2->setText("Cancel");

    spnbx1->setFixedSize(40,25);
    spnbx1->setValue(1);

    tblwdgt->setRowCount(1);
    tblwdgt->setColumnCount(1);
    tblwdgt->setItem(0,0,new QTableWidgetItem());

    connect(btn1, SIGNAL(clicked()), this, SLOT(onCreateDBBtn()));
    connect(btn2, SIGNAL(clicked()), this, SLOT(onCancelBtn()));
    connect(spnbx1, SIGNAL(valueChanged(int)), this, SLOT(onColCountChange()));
    QGridLayout* Grid = new QGridLayout;
    QHBoxLayout* row1 = new QHBoxLayout;
    QHBoxLayout* row2 = new QHBoxLayout;
    QHBoxLayout* row3 = new QHBoxLayout;
    QHBoxLayout* row4 = new QHBoxLayout;

    row1->addWidget(lbl1); row1->addWidget(lnedit1);
    row1->addWidget(lbl2); row1->addWidget(lnedit2);
    row1->addWidget(lbl3); row1->addWidget(spnbx1);

    row2->addWidget(lbl4);
    row2->setAlignment(lbl4, Qt::AlignHCenter);

    row3->addWidget(tblwdgt);

    row4->addWidget(btn1); row4->addWidget(btn2);
    Grid->addLayout(row1,0,0);
    Grid->addLayout(row2,1,0);
    Grid->addLayout(row3,2,0);
    Grid->addLayout(row4,3,0);

    wnd->setLayout(Grid);
    wnd->exec();

}

void DialogForDBCreation::onColCountChange()
{
    if (spnbx1->value() == 0)
        spnbx1->setValue(1);
    if (spnbx1->value() > SpnBxPrevVal)
    {
        tblwdgt->insertColumn(spnbx1->value()-1);
        tblwdgt->setItem(0,spnbx1->value()-1,new QTableWidgetItem());
    }
    else
        tblwdgt->removeColumn(spnbx1->value());
    SpnBxPrevVal = spnbx1->value();
}

bool DialogForDBCreation::onCreateDBBtn()
{

    if (lnedit1->text() == "" || lnedit2->text() == "")
    {
        QMessageBox::warning(0,"Can't do that!", "fill all the fields please!");
        return false;
    }
    else
        for (int i = 0; i < tblwdgt->columnCount(); i++)
            if (tblwdgt->item(0,i)->text() == "")
            {
                QMessageBox::warning(0,"Can't do that!", "fill all field names please!");
                return false;
            }
    if (!lnedit1->text().contains('.')) // this is to generate database extention if there is no one.
        templ8.dbname = lnedit1->text()+".db";
    else
        templ8.dbname = lnedit1->text();
    templ8.tablename = lnedit2->text();
    templ8.fields_count = tblwdgt->columnCount();
    for (int i = 0; i < tblwdgt->columnCount(); i++)
        templ8.fields_names.push_back(tblwdgt->item(0,i)->text());
    this->wnd->accept();
    return true;
}

void DialogForDBCreation::onCancelBtn()
{

    this->wnd->reject();
}

DialogForDBOpening::DialogForDBOpening(QStringList tables)
{
    this->wnd = new QDialog();
    wnd->setFixedSize(480,160);
    lbl = new QLabel("Selected database contains more than one table."
                     " Please select which one you wish to open:");
    btn1 = new QPushButton("Ok");
    btn2 = new QPushButton("Cancel");
    tblwdgt = new QTableWidget(1,tables.size());
        for (int i=0; i<tables.size(); i++)
        {
            tblwdgt->setItem(0,i,new QTableWidgetItem(tables.at(i)));
            tblwdgt->item(0,i)->setFlags(tblwdgt->item(0,i)->flags() ^ Qt::ItemIsEditable);
        }

    connect(btn1, SIGNAL(clicked()), this, SLOT(onOkBtn()));
    connect(btn2, SIGNAL(clicked()), this, SLOT(onCancelBtn()));

    QGridLayout *Grid = new QGridLayout;

    QHBoxLayout *row1 = new QHBoxLayout;
    QHBoxLayout *row2 = new QHBoxLayout;
    QHBoxLayout *row3 = new QHBoxLayout;

    row1->addWidget(lbl);
    row2->addWidget(tblwdgt);
    row3->addWidget(btn1); row3->addWidget(btn2);

    Grid->addLayout(row1,0,0);
    Grid->addLayout(row2,1,0);
    Grid->addLayout(row3,2,0);

    wnd->setLayout(Grid);
    wnd->exec();

}

bool DialogForDBOpening::onOkBtn()
{
    if (tblwdgt->selectedItems().empty())
    {
        QMessageBox::warning(0,"Can't do that!", "select table name please!");
        return false;
    }
    table_name = tblwdgt->selectedItems().at(0)->text();
    wnd->accept();
    return true;
}

void DialogForDBOpening::onCancelBtn()
{
    wnd->reject();
}

DialogSave::DialogSave()
{
    this->wnd = new QDialog();
    wnd->setFixedSize(150,100);
    lbl = new QLabel("Save current table?");
    btn1 = new QPushButton("Yes");
    btn2 = new QPushButton("No");


    connect(btn1, SIGNAL(clicked()), this, SLOT(onOkBtn()));
    connect(btn2, SIGNAL(clicked()), this, SLOT(onCancelBtn()));

    QGridLayout *Grid = new QGridLayout;

    QHBoxLayout *row1 = new QHBoxLayout;
    QHBoxLayout *row2 = new QHBoxLayout;

    row1->addWidget(lbl);
    row2->addWidget(btn1); row2->addWidget(btn2);

    Grid->addLayout(row1,0,0);
    Grid->addLayout(row2,1,0);

    wnd->setLayout(Grid);
    wnd->exec();

}

void DialogSave::onOkBtn()
{
    wnd->accept();
}

void DialogSave::onCancelBtn()
{
    wnd->reject();
}
