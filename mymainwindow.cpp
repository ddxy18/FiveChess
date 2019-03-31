#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include<QPainter>
#include<QPalette>
#include<QMouseEvent>
#include<QPushButton>
#include<QDialog>
#include<QDialogButtonBox>
#include<QTextBrowser>
#include<QLabel>

enum chesskind {EMPTY,BLACK,WHITE}; //玩家黑棋，电脑白棋
int chessboard[17][17]={0}; //棋盘
int chessscore[17][17]={0}; //评分数组

MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);
    for(int i=0;i<17;++i)
        for(int j=0;j<17;++j) chessboard[i][j]=0;
    setFixedSize(720,720);
    setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    connect(ui->help,&QAction::triggered,this,&MyMainWindow::help);
    connect(ui->TotalInformation,&QAction::triggered,this,&MyMainWindow::totalInformation);
    connect(ui->NewGame,&QAction::triggered,
            [=] () mutable
            {
             ChangePlay();
             update();
            }
           );
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}

void MyMainWindow::paintEvent(QPaintEvent *)
{
    if(isplay==0) return;
    if(isplay==1)
    {
        QPalette palette;
        palette.setColor(QPalette::Background,QColor("#B1723C"));
        setPalette(palette); //设置背景色为黄色
        QPainter paint(this);
        QBrush brush;
        brush.setColor(QColor("#EEC085"));
        brush.setStyle(Qt::SolidPattern);
        paint.setBrush(brush);
        paint.drawRect(40,40,640,640);
        for(int i=0;i<16;++i)
        {
            paint.drawLine(Limit1+40*i,Limit1,Limit1+40*i,Limit2); //竖线
            paint.drawLine(Limit1,Limit1+40*i,Limit2,Limit1+40*i); //横线
        }
        if(player==1&&movemouse==1)
        {
            brush.setColor(QColor(Qt::red));
            paint.setBrush(brush);
            paint.setRenderHint(QPainter::Antialiasing);
            paint.drawEllipse(crossx*40+35,crossy*40+35,10,10);
            movemouse=0;
        }
        for(int i=0;i<17;++i)
            for(int j=0;j<17;++j)
            {
                if(chessboard[i][j]==1)
                {
                    brush.setStyle(Qt::SolidPattern);
                    brush.setColor(QColor(Qt::black));
                    paint.setBrush(brush);
                    paint.drawEllipse(40*i+27.5,40*j+27.5,25,25);
                }

            }
        if(player==0) AIPlay();
        for(int i=0;i<17;++i)
            for(int j=0;j<17;++j)
            {
                if(chessboard[i][j]==2)
                {
                    brush.setStyle(Qt::SolidPattern);
                    brush.setColor(QColor(Qt::white));
                    paint.setBrush(brush);
                    paint.drawEllipse(40*i+27.5,40*j+27.5,25,25);
                }

            }
     }
}

void MyMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x=event->x(),y=event->y();
    if(!isOverLimit(x,y))
    {
        int tmpx=x,tmpy=y;
        int xi=(x-Limit1)/40;
        int yi=(y-Limit1)/40;
        if((x-40-40*xi)*(x-40-40*xi)+(y-40-40*yi)*(y-40-40*yi)<225&&chessboard[xi][yi]==0) {crossx=xi;crossy=yi;}
        if((x-40*xi-80)*(x-40*xi-80)+(y-40*yi-40)*(y-40*yi-40)<225&&chessboard[xi+1][yi]==0) {crossx=xi+1;crossy=yi;}
        if((x-40*xi-80)*(x-40*xi-80)+(y-40*yi-80)*(y-40*yi-80)<225&&chessboard[xi+1][yi+1]==0) {crossx=xi+1;crossy=yi+1;}
        if((x-40*xi)*(x-40*xi)+(y-40*yi-80)*(y-40*yi-80)<225&&chessboard[xi][yi+1]==0) {crossx=xi;crossy=yi+1;}
    }
    movemouse=1;
    update();
}

