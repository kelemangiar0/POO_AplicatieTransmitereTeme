#include "afterlogin.h"
#include <ui_afterlogin.h>
#include "./ui_afterlogin.h"
#include <QLabel>
#include <QEvent>

#include <QFileDialog>
#include <QPushButton>

#include <QListWidgetItem>

#include <fstream>

#define DEFAULT_BUFFLEN 512

afterlogin::afterlogin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::afterlogin)
{
    ui->setupUi(this);
    // ConnectSocket = INVALID_SOCKET;
    m_filePath = "";
    ui->usernamelabel->setText(m_username);
    ui->uploadButton->hide();
    ui->detailsWidget->hide();

    ui->chooseButton->hide();


    ui->idLabel->hide();
    ui->statusLabel->hide();
    ui->titleLabel->hide();
    ui->groupLabel->hide();

    ui->reviewButton->hide();
}


void afterlogin::setUsername(QString newusername)
{
    m_username = newusername;
    ui->usernamelabel->setText(m_username);
}



void afterlogin::on_toolButton_clicked()
{


    send(this->ConnectSocket, "logout",50, 0);
     parinte->show();
     this->deleteLater();

}

afterlogin::~afterlogin()
{
    delete ui;
}





void afterlogin::addTitlesToWidget(QList<QString>& obj)
{
    for(auto& it : obj)
    {
        ui->homeworksWidget->addItem(it);
    }
}

void afterlogin::on_homeworksWidget_itemClicked(QListWidgetItem *item)
{
    ui->uploadButton->hide();
    ui->idLabel->show();
    ui->statusLabel->show();
    ui->titleLabel->show();
    ui->groupLabel->show();


    ui->detailsWidget->clear();
    ui->detailsWidget->show();
    ui->homeworksWidget->move(105, 130);

    QString status;
    for(auto& it : homeworksdetails)
    {
        if(it.contains(item->text()))
        {

            int lastSpaceIndex = it.lastIndexOf(" ");
             status = it.mid(lastSpaceIndex + 1);


            int firstSpaceIndex = it.indexOf(" ");
            QString firstWord = it.left(firstSpaceIndex);
            QString idlabeltext = "Homework ID: ";
            idlabeltext += firstWord;
            ui->idLabel->setText(idlabeltext); // homework id


             lastSpaceIndex = it.lastIndexOf(" ");
            if (lastSpaceIndex != -1) {
                int secondLastSpaceIndex = it.lastIndexOf(" ", lastSpaceIndex - 1);
                if (secondLastSpaceIndex != -1) {
                    QString secondLastWord = it.mid(secondLastSpaceIndex + 1, lastSpaceIndex - secondLastSpaceIndex - 1);

                    QString text = "Term: " + secondLastWord;
                }
            }

            break;
        }
    }
    if(status == "Incomplet")
    { ui->uploadButton->show();
      ui->chooseButton->show();
      ui->reviewButton->hide();
    }
    else
    {
    ui->uploadButton->hide();
    ui->chooseButton->hide();
    ui->reviewButton->show();
    }


    QString newstatus = "Status: " + status;
    ui->statusLabel->setText(newstatus); // status



 }

void afterlogin::on_chooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select file"), "/", tr("All Files (*)"));
        if (!fileName.isEmpty()) {

            ui->chooseButton->setText(fileName);
            m_filePath = fileName;
        }
        ui->uploadButton->show();
}



void afterlogin::on_uploadButton_clicked()
{

            send(ConnectSocket, "uploadFile", 512, 0);
            std::string filename = m_filePath.toStdString();
            std::string ext = filename.substr(filename.find_last_of(".") + 1);
            std::string message = "uploadFile " + ext;

            //send la hwID
            QString str = ui->idLabel->text();
            int firstSpaceIndex = str.indexOf(" ");
            if (firstSpaceIndex != -1) {
            int secondSpaceIndex = str.indexOf(" ", firstSpaceIndex + 1);
            if (secondSpaceIndex != -1) {
            QString thirdWord = str.mid(secondSpaceIndex + 1, str.indexOf(" ", secondSpaceIndex + 1) - secondSpaceIndex - 1);
                send(ConnectSocket, thirdWord.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
            }
            }

             send(ConnectSocket, ext.c_str(), 4, 0);

            int bytesSent = 0 ;
            if (bytesSent == SOCKET_ERROR) {

                return;
            }


            std::ifstream inFile(filename, std::ios::binary);
            if (!inFile) {

                return;
            }
            const int dimension = 65536;
            char buffer[dimension];
            while (inFile.read(buffer, sizeof(buffer)) || inFile.gcount()) {
                int bytesToWrite = (int)inFile.gcount();
                if (inFile.eof() && bytesToWrite < sizeof(buffer)) {

                    buffer[bytesToWrite] = '\0';
                    bytesToWrite++;
                }
                bytesSent = send(ConnectSocket, buffer, bytesToWrite, 0);
                if (bytesSent == SOCKET_ERROR) {

                    return;
                }
                Sleep(1);
            }
            Sleep(1000);
            send(ConnectSocket, "gata", 50, 0);


}

