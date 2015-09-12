#include <SQLiteDB.h>

bool SQLiteDB::database_opened = false;

SQLiteDB::SQLiteDB()
{
    this->m_db = m_db = QSqlDatabase::addDatabase("QSQLITE");
}

void SQLiteDB::submit()
{
    if(model->submitAll())
        model->database().commit();
    else
    {
        model->database().rollback();
        QMessageBox::critical(0,"Error",model->lastError().text());
    }
}

QStringList SQLiteDB::GetTablesNames()
{
    return m_db.tables();
}

void SQLiteDB::establish_connection(DBTemplate &templ8)
{
    m_db.setDatabaseName(templ8.dbname);
    m_db.open();

    QSqlQuery create_table;
    QString qstr = "";
    if (templ8.fields_count > 1)
    {
                            // parsing query string...
        qstr += "CREATE TABLE " + templ8.tablename + "(" + templ8.fields_names.at(0) + " TEXT, ";
        for (int i = 1; i < templ8.fields_count-1; i++)
            qstr += templ8.fields_names.at(i) + " TEXT, ";
        qstr += templ8.fields_names.at(templ8.fields_count-1) + " TEXT);";
    }
    else
        qstr += "CREATE TABLE " + templ8.tablename + "(" + templ8.fields_names.at(0) + " TEXT);";

    create_table.exec(qstr);

    model = new QSqlTableModel(0,m_db);

    model->setTable(templ8.tablename);
    model->insertColumns(0,templ8.fields_count);
    for (int i=0; i<templ8.fields_count; i++)
        model->setHeaderData(i,Qt::Horizontal,templ8.fields_names.at(i));
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->insertRow(0);
}

void SQLiteDB::establish_connection(const QString &dbname)
{
    m_db.setDatabaseName(dbname);
    m_db.open();

}

void SQLiteDB::set_table(const QString &tablename)
{
    model = new QSqlTableModel(0,m_db);

    model->setTable(tablename);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

}

bool SQLiteDB::ContainsMultipleTables()
{
    if (m_db.tables().size() > 1)
        return true;
    else
        return false;
}
void SQLiteDB::insert_record()
{
    model->insertRow(model->rowCount());
}

void SQLiteDB::delete_record(int row)
{
    model->removeRow(row);
}

SQLiteDB::~SQLiteDB()
{
    if (m_db.isOpen())
        m_db.close();
}
