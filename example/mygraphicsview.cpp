#include "mygraphicsview.h"

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMovie>
#include <QPainter>
#include <QTimer>

// 槽函数: 处理信号, 展示游戏窗口
void MyGraphicsView::handleEvokeGameSignal() { this->show(); }

// 槽函数: 更新运动圆形的路径
void MyGraphicsView::updateCirclePath() {
    // 沿正方形轨迹移动
    // 这里用的是什么坐标系?
    const int speed = 3;
    QPointF localPos = m_moving_circle->pos() - initPoint;

    switch (currentEdge) {
        case 0:  // 上边向右移动
            localPos.rx() += speed;
            if (localPos.x() > SQUARE_SIZE / 2) {
                localPos.rx() = SQUARE_SIZE / 2;
                currentEdge = 1;
            }
            break;
        case 1:  // 右边向下移动
            localPos.ry() += speed;
            if (localPos.y() > SQUARE_SIZE / 2) {
                localPos.ry() = SQUARE_SIZE / 2;
                currentEdge = 2;
            }
            break;
        case 2:  // 下边向左移动
            localPos.rx() -= speed;
            if (localPos.x() < -SQUARE_SIZE / 2) {
                localPos.rx() = -SQUARE_SIZE / 2;
                currentEdge = 3;
            }
            break;
        case 3:  // 左边向上移动
            localPos.ry() -= speed;
            if (localPos.y() < -SQUARE_SIZE / 2) {
                localPos.ry() = -SQUARE_SIZE / 2;
                currentEdge = 0;
            }
            break;
    }
    m_moving_circle->setPos(localPos + initPoint);
}

MyGraphicsView::MyGraphicsView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent) {
    setFocusPolicy(Qt::StrongFocus);
    setWindowTitle("Moving demo");

    // 加载并处理圆形背景
    // ":/" 表示从 qrc 加载资源
    QPixmap backgroundPixmap(":/figs/background.jpg");
    if (backgroundPixmap.isNull()) {
        // 时常使用 qDebug 输出调试信息, 是好的开发习惯
        qDebug() << "Failed to load background image!";
    }

    const int bgSize = 1800;  // 圆形背景的直径
    QPixmap scaledBg =
        backgroundPixmap.scaled(bgSize, bgSize, Qt::KeepAspectRatioByExpanding,
                                Qt::SmoothTransformation);

    QPixmap circularBg(bgSize, bgSize);
    circularBg.fill(Qt::transparent);
    QPainter painter(&circularBg);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform);
    painter.setClipRegion(QRegion(0, 0, bgSize, bgSize, QRegion::Ellipse));
    painter.drawPixmap((bgSize - scaledBg.width()) / 2,
                       (bgSize - scaledBg.height()) / 2, scaledBg);

    m_background = scene->addPixmap(circularBg);
    m_background->setPos(0, 0);
    scene->setSceneRect(0, 0, bgSize, bgSize);

    // 创建运动圆形
    m_moving_circle = new QGraphicsEllipseItem(-15, -15, 30, 30, m_background);
    m_moving_circle->setBrush(Qt::red);
    // 将圆形设置在背景的中心
    QPointF bgCenter = m_background->boundingRect().center();
    m_moving_circle->setPos(bgCenter);  // 初始位置
    initPoint = bgCenter;

    // 加载人物 GIF
    m_movie = new QMovie(":/figs/capoo.gif");
    if (!m_movie->isValid()) {
        qDebug() << "Failed to load GIF!";
    }
    m_movie->setScaledSize(QSize(160, 135));

    m_item = scene->addPixmap(m_movie->currentPixmap());
    m_item->setPos(scene->width() / 2,
                   scene->height() / 2);  // 初始位置在场景中心

    connect(m_movie, &QMovie::frameChanged, this,
            [this]() { m_item->setPixmap(m_movie->currentPixmap()); });
    m_movie->start();

    // 路径运动定时器（每 30ms 更新位置）
    connect(&m_pathTimer, &QTimer::timeout, this,
            &MyGraphicsView::updateCirclePath);
    m_pathTimer.start(30);

    // 居中显示人物
    centerOn(m_item);
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event) {
    const int step = 10;
    switch (event->key()) {
        case Qt::Key_W:
            m_background->moveBy(0, step);
            break;
        case Qt::Key_S:
            m_background->moveBy(0, -step);
            break;
        case Qt::Key_A:
            m_background->moveBy(step, 0);
            break;
        case Qt::Key_D:
            m_background->moveBy(-step, 0);
            break;
        default:
            QGraphicsView::keyPressEvent(event);
    }
    centerOn(m_item);  // 强制视图始终居中显示角色
    scene()->update();
}
