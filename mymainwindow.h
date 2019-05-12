#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MyMainWindow;
}

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyMainWindow(QWidget *parent = nullptr);
    ~MyMainWindow();
    void help(); //打开帮助信息
    int FiveInLine(int,int); //判断某一点五子是否连成一线
    int isFiveInLine(); //判断整个棋盘上是否有五子连成一线
    void totalInformation(); //打开统计信息
    void again(); //开始一局新游戏
    void endWin(int); //确认赢家信息并作出响应
    void end(); //在每次落子自动调用IsFiveInLine后确认是否有五子连成一线
    bool isOverLimit(int,int); //确定落子区域是否越界
    void ChangePlay(); //改变isplay的参数
    void AIPlay();
    void calculateScore(); //计算评分

protected:
    void paintEvent(QPaintEvent *); //绘图
    void mouseMoveEvent(QMouseEvent *); //实时反映鼠标移动情况
    void mouseReleaseEvent(QMouseEvent *); //鼠标控制落子

private:
    Ui::MyMainWindow *ui;
    const int Limit1=40;
    const int Limit2=680;
    int crossx=0,crossy=0; //自动记录可以落子处坐标
    int player=1; //记录下一个落子的信息，1表示玩家下个为落子，0表示下个为AI落子
    int movemouse=0; //记录鼠标是否移动，1为移动，0为不移动
    int isplay=0; //记录是否在玩，1为游戏进行，0为游戏结束
    int total=0;
    int totalWin=0; //记录赢的总次数
    int sumBlack=0; //记录玩家落子次数
    QPoint lastBlack,lastWhite; //记录上一次落子，用于悔棋
public slots:
private slots:
    void on_regret_clicked();
};

#endif // MYMAINWINDOW_H
