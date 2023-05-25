#include "viewhomework.h"
#include "ui_viewhomework.h"
#include "QDate"
#include <QStringList>
#include <fstream>
#define DEFAULT_BUFFLEN 512
viewhomework::viewhomework(QWidget *parent, QList<QString>& list) :
    QMainWindow(parent),
    ui(new Ui::viewhomework)
{
    ui->setupUi(this);

    ui->homeworksWidget->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->homeworksWidget->setFocusPolicy(Qt::NoFocus);
    ui->studentsWidget->hide();
    ui->downloadButton->hide();
    ui->reportWidget->hide();
    ui->gradeLineEdit->hide();
    ui->markButton->hide();
    ui->reportButton->hide();
    setHomeworkList(list);
    setWidgetItems(homeworkslist, ui->homeworksWidget);


}
void viewhomework::setHomeworkList(QList<QString>& list)
{
    for(auto& x: list)
    {
        this->homeworkslist.push_back(x);
    }
}


viewhomework::~viewhomework()
{
    delete ui;
}

void viewhomework::setWidgetItems(QList<QString>& newqwidgetitems, QListWidget*& listwidgetptr)
{
    listwidgetptr->clear();
    for(auto& x : newqwidgetitems)
    {
        listwidgetptr->addItem(x);
    }

}

void viewhomework::on_homeworksWidget_itemClicked(QListWidgetItem *item)
{
    char statusbuffer[DEFAULT_BUFFLEN];
    ui->reportButton->hide();
    ui->studentsWidget->show();
    ui->homeworksWidget->move(105, 130);
    ui->selecthomeworkLabel->hide();
    ui->downloadButton->hide();
    ui->markButton->hide();
    ui->reportWidget->hide();
    ui->reportWidget->clear();
    ui->gradeLineEdit->hide();
    ui->gradeLineEdit->setText("Grade");
    ui->studentsWidget->clear();
    QString text = item->text();
    QStringList words = text.split(" "); // Impartim textul in cuvinte
    QString firstWord = words[0];

      // numar, user, grupa, status
    send(ConnectSocket, firstWord.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
       Sleep(10);

    this->homeworkID = firstWord;
    char numberStudents[DEFAULT_BUFFLEN];
    recv(ConnectSocket, numberStudents, DEFAULT_BUFFLEN, 0);

    for(int i =0; i< atoi(numberStudents); i++)
    {
        char usernamebuffer[DEFAULT_BUFFLEN];
        char groupbuffer[DEFAULT_BUFFLEN];
        char statusbuffer[DEFAULT_BUFFLEN];

        recv(ConnectSocket, usernamebuffer, DEFAULT_BUFFLEN, 0);
        Sleep(1);
        recv(ConnectSocket, groupbuffer, DEFAULT_BUFFLEN, 0);
        Sleep(1);
        recv(ConnectSocket, statusbuffer, DEFAULT_BUFFLEN, 0);
        Sleep(1);
        QString usernamestring(usernamebuffer);
        QString groupstring(groupbuffer);
        QString statusstring(statusbuffer);

        QString finalstring = usernamestring + ' ' + groupstring + ' ' + statusstring;
        ui->studentsWidget->addItem(finalstring);
    }


    recv(ConnectSocket, statusbuffer, 15, 0);
    Sleep(10);
    //send(ConnectSocket,  this->homeworkID.toLocal8Bit(), DEFAULT_BUFFLEN, 0);

    QString status(statusbuffer);
    if(status != "cannotGenerate")
    {
        ui->reportButton->show();
    }

}




void viewhomework::on_studentsWidget_itemClicked(QListWidgetItem *item)
{
    ui->reportWidget->clear();
    ui->reportWidget->hide();
    QString itemtext = item->text();
    QStringList words = itemtext.split(' ');
    QString lastWord = words.last();
    ui->gradeLineEdit->setText("Grade");
    if(lastWord == "Complet")
    {
        QString text = "Download for ";
        QString username = words.at(0);
        this->username = username;
        ui->downloadButton->setText(text + username);
        ui->downloadButton->show();
        ui->gradeLineEdit->show();
        ui->markButton->show();
    }
    if(lastWord == "Incomplet")
    {
        ui->downloadButton->hide();
        ui->gradeLineEdit->hide();
        ui->markButton->hide();
    }


}


void viewhomework::on_backButton_clicked()
{
    ptr->show();
    this->deleteLater();
    send(ConnectSocket, "back", DEFAULT_BUFFLEN, 0);
}


void viewhomework::on_downloadButton_clicked()
{
    send(ConnectSocket, "downloadFromServer", DEFAULT_BUFFLEN, 0);

    send(ConnectSocket, this->homeworkID.toLocal8Bit(),DEFAULT_BUFFLEN, 0);
    send(ConnectSocket, this->username.toLocal8Bit(),DEFAULT_BUFFLEN, 0);

    char path[DEFAULT_BUFFLEN];
    recv(ConnectSocket, path, DEFAULT_BUFFLEN, 0);

    QString pathstring(path);
    int pos = pathstring.lastIndexOf('\\');
    QString name = QString(pathstring.mid(pos + 1));




    // primire fisier
    char buffer[65536];
    int bytesReceived;

    QString filename = "C:\\POO\\" + name;
    ofstream outFile(filename.toStdString(), std::ios::binary);


    while ((bytesReceived = recv(ConnectSocket, buffer, sizeof(buffer), 0)) > 0)
    {

        if (strcmp(buffer, "gata") == 0)
        {
            break;
        }
        outFile.write(buffer, bytesReceived);
        std::cout << "Se primeste..." << endl;
        Sleep(1);
    }


    outFile.close();
   // recv(ConnectSocket, buffer, sizeof(buffer), 0);
    qDebug() << "s-a primit";
}


void viewhomework::on_markButton_clicked()
{
    QString grade = ui->gradeLineEdit->text();

    send(ConnectSocket, "marked",DEFAULT_BUFFLEN, 0);
    send(ConnectSocket, homeworkID.toLocal8Bit(),DEFAULT_BUFFLEN, 0);
    send(ConnectSocket, username.toLocal8Bit(),DEFAULT_BUFFLEN, 0);
    send(ConnectSocket, grade.toLocal8Bit(),DEFAULT_BUFFLEN, 0);


}


void viewhomework::on_reportButton_clicked()
{
    send(ConnectSocket, "generateReport",DEFAULT_BUFFLEN, 0);
    ui->reportWidget->show();

    char numberStudents[DEFAULT_BUFFLEN];
    recv(ConnectSocket, numberStudents, DEFAULT_BUFFLEN, 0);
    Sleep(1);


    for(int i =0; i< atoi(numberStudents); i++)
    {
        char name[DEFAULT_BUFFLEN];
        char grade[DEFAULT_BUFFLEN];

        recv(ConnectSocket, name, DEFAULT_BUFFLEN, 0);
        Sleep(1);
        recv(ConnectSocket, grade, DEFAULT_BUFFLEN, 0);
          Sleep(1);

        QString namestring(name);
        QString gradestring(grade);
        QString stringItem = namestring + " : " + gradestring;

        ui->reportWidget->addItem(stringItem);

    }

    char average[DEFAULT_BUFFLEN];
    recv(ConnectSocket, average, DEFAULT_BUFFLEN, 0);
    Sleep(1);
    QString text = "Group average grade : ";
    QString averageItem(average);
    text+=averageItem;

    ui->reportWidget->addItem(text);
    // afisare stil kennedy : Nume-Prenume : nota \n Medie grupa : medie

}

