#ifndef SQLITEDB
#define SQLITEDB
#include <string>
#include <QtSql/QSqlDatabase>
#include <QtGui>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QtDebug>
#include <QTextStream>
#include <QSqlTableModel>

struct DBTemplate // this structure is to ease creation of new database
{
    QString dbname;
    QString tablename;
    int fields_count;
    QStringList fields_names;
};

class SQLiteDB // this class is designed for working with database. Obviously 8)
{
public:
    SQLiteDB();
    ~SQLiteDB();
    void establish_connection(const QString &); // only establishes connection with db, nothing more
    void establish_connection(DBTemplate &); // establishes connection with db and applies attached table to table model
    void set_table (const QString&); // sets table named in parameter to table model
    void insert_record();
    void delete_record(int);
    bool ContainsMultipleTables(); // checks if database contains more then 1 table
    void submit();              // saves changes to table
    QStringList GetTablesNames();
    QSqlTableModel *get_model(){return this->model;}
    static bool database_opened; // determines whether the database is opened at the moment
private:
    QSqlDatabase m_db;
    QSqlTableModel *model;
};
#endif // SQLITEDB

