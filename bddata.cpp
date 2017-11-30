#include "bddata.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QDialog>
#include <QTextStream>
#include <QDebug>
#include <stdlib.h>
QTextStream cout(stdout);
#include <QSql>
#include <QSqlRecord>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
/*BDData::BDData()
{

}*/
BDData::BDData(QObject *parent): QAbstractTableModel(parent)
{

}
QVariant BDData::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
            return  QVariant();
    else
            return data1.at(index.row()).at(index.column());
    //return QAbstractItemModel::data(index,role);
}

int BDData::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return rows;
  /*  if (parent.isValid()) return 0;
    return 1;//data1.count();*/

}

int BDData::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return cols;   
  //  return 2;//column_name.count();
}
QVariant BDData::headerData(int section, Qt::Orientation orientation, int role) const //???
{
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
                if (section < column_name.count())
                        return column_name[section];
                else
                    return QVariant();
        return QAbstractItemModel::headerData(section, orientation, role);
}

void BDData::load_from_csv(QString file_name1){
    if(cols!=0)
    {
        cols=0;rows=0; data1.clear();column_name.clear();type_data.clear();
    }
    {
    //file_name1="/home/student/qt_project/convec/basa.csv";
    QFile file(file_name1);
       if (!file.open(QIODevice::ReadOnly))
       {
         //  QMessageBox::critical(,tr("Error"),tr("Could not open file"));
           qDebug()<<"error from open file"<<endl;;
           return;
       }
       QTextStream in(&file);
      QString col_name= in.readLine();

      for (QString item : col_name.split(";")) {
                      column_name.append(item);

                      cols++;
                  //    qDebug()<<item<<cols;
      }
      while (!in.atEnd())
            {
                // ... построчно
                QString line = in.readLine();
                // Добавляем в модель по строке с элементами
              //  QList<QStandardItem *> standardItemsList;
                QList<QString> temp;
               // int k=-1;
                // учитываем, что строка разделяется точкой с запятой на колонки
                for (QString item : line.split(";")) {
                    //if(k==cols+1)k=0; else k++;
                //    data1[1][k].append(item);
                    temp.append(item);
                //            qDebug()<<item;
                }
                data1.append(temp); temp.clear();
                qDebug()<<data1[rows];
               // csvModel->insertRow(csvModel->rowCount(), standardItemsList);
          rows++;
      }
                  // for(int i=0;i<5;i++)   qDebug()<<column_name[i];

        //   qDebug()<<rows;
       file.close();
    }
   // else qDebug()<<"also data is in prog";
}

void BDData::output_in_csv(QString filename){
    if(cols==0){qDebug()<<"No data in prog";}
   /* QString name="";
    for (int i=0;i<cols;i++)
    name+=column_name[i];*/
    //filename="/home/student/qt_project/convec/basa_out.csv"; //delete
    QFile file(filename); //заменить на filename
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream text_stream(&file);
        QStringList str;
        for(int k=0;k<cols;k++){
            str<<column_name[k];
        }
        text_stream<<str.join(';')+"\n";
        for (int i=0;i<rows;i++)
        {
            str.clear();
            for (int j =0;j<cols;j++){
                QList<QString> temp;
            temp=data1[i];
                str<<temp[j];

            }
            text_stream<<str.join(';')+"\n";
        }

        file.close();
    }
    else qDebug()<<"error open file to write";
}

void BDData::opred_data_type() //INTEGER -> REAL -> TEXT
{
QList <QString>temp,d;bool qw[cols];
for (int i=0;i<cols;i++){
    temp.append("TEXT");
    type_data.append("INTEGER");
    qw[i]=false;
}
for (int i=0;i<rows;i++)
    for(int j=0;j<cols;j++)
    {
        d.clear();
        d=data1[i];
        bool flag;
        d[j].toInt(&flag);
        if(flag) temp[j]="INTEGER";
        else {d[j].toDouble(&flag);
            if(flag) {temp[j]="REAL";qw[j]=true;}
            else {temp[j]="TEXT"; type_data[j]="TEXT";}
    }
for(int i=0;i<cols;i++)
{
    if(qw[i] &&(type_data[i]!="TEXT")) type_data[i]="REAL";
   // else if(type_data[i]!="TEXT") type_data[i]="INTEGER";
}
}
qDebug()<<temp;
qDebug()<<type_data;
}
/*
void BDData::opred_data_type() //INTEGER -> REAL -> TEXT
{
int32_t Int_List[rows];
double Double_List[rows];
QList<QString> Text;
for (int j = 0; j < cols; j++)
{
    bool f = true;
    for (int i = 0; i < rows; i++)
    {
        bool isNum = toInt(data1[i][j]);
        qDebug() << data1[i][j] << " " << isNum << "\n";
    }
}
}
*/
/*QList <QString>temp,d;bool qw[cols];
for (int i=0;i<cols;i++){
    temp.append("TEXT");
    type_data.append("INTEGER");
    qw[i]=false;
}
for (int i=0;i<rows;i++)
    for(int j=0;j<cols;j++)
    {
        d.clear();
        d=data1[i];
        bool flag;
        d[j].toInt(&flag);
        if(flag) temp[j]="INTEGER";
        else {d[j].toDouble(&flag);
            if(flag) {temp[j]="REAL";qw[j]=true;}
            else {temp[j]="TEXT"; type_data[j]="TEXT";}
    }
for(int i=0;i<cols;i++)
{
    if(qw[i] &&(type_data[i]!="TEXT")) type_data[i]="REAL";
   // else if(type_data[i]!="TEXT") type_data[i]="INTEGER";
}
}
qDebug()<<temp;
qDebug()<<type_data;
}
*/

