#include "mainwindow.h"

// 这个头文件是哪来的? 为什么需要 include 它? 头文件里的内容是什么意思?
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 连接按钮点击事件
    // 当 slot 命名为 `on_按钮名称_操作` 时, Qt 会自动将按钮与槽连接起来
    // 但还是建议显示地连接一下
    connect(ui->pushButton, &QPushButton::clicked, this,
            &MainWindow::on_pushButton_clicked);
}

MainWindow::~MainWindow() { delete ui; }

// 按下按钮的逻辑
void MainWindow::on_pushButton_clicked() {
    this->hide();  // 隐藏当前界面

    // 析构自身
    // 当一个成员函数调用后, 这个对象再也不会被使用, 则调用 deleteLater 析构
    // 但是这里不能调用, 为什么?
    // this->deleteLater();

    // 回答完上面的问题, 你还需要考虑新建一个窗口时是否使用 new
    // 统一的设计会为项目开发带来好处

    emit evokeGameSignal();  // 触发信号, 对应步骤(4)
}
