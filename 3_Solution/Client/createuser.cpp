#include "createuser.h"
#include "ui_createuser.h"

#include <QLayout>
#include <QComboBox>
#include <QLineEdit>

#define DEFAULT_BUFFLEN 512

void placePair(const QString& string, createuser* parent,QLabel*& qlabelptr, QLineEdit*& qlineeditptr, const QRect& qrectlabel, const QRect& qrectlineedit)
{
    qlabelptr = new QLabel("", parent);
    qlabelptr->setText(string);
    qlabelptr->setStyleSheet("color: #333; font-weight: bold; font-size: 14px;");
    qlabelptr->setGeometry(qrectlabel);
    qlineeditptr = new QLineEdit(parent);
    qlineeditptr->setFixedSize(250, 20);
    qlineeditptr->setGeometry(qrectlineedit);
    qlineeditptr->setStyleSheet("background-color: #f5f5f5; border: 1px solid #ccc;");
}


createuser::createuser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::createuser)
{
    ui->setupUi(this);

    ui->comboBox->setEditable(false);
    ui->comboBox->setPlaceholderText("Select type");
    ui->comboBox->addItem("Student");
    ui->comboBox->addItem("Professor");

    placePair("Group:",this, m_groupLabel, m_groupLineEdit, *new QRect(405,375,200,50), *new QRect(480,390,200,50));
    placePair("Username:",this, m_nameLabel, m_nameLineEdit, *new QRect(405, 275, 200, 50), *new QRect(480,290,200,50));
    placePair("Password",this, m_passwordLabel, m_passwordLineEdit, *new QRect(405, 325, 200, 50), *new QRect(480,340,200,50));

    m_groupLabel->setVisible(false);
    m_groupLineEdit->setVisible(false);
    m_nameLabel->setVisible(false);
    m_nameLineEdit->setVisible(false);
    m_passwordLabel->setVisible(false);
    m_passwordLineEdit->setVisible(false);

    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &createuser::on_comboBox_currentTextChanged);
}

createuser::~createuser()
{
    delete ui;
}

void createuser::on_pushButton_clicked()
{
    ptr->show();
    this->deleteLater();
    send(ConnectSocket, "back", DEFAULT_BUFFLEN, 0);
}

void createuser::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Student")
    {
        m_nameLineEdit->setText("");
        m_passwordLineEdit->setText("");
        m_groupLineEdit->setText("");
        m_groupLabel->setVisible(true);
        m_groupLineEdit->setVisible(true);
        m_nameLabel->setVisible(true);
        m_nameLineEdit->setVisible(true);
        m_passwordLabel->setVisible(true);
        m_passwordLineEdit->setVisible(true);

    }
    if(arg1 == "Professor")
    {
        m_nameLineEdit->setText("");
        m_passwordLineEdit->setText("");
        m_groupLabel->setVisible(false);
        m_groupLineEdit->setVisible(false);
        m_nameLabel->setVisible(true);
        m_nameLineEdit->setVisible(true);
        m_passwordLabel->setVisible(true);
        m_passwordLineEdit->setVisible(true);
    }
}

void createuser::on_registerButton_clicked()
{
    if(ui->comboBox->currentText() == "Student")
    {
        QString usernameptr = this->m_nameLineEdit->text();
        QString passwordptr = this->m_passwordLineEdit->text();
        QString typeptr = "student";
        QString groupptr = this->m_groupLineEdit->text();
        send(ConnectSocket, usernameptr.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
        send(ConnectSocket, passwordptr.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
        send(ConnectSocket, typeptr.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
        send(ConnectSocket, groupptr.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
    }
    if(ui->comboBox->currentText() == "Professor")
    {
        QString usernameptr = this->m_nameLineEdit->text();
        QString passwordptr = this->m_passwordLineEdit->text();
        QString typeptr = "profesor";


        send(ConnectSocket, usernameptr.toLocal8Bit(),DEFAULT_BUFFLEN , 0);
        send(ConnectSocket, passwordptr.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
        send(ConnectSocket, typeptr.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
        send(ConnectSocket, "NULL", DEFAULT_BUFFLEN, 0);
    }
    ptr->show();
    this->deleteLater();
}

