#include <QCoreApplication>
#include <QtSql>

void CreateFileWithHTMLCode(QString ProductName);
bool MatchingFolderNameWithProductName(QString folderName);
void setTableNames();

/*
 *  Globle variables
 *  for file and folder path
 */

QString FolderPath = "E:/Projects/Gallopmedia Work/Eltamimi/Data & Photos/";
QString DatabasePath = "E:/Projects/Gallopmedia Work/Eltamimi/";
QString TemplatePath = DatabasePath;
QString TemplateName = "template.html";
QList<QString> TableNames;
int RecordEnd = 14;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


   /*
    * Activating this stupid code
    * for setting all the table names
    */
    setTableNames();

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
                        QString ProductName = query.value(1).toString();
                        // if return true
                        // get full path
                        qDebug() << "Folder is OK!";

                        /*
                         * loop data from database
                         * get regex for brack line in receved data
                         * stream it to file with html code
                         */

                        QFile DataFile(DatabasePath+"Data & Photos/"+ProductName+"/"+"DataFile.html");
                        if (DataFile.open(QFile::WriteOnly | QFile::Text | QIODevice::Append)) {

                            /*
                             * Starting the template file for stream in
                             */

                            QTextStream StreamToFile(&DataFile);

                            /*
                             * Writing start of the file schema
                             * in html form from another file
                             */

                            QFile HeaderFile(TemplatePath+TemplateName);
                            if(HeaderFile.open((QIODevice::ReadOnly | QIODevice::Text))) {
                                QTextStream StreamFromFile(&HeaderFile);
                                while(!StreamFromFile.atEnd()) {
                                    StreamToFile << StreamFromFile.readLine();
                                }

                                HeaderFile.flush();
                                HeaderFile.close();
                            }

                            /*
                            * Starting schama afer the template header
                            */

                            StreamToFile << "<table border='0'>";

                            /*
                            * Starting schama for the Data from the Database
                            */

                            /*
                             * Geting every \n brake line from all data
                             * and sterating it for each <tr><td>
                             */

                            int RecordStart = 0;
                            while(RecordStart != RecordEnd) {
                                int checkItems = 0;
                                QList<QString> trtdList;

                                QRegularExpression reA("([A-Za-z0-9:\\s,/.&)(~\"\\n\\r][^$\\r\\n])+[^$\\r\\n]+");

                                QRegularExpressionMatchIterator i = reA.globalMatch(query.value(RecordStart).toString());
                                while (i.hasNext()) {
                                    QRegularExpressionMatch match = i.next();
                                    if (match.hasMatch()) {
                                         //qDebug() << "<tr><td>" << match.captured(0) << "<tr><td>";
                                         trtdList.push_back(match.captured(0));
                                         checkItems++;
                                    }
                                }

                                if(RecordStart == 0) {
                                    StreamToFile << "<tr>"
                                                 << "<th>" << TableNames.at(RecordStart) << "</th>";
                                } else {

                                StreamToFile << "<tr>"
                                             << "<th rowspan='" << checkItems+1 << "'>" << TableNames.at(RecordStart) << "</th>";
                                }

                                for(int j=0;j<trtdList.count();j++)
                                {
                                    StreamToFile << "<tr><td>" << trtdList.at(j) << "</tr></td>";
                                }

                                StreamToFile << "</tr>";
                                RecordStart++;

        }

                            /*
                             * Writing end of file schema
                             * in html form
                             */

                            StreamToFile << "</table>";

                            // Flushing the file
                            // and closing it

                            DataFile.flush();
                            DataFile.close();
                            qDebug() << "file done!";

                        }

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


void setTableNames()
{
   /*
    * Staticlly added to avoid lots of codes
    * will be modifyed later
    */

    TableNames.push_front("TESTS");
    TableNames.push_front("MISC");
    TableNames.push_front("Battery");
    TableNames.push_front("Features");
    TableNames.push_front("COMMS");
    TableNames.push_front("Sound");
    TableNames.push_front("Camera");
    TableNames.push_front("Memory");
    TableNames.push_front("Platform");
    TableNames.push_front("Display");
    TableNames.push_front("Body");
    TableNames.push_front("Launch");
    TableNames.push_front("Network");
    TableNames.push_front("Product Name");

}