bool MyMainWindow::isOverLimit(int x,int y)
{
    if(x>=Limit1&&x<=Limit2&&y>=Limit1&&y<=Limit2) return false;
    else return true;
}

void MyMainWindow::help()
{
    QDialog helpwindow;
    helpwindow.setFixedSize(250,120);
    QTextBrowser *textbrowser=new QTextBrowser(&helpwindow);
    textbrowser->setFixedSize(250,120);
    textbrowser->setText("黑白双方依次落子,由黑先下,也可以不交换,主动权在白方!然后继续下棋,任一方先在棋盘上形成横向、竖向、斜向的连续的相同颜色的五个(含五个以上)棋子的一方为胜");
    QDialogButtonBox *button=new QDialogButtonBox(&helpwindow);
    button->addButton("OK",QDialogButtonBox::YesRole);
    button->move(90,90);
    connect(button,&QDialogButtonBox::clicked,&helpwindow,&QDialog::close);
    helpwindow.exec();
}

void MyMainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    int tmp=sumBlack;
    if(player==1&&chessboard[crossx][crossy]==EMPTY&&(event->button()==Qt::LeftButton||event->button()==Qt::RightButton))
        {
            chessboard[crossx][crossy]=1;
            sumBlack++;
        }
    end();
    if(sumBlack>tmp) player=0;
    update();
}

int MyMainWindow::FiveInLine(int x,int y)
{
    int sum1=-1,sum2=0;
    for(int i=0;i<5;++i)
    {
        if(chessboard[x-i][y]==1) ++sum1;
        else break;
    }
    for(int i=0;i<5;++i)
    {
        if(chessboard[x+i][y]==1) ++sum2;
        else break;
    }
    if(sum1+sum2==5) return 1;
    else {sum1=-1;sum2=0;}

    for(int i=0;i<5;++i)
    {
        if(chessboard[x-i][y-i]==1) ++sum1;
        else break;
    }
    for(int i=0;i<5;++i)
    {
        if(chessboard[x+i][y+i]==1) ++sum2;
        else break;
    }
    if(sum1+sum2==5) return 1;
    else {sum1=-1;sum2=0;}

    for(int i=0;i<5;++i)
    {
        if(chessboard[x][y+i]==1) ++sum1;
        else break;
    }
    for(int i=0;i<5;++i)
    {
        if(chessboard[x][y-i]==1) ++sum2;
        else break;
    }
    if(sum1+sum2==5) return 1;
    else {sum1=-1;sum2=0;}

    for(int i=0;i<5;++i)
    {
        if(chessboard[x-i][y+i]==1) ++sum1;
        else break;
    }
    for(int i=0;i<5;++i)
    {
        if(chessboard[x+i][y-i]==1) ++sum2;
        else break;
    }
    if(sum1+sum2==5) return 1;
    else {sum1=-1;sum2=0;}

    for(int i=0;i<5;++i)
    {
        if(chessboard[x-i][y]==2) ++sum1;
        else break;
    }
    for(int i=0;i<5;++i)
    {
        if(chessboard[x+i][y]==2) ++sum2;
        else break;
    }
    if(sum1+sum2==5) return 2;
    else {sum1=-1;sum2=0;}

    for(int i=0;i<5;++i)
    {
        if(chessboard[x-i][y-i]==2) ++sum1;
        else break;
    }
    for(int i=0;i<5;++i)
    {
        if(chessboard[x+i][y+i]==2) ++sum2;
        else break;
    }
    if(sum1+sum2==5) return 2;
    else {sum1=-1;sum2=0;}

    for(int i=0;i<5;++i)
    {
        if(chessboard[x][y+i]==2) ++sum1;
        else break;
    }
    for(int i=0;i<5;++i)
    {
        if(chessboard[x][y-i]==2) ++sum2;
        else break;
    }
    if(sum1+sum2==5) return 2;
    else {sum1=-1;sum2=0;}

    for(int i=0;i<5;++i)
    {
        if(chessboard[x-i][y+i]==2) ++sum1;
        else break;
    }
    for(int i=0;i<5;++i)
    {
        if(chessboard[x+i][y-i]==2) ++sum2;
        else break;
    }
    if(sum1+sum2==5) return 2;
    else {sum1=-1;sum2=0;}

    return 0;
}

