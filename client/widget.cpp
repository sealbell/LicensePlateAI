#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include "tcp.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    if(ui->lineEdit->text()=="123" &&
            ui->lineEdit_2->text()=="123")
    {
        ca->show();
        this->close();
    }
    else
    {
        QMessageBox::critical(this,"错误","用户名或账号输入错误，请重新输入");
    }

}
