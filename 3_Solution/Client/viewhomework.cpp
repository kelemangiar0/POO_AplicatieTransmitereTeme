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

    ui->studentsWidget->show();
    ui->homeworksWidget->move(105, 130);
    ui->selecthomeworkLabel->hide();
    ui->studentsWidget->clear();
    QString text = item->text();
    QStringList words = text.split(" "); // Impartim textul in cuvinte
    QString firstWord = words[0];

      // numar, user, grupa, status
    send(ConnectSocket, firstWord.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
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

}




void viewhomework::on_studentsWidget_itemClicked(QListWidgetItem *item)
{
    QString itemtext = item->text();
    QStringList words = itemtext.split(' ');
    QString lastWord = words.last();
    if(lastWord == "Complet")
    {
        QString text = "Download for ";
        QString username = words.at(0);
        this->username = username;
        ui->downloadButton->setText(text + username);
        ui->downloadButton->show();
    }
    else
    {
        ui->downloadButton->hide();
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
    qDebug() << path;
    char buftest[DEFAULT_BUFFLEN];
    // primire fisier
    char buffer[65536];
    int bytesReceived;
    ofstream outFile("C:\\POO\\fisier.pdf", std::ios::binary);
    while ((bytesReceived = recv(ConnectSocket, buffer, sizeof(buffer), 0)) > 0)
    {
        if (strcmp(buffer, "gata") == 0)
            break;

        outFile.write(buffer, bytesReceived);
        std::cout << "Se primeste..." << endl;
        Sleep(10);
    }
    // recv(ConnectSocket, buftest, DEFAULT_BUFFLEN, 0);

    outFile.close();
    qDebug() << "s-a primit";
}

