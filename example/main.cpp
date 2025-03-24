#include <QApplication>

#include "mainwindow.h"
#include "mygraphicsview.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 初始化游戏场景
    QGraphicsScene scene;
    MyGraphicsView view(&scene);
    view.resize(1080, 675);
    // 关闭滚动条
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 创建开始界面
    MainWindow mw;

    // 连接信号到成员函数
    // 对应步骤(3)
    // 在 main 函数中, 调用 connect 需要加上 QObject::
    // 但是在某些类的成员函数中调用 connect 却并不需要, 为什么?
    QObject::connect(&mw, &MainWindow::evokeGameSignal, &view,
                     &MyGraphicsView::handleEvokeGameSignal);

    // 展示开始界面
    mw.show();

    // 运行应用, 并以应用的返回值作为整个程序的返回值
    return app.exec();
}
