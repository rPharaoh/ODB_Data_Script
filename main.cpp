#include <QCoreApplication>
#include <QtSql>

void CreateFileWithHTMLCode(QString ProductName);
bool MatchingFolderNameWithProductName(QString folderName);

/*
 *  Globle variables
 *  for file and folder path
 */

QString FolderPath = "E:/Projects/Gallopmedia Work/Eltamimi/Data & Photos/";
QString DatabasePath = "E:/Projects/Gallopmedia Work/Eltamimi/";
QString TemplatePath = DatabasePath;
QString TemplateName = "template.html";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

   /*
    * Database Connection
    * Database Name: Eltamime.accdb
    */

    QString DatabaseName = "Eltamime.accdb";
    QString DatabaseFullPath = DatabasePath + DatabaseName;
    QString DSN = "Driver={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DSN='';DBQ=" + DatabaseFullPath;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
            db.setDatabaseName(DSN);
            if (!db.open()) {
                qDebug() << "MS Access Database Connection Error! " << db.lastError().text();
                return false;
            }
            else {
                qDebug() << "Connection Success!;";

                /*
                 * Get proudct name
                 */

                QSqlQuery query;
                QString qry = "SELECT * FROM Eltamime";
                query.exec(qry);

                /*
                 * Colums Name:
                 * ID = 0, Product Name = 1, Network = 2, Launch = 3
                 * Body = 4, Display = 5, Platform = 6, Memory = 7
                 * Camera = 8, Sound = 9, Comms = 10, Features = 11
                 * Battery = 12, MISC = 13, Tests = 14
                 */

                while(query.next()) {
                    //qDebug() << "Product ID:" << query.value(0).toInt() << ", "
                    //         << "Product Name:" << query.value(1).toString();

                    if(MatchingFolderNameWithProductName(query.value(1).toString()) == true) {

                        // Set Table Header to QString
                        QString ProductName = query.value(1).toString(); QString Network = query.value(2).toString();
                        QString Launch = query.value(3).toString(); QString Body = query.value(4).toString();
                        QString Display = query.value(5).toString(); QString Platform = query.value(6).toString();
                        QString Memory = query.value(7).toString(); QString Camera = query.value(8).toString();
                        QString Sound = query.value(9).toString(); QString Comms = query.value(10).toString();
                        QString Features = query.value(11).toString(); QString Battery = query.value(12).toString();
                        QString MISC = query.value(13).toString(); QString Tests = query.value(14).toString();


                        // if return true
                        // get full path
                        qDebug() << "Folder is OK!";


                        QRegularExpression reA("Nano-SIM");

                        QRegularExpressionMatchIterator i = reA.globalMatch(Body);
                        while (i.hasNext()) {
                            QRegularExpressionMatch match = i.next();
                            if (match.hasMatch()) {
                                 qDebug() << match.captured();
                            }
                            else {
                                qDebug() << "No match Found!";
                            }
                        }



//                        /*
//                         * loop data from database
//                         * get regex for brack line in receved data
//                         * stream it to file with html code
//                         */

//                        QFile DataFile(DatabasePath+"Data & Photos/"+ProductName+"/"+"DataFile.txt");
//                        if (DataFile.open(QFile::WriteOnly | QFile::Text | QIODevice::Append)) {

//                            /*
//                             * Starting the template file for stream in
//                             */

//                            QTextStream StreamToFile(&DataFile);

//                            /*
//                             * Writing start of the file schema
//                             * in html form from another file
//                             */

//                            QFile HeaderFile(TemplatePath+TemplateName);
//                            if(HeaderFile.open((QIODevice::ReadOnly | QIODevice::Text))) {
//                                QTextStream StreamFromFile(&HeaderFile);
//                                while(!StreamFromFile.atEnd()) {
//                                    StreamToFile << StreamFromFile.readLine();
//                                }

//                                HeaderFile.flush();
//                                HeaderFile.close();
//                            }

//                            /*
//                            * Starting schama afer the template header
//                            */

//                            StreamToFile << "<table border='0'>";

//                            /*
//                            * Starting schama for the Data from the Database
//                            */

//                            /*
//                             * Geting every \n brake line from all data
//                             * and sterating it for each <tr><td>
//                             */

//                            QRegularExpression reA("(<bar [se]>[^<]+</bar>)");

//                            QRegularExpressionMatchIterator i = reA.globalMatch(Network);
//                            while (i.hasNext()) {
//                                QRegularExpressionMatch match = i.next();
//                                if (match.hasMatch()) {
//                                     qDebug() << match.captured(0);
//                                }
//                            }

//                            StreamToFile << "<tr>"
//                                            "      <th rowspan='{RowNumber}'>" << ProductName << "</th>"
//                                            "      <tr><td>Technology	 : GSM / HSPA / LTE</td></tr>"
//                                            "      <tr><td>Speed	 : HSPA 42.2/5.76 Mbps, LTE Cat4 150/50 Mbps </td></tr>"
//                                            "      <tr><td>GPRS	 : Yes </td></tr>"
//                                            "      <tr><td>EDGE : 	Yes </td></tr>"
//                                            "</tr>";

//                            /*
//                             * Writing end of file schema
//                             * in html form
//                             */

//                            StreamToFile << "</table>";

//                            // Flushing the file
//                            // and closing it

//                            DataFile.flush();
//                            DataFile.close();
//                            qDebug() << "file done!";

//                        }

                    }
                    else {
                        qDebug() << "Folder dose not exist!";
                    }
                }

                return true;
            }
    return a.exec();
}

bool MatchingFolderNameWithProductName(QString folderName) {

   /*
    * Check if folder exist or not
    * if exist return true else false
    */

    QString DirFullPath = FolderPath + folderName;

    if(QDir(DirFullPath).exists()) {
        return true;
    }
    else { return false; }
}

void CreateFileWithHTMLCode(QString ProductName) {

    /*
     * Read head file that
     * Contain CSS data
     * and put it in data.txt
     * file in the same proudct
     * folder name
     */

    QString DataFileName = "DataFile.txt";
    QString filename = TemplatePath + ProductName + "/" + DataFileName;


   /*
    * Append DataFile.txt file
    * then put the data from db
    * in it around the HTML code
    */

    QFile file(filename);
    // Trying to open in WriteOnly and Text mode
    if(!file.open(QFile::WriteOnly |
                  QFile::Text))
    {
        qDebug() << " Could not open file for writing";
        return;
    }

   /*
    * To write text, we use operator<<(),
    * which is overloaded to take
    * a QTextStream on the left
    * and data types (including QString) on the right
    */

    QTextStream out(&file);
    out << "QFile";
    file.flush();
    file.close();
}