int MyMainWindow::isFiveInLine()
{
    for(int i=0;i<17;++i)
        for(int j=0;j<17;++j)
            {
                int k=FiveInLine(i,j);
                if(k==1||k==2) return k;
            }
    return 0;
}

void MyMainWindow::AIPlay()
{
    int maxx=0,maxy=0,tmp=0;
    calculateScore();
    for(int i=0;i<17;++i)
        for(int j=0;j<17;++j)
            if(chessscore[i][j]>tmp&&chessboard[i][j]==EMPTY)
            {
                tmp=chessscore[i][j];
                maxx=i;
                maxy=j;
            }
    chessboard[maxx][maxy]=WHITE;
    player=1;
    end();
}

void MyMainWindow::calculateScore()
{
    for(int i=0;i<17;++i)
        for(int j=0;j<17;++j)
            chessscore[i][j]=0;
    int playersum=0,AIsum=0,emptysum=0;

    for(int i=0;i<17;i++)
            for(int j=0;j<17;j++)
            {
                if(i>0&&i>0&&chessboard[i][j]==EMPTY)
                {
                    for(int y=-1;y<=1;y++)
                        for(int x=-1;x<=1;x++)
                        {
                            playersum=0;
                            AIsum=0;
                            emptysum=0;
                            if (!(y==0&&x==0))
                            {
                                for(int k=1;k<=4;k++)
                                {
                                    if (i+k*y>0&&i+k*y<17&&j+k*x>0&&j+k*x<17&&chessboard[i+k*y][j+k*x]==BLACK)
                                    {
                                        playersum++;
                                    }
                                    else if(i+k*y>0&&i+k*y<17&&j+k*x>0&&j+k*x<17&&chessboard[i+k*y][j+k*x]==0)
                                    {
                                        emptysum++;
                                        break;
                                    }
                                    else
                                        break;
                                }

                                for(int k=1;k<=4;k++)
                                {
                                    if(i-k*y>0&&i-k*y<17&&j-k*x>0&&j-k*x<17&&chessboard[i-k*y][j-k*x]==BLACK)
                                    {
                                        playersum++;
                                    }
                                    else if(i-k*y>0&&i-k*y<17&&j-k*x>0&&j-k*x<17&&chessboard[i-k*y][j-k*x]==0)
                                    {
                                        emptysum++;
                                        break;
                                    }
                                    else
                                        break;
                                }

                                if(playersum==1)
                                    chessscore[i][j]+=10;
                                else if(playersum==2)
                                {
                                    if(emptysum==1)
                                        chessscore[i][j]+=30;
                                    else if (emptysum==2)
                                        chessscore[i][j]+=40;
                                }
                                else if(playersum==3)
                                {
                                    if(emptysum==1)
                                        chessscore[i][j]+=60;
                                    else if(emptysum==2)
                                        chessscore[i][j]+=110;
                                }
                                else if(playersum==4)
                                    chessscore[i][j]+=10100;

                                emptysum=0;

                                for(int k=1;k<=4;k++)
                                {
                                    if(i+k*y>0&&i+k*y<17&&j+k*x>0&&j+k*x<17&&chessboard[i+k*y][j+k*x]==1)
                                    {
                                        AIsum++;
                                    }
                                    else if(i+k*y>0&&i+k*y<17&&j+k*x>0&&j+k*x<17&&chessboard[i+k*y][j+k*x]==0)
                                    {
                                        emptysum++;
                                        break;
                                    }
                                    else
                                        break;
                                }

                                for(int k=1;k<=4;k++)
                                {
                                    if(i-k*y>0&&i-k*y<17&&j-k*x>0&&j-k*x<17&&chessboard[i-k*y][j-k*x]==-1)
                                    {
                                        AIsum++;
                                    }
                                    else if(i-k*y>0&&i-k*y<17&&j-k*x>0&&j-k*x<17&&chessboard[i-k*y][j-k*x]==0)
                                    {
                                        emptysum++;
                                        break;
                                    }
                                    else
                                        break;
                                }

                                if(AIsum==0)
                                    chessscore[i][j]+=5;
                                else if(AIsum==1)
                                    chessscore[i][j]+=10;
                                else if(AIsum==2)
                                {
                                    if(emptysum==1)
                                        chessscore[i][j]+=25;
                                    else if(emptysum==2)
                                        chessscore[i][j]+=50;
                                }
                                else if(AIsum==3)
                                {
                                    if(emptysum==1)
                                        chessscore[i][j]+=55;
                                    else if(emptysum==2)
                                        chessscore[i][j]+=100;
                                }
                                else if(AIsum>=4)
                                    chessscore[i][j]+=10000;

                            }
                        }

                }
            }
}

