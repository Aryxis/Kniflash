#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMovie>
#include <QTimer>

class MyGraphicsView : public QGraphicsView {
private:
    QGraphicsPixmapItem *m_item;            // 可移动的图形项
    QGraphicsPixmapItem *m_background;      // 背景图像项
    QMovie *m_movie;                        // 用于播放 GIF
    QGraphicsEllipseItem *m_moving_circle;  // 运动圆形

    QTimer m_pathTimer;
    const int SQUARE_SIZE = 500;  // 运动轨迹正方形边长
    int currentEdge = 0;          // 当前所在轨迹边（0-3）
    QPointF initPoint;

public slots:
    // 处理信号, 展示游戏窗口
    // 这里对应步骤(2)
    void handleEvokeGameSignal();

    // 更新运动圆形的路径
    void updateCirclePath();

public:
    explicit MyGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);

protected:
    // 键盘事件, 用来移动背景
    // 注意 override
    void keyPressEvent(QKeyEvent *event) override;
};

#endif  // MYGRAPHICSVIEW_H