void BDData::load_from_sql(QString filename, QString name_of_table)
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);
    if( !db.open() ) {
        qDebug() <<  "Cannot open database:" << db.lastError().text();
        return;
    }
    QSqlRecord rec = db.record(name_of_table);
    if (rec.isEmpty())
    {
        qDebug() << "Cannot found table" + name_of_table;
        return;
    }
    if (cols != 0){
       cols=0;
       rows=0;
       data1.clear();
       column_name.clear();
       type_data.clear();
     }

    beginInsertColumns(QModelIndex(), 0, rec.count()-1);
    for(int j = 0; j < rec.count(); j++)
    column_name.append(rec.fieldName(j));
    endInsertColumns();
    cols = rec.count();

    QList<QString> temp;
    QSqlQuery countRows;
    countRows.exec("SELECT count(*) FROM " + name_of_table);
    countRows.last();
    qDebug() << countRows.value(countRows.at()).toString();
    rows = countRows.value(countRows.at()).toInt();
    cout << rows;
    QSqlQuery query("SELECT * FROM " + name_of_table);

    emit beginInsertRows(QModelIndex(), 0, rows-1);
    while (query.next())
    {
        for (int j = 0; j < cols; j++)
              temp.append(query.value(j).toString());
              data1.append(temp);
              temp.clear();
     }
     emit endInsertRows();

     db.close();
     return ;
}

void BDData::output_in_sql(QString filename, QString name_of_table)
{
    opred_data_type();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);
    if (!db.open())
    {
        qDebug() << "Cannot open database:" + db.lastError().text();
        return;
    }
    //PRIMARY KEY NOT NULL
    QSqlQuery queru;
    QString str;
    str = "CREATE TABLE "+ name_of_table + "( ";
    str += "\'" + column_name[0] + "\' " + type_data[0] +" , ";
    for (int j = 1; j < cols-1; j++)
        str += "\'" + column_name[j] + "\' " + type_data[j]+", ";
    str += "\'" + column_name[cols-1] + "\' " + type_data[cols-1] +" );";
    qDebug() << str;
    if (!queru.exec(str)) {
        qDebug() << "Unable to create a table";
    }

    QSqlQuery query;
    QString strt;
    for (int i = 0; i < rows; i++)
    {
    strt = "INSERT INTO " + name_of_table+" (";
    for (int j = 0; j < cols-1; j++)
        strt += "\'" + column_name[j] + "\'" + ", ";
    strt += "\'" + column_name[cols-1] + "\'" + ")" + " VALUES (";

    if(type_data[0]=="TEXT")
        strt += "'" + data1[i][0] + "'" + ", ";
    else
        strt += data1[i][0] + ", ";
    //strt += "'" + data1[i][0] + "'" + ", ";
    for (int j = 1; j < cols-1; j++){
        if(type_data[j]=="TEXT")
        //if (data1[i][j] != "")
            strt += "'" + data1[i][j] + "'" + ", ";
        else
            strt = strt  + data1[i][j]  + ", ";
    }
        //qDebug() << data1[i][j] <<" , ";}
    if(type_data[cols-1]=="TEXT")
        strt += "'" + data1[i][cols-1]+ "'" + ");";
    else
        strt +=data1[i][cols-1] + ");";
    qDebug() << strt;
    if (!query.exec(strt)) {
        qDebug() << "Unable to do insert operation";
    }
    }
    /*
    //for (int j = 0; j < cols; j++)
    //    type_data[j] = "TEXT";
    QString qq="INSERT INTO '"+table_name+"' VALUES ";
    for(int i=0;i<rows;i++){
        QString tem;
        tem="";
        QList<QString> temp;
           temp=data1[i];
        for(int j=0; j<cols;j++){

              if(type_data[j]=="TEXT")
            tem+="'"+temp[j]+"'";
             else tem+=temp[j];
              if(j<cols-1)tem+=", ";
        }
      if(i<rows-1)  qq+="("+tem+"), ";else qq+="("+tem+")";
    }
       QSqlQuery qqq;
    if(!qqq.exec(qq)){ qDebug() << "DataBase: error of input data ";
        qDebug() << qqq.lastError().text();
        }
*/


    db.close();
}
QList<QString> BDData::get_column_name(){
    return column_name;
}

QList<QList<QString>> BDData::get_data(){
    return data1;
}

int BDData::get_row(){
    return rows;
}

int BDData::get_col(){
    return cols;
}