void MyMainWindow::again()
{
    isplay=1;
    update();
}

void MyMainWindow::endWin(int i)
{
    if(i==0) return;
    QDialog *dialog=new QDialog(this);
    dialog->setFixedSize(200,100);
    QLabel *label=new QLabel(dialog);
    if(i==1)
    {
        label->setText("你赢了");
        ++totalWin;
    }
    else if(i==2) label->setText("你输了");
    QDialogButtonBox *button1=new QDialogButtonBox(dialog);
    button1->addButton("再来一局",QDialogButtonBox::YesRole);
    button1->move(30,60);
    connect(button1,&QDialogButtonBox::clicked,
            [=]() mutable
    {
        dialog->close();
        MyMainWindow::again();
    }
    );
    QDialogButtonBox *button2=new QDialogButtonBox(dialog);
    button2->addButton("返回",QDialogButtonBox::YesRole);
    button2->move(120,60);
    connect(button2,&QDialogButtonBox::clicked,
            [=]()mutable
    {
        dialog->close();
        isplay=0;
    });
    dialog->exec();
    for(int i=0;i<17;++i)
        for(int j=0;j<17;++j) chessboard[i][j]=0;
    player=1;
    sumBlack=0;
    crossx=0;
    crossy=0;
    ++total;
}

void MyMainWindow::end()
{
    int i=isFiveInLine();
    endWin(i);
}

void MyMainWindow::ChangePlay()
{
    isplay=1;
}

void MyMainWindow::totalInformation()
{
    QDialog *dialog=new QDialog(this);
    dialog->setFixedSize(250,150);
    QTextBrowser *textbrowser1=new QTextBrowser(dialog);
    textbrowser1->setText("你一共玩了");
    textbrowser1->move(10,20);
    textbrowser1->setFixedSize(180,15);
    QTextBrowser *textbrowser2=new QTextBrowser(dialog);
    textbrowser2->setText(QString::number(total));
    textbrowser2->move(160,20);
    textbrowser2->setFixedSize(130,15);
    QTextBrowser *textbrowser3=new QTextBrowser(dialog);
    textbrowser3->setText("你一共赢了:");
    textbrowser3->move(10,40);
    textbrowser3->setFixedSize(180,15);
    QTextBrowser *textbrowser4=new QTextBrowser(dialog);
    textbrowser4->setText(QString::number(totalWin));
    textbrowser4->move(160,40);
    textbrowser4->setFixedSize(130,15);
    QDialogButtonBox *button1=new QDialogButtonBox(dialog);
    button1->addButton("返回",QDialogButtonBox::YesRole);
    button1->move(90,100);
    connect(button1,&QDialogButtonBox::clicked,
            [=]() mutable
    {
        dialog->close();
    }
    );
    dialog->exec();
}
