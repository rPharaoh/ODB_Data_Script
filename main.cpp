/*
* Author: Nader Nabil
* Date: 23/01/2016
* Description: A Script for wrapping data from MS Access
* database around html code to be put in e-commerce website
* using template.html as header containing CSS code and
* DataFile.txt as a final data
*/

#include <QCoreApplication>
#include <QtSql>

void CreateFileWithHTMLCode(QString ProductName);
bool MatchFolderName(QString folderName);
void setColumnNames();

/*
 *  Globle variables
 *  for files and folders path
 *
 *  FolderPath: folder that containe images data
 *  and will put the generated files into them
 *
 *  DatabasePath: the path of the database that will
 *                extract from it the data
 *
 *  TemplatePath: the header or the head of the wrapping file
 *
 *  TemplateName and DataFileName for the file names
 *
 *  Every thing stand for it self, hope so
 */

//#define PRODUCTION
#define DEVELOPMENT

#ifdef PRODUCTION
    #define DataFileName "DataFile.txt"
    #define ProductPath ""
    #define DbFolderPath  ""
#endif

#ifdef DEVELOPMENT
    #define DataFileName "DataFile.html"
    #define ProductPath ""
    #define DbFolderPath  ""
#endif

//folder path for the product name
QString FolderPath = ProductPath;

//folder path for the database
QString DatabasePath = DbFolderPath;
QString TemplatePath = DatabasePath;
QString TemplateName = "template.html";
QString DatabaseName = "{Name}.accdb";
QString DatabaseFullPath = DatabasePath + DatabaseName;

QList<QString> ColumnNames;
int RecordEnd = 13;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


   /*
    * Activating this stupid code
    * for setting all the table names
    */
    setColumnNames();

   /*
    * Database Connection
    * Database Name: {name}.accdb
    */
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
                 * Colums Name:
                 * ID = 0, Product Name = 1, Network = 2, Launch = 3
                 * Body = 4, Display = 5, Platform = 6, Memory = 7
                 * Camera = 8, Sound = 9, Comms = 10, Features = 11
                 * Battery = 12, MISC = 13, Tests = 14
                 */

                QSqlQuery query;
                QString qry = "SELECT * FROM {Table Name}";
                query.exec(qry);

                while(query.next()) {

                    if(MatchFolderName(query.value(1).toString()) == true) {
                        QString ProductName = query.value(1).toString();
                        // if return true
                        // get full path
                        qDebug() << "Folder is OK!";
                        qDebug() << "Writing html code...";

                        /*
                         * loop data from database
                         * get regex for brack line in receved data
                         * stream it to file with html code
                         */

                        QFile DataFile(DatabasePath+"Data & Photos/"+ProductName+"/"+DataFileName);
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

                                // RecordStart+1 for avoiding ID section in database
                                QRegularExpressionMatchIterator i = reA.globalMatch(query.value(RecordStart+1).toString());
                                while (i.hasNext()) {
                                    QRegularExpressionMatch match = i.next();
                                    if (match.hasMatch()) {
                                         //qDebug() << "<tr><td>" << match.captured(0) << "<tr><td>";
                                         trtdList.push_back(match.captured(0));
                                         checkItems++;
                                    }
                                }

                                StreamToFile << "<tr>"
                                             << "<th rowspan='" << checkItems+1 << "'>"
                                             << ColumnNames.at(RecordStart) << "</th>";

                                for(int j=0;j<trtdList.count();j++)
                                {
                                        StreamToFile << "<tr><td>" << trtdList.at(j) << "</td></tr>";
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
                            qDebug() << "Writing done!";
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

bool MatchFolderName(QString folderName) {

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
    * This will be refactored later
    * will streem file data from a loop
    */

    /*
     * Read head file that
     * Contain CSS data
     * and put it in data.txt
     * file in the same proudct
     * folder name
     */

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
        qDebug() << "Could not open file for writing";
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

    if(!EOF) {
        file.flush();
        file.close();
    }
}


void setColumnNames()
{
   /*
    * Staticlly added to avoid lots of codes
    * will be modifyed later
    */

    ColumnNames.push_front("TESTS");
    ColumnNames.push_front("MISC");
    ColumnNames.push_front("Battery");
    ColumnNames.push_front("Features");
    ColumnNames.push_front("COMMS");
    ColumnNames.push_front("Sound");
    ColumnNames.push_front("Camera");
    ColumnNames.push_front("Memory");
    ColumnNames.push_front("Platform");
    ColumnNames.push_front("Display");
    ColumnNames.push_front("Body");
    ColumnNames.push_front("Launch");
    ColumnNames.push_front("Network");
    ColumnNames.push_front("Product Name");

}
