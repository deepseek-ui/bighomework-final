#include "widget.h"
#include"mainwindow1.h"

#include<QPaintEvent>
#include<QPainter>
#include<QPixmap>
#include<QCursor>
#include<QMetaEnum>

//窗口构造函数
Widget::Widget(QWidget *parent) : QWidget(parent),
    timer(new QTimer(this)),  //初始化计时器
    menu(new QMenu(this))     //初始化菜单栏
{
    //窗口设为无框透明
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //实现拖拽功能
    this->installEventFilter(new DragFilter);
    //更新动画帧
    connect(timer,&QTimer::timeout,
            [this](){
                static int i=0;
                auto paths=this->action_map.value(this->cur_role_act);
                //循环播放帧
                this->cur_role_pix=paths[i++%paths.size()];
                this->update();
            });
    // 初始化右键菜单和加载资源
    initMenu();
    loadRoleActRes();
    //默认显示小八打招呼
    showAction(RoleAct::sayhello);
}

Widget::~Widget() {}

//显示动作动画
void Widget::showAction(RoleAct k)
{
    this->cur_role_act=k;
    timer->start(70);
}

//每个动作的对齐方式不同
QMap<RoleAct, QString> actionAlignment = {
    {RoleAct::sayhello, "bottom"},
    {RoleAct::fly, "bottom"},
    {RoleAct::happy, "bottom"},
    {RoleAct::cold,"bottom"},
    {RoleAct::liedown,"bottom"},
    {RoleAct::oioioi,"bottom"},
    {RoleAct::jump, "top-left"},
};

//绘制动画
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixture;
    //加载当前帧
    pixture.load(this->cur_role_pix.toLocalFile());

    // 获取当前动作的对齐方式
    QString alignment = actionAlignment.value(this->cur_role_act, "top-left");

    if (alignment == "bottom") {
        // 底部对齐
        int pixHeight = pixture.height();
        int yPos = this->height() - pixHeight;
        if (yPos < 0) {
            yPos = 0;// 确保图片不会超出窗口顶部
        }
        // 默认(左上角)对齐 - 在(100,100)位置绘制
        painter.drawPixmap(0, yPos, pixture);
    }
    else {
        painter.drawPixmap(100,100,pixture);
    }

}

// 右键菜单栏
void Widget::contextMenuEvent(QContextMenuEvent *event){
    this->menu->popup(QCursor::pos());
}

// 加载
void Widget::loadRoleActRes()
{
    //将资源添加到动作中
    auto addRes=[this](RoleAct k,QString path,int n){
        QList<QUrl>paths;
        char buf[260];
        for(int i=0;i<n;i++){
            memset(buf,0,sizeof(buf));
            sprintf_s(buf,path.toStdString().c_str(),i);
            paths.append(QUrl::fromLocalFile(buf));
        }
        action_map.insert(k,paths);
    };
    addRes(RoleAct::cold,":/cold/image/cold.png/cold(%d).png",20);
    addRes(RoleAct::fly,":/fly/image/fly.png/fly(%d).png",16);
    addRes(RoleAct::happy,":/happy/image/happy.png/happy(%d).png",8);
    addRes(RoleAct::jump,":/jump/image/jump.png/jump(%d).png",8);
    addRes(RoleAct::liedown,":/liedown/image/liedown.png/liedown(%d).png",57);
    addRes(RoleAct::oioioi,":/oioioi/image/oioioi.png/oioioi(%d).png",6);
    addRes(RoleAct::sayhello,":/sayhello/image/sayhello.png/sayhello(%d).png",12);
}

//初始化右键菜单栏
void Widget::initMenu()
{
    //关闭
    QAction* hideAction = new QAction("Hide");
    connect(hideAction, &QAction::triggered, [this](){
        this->setVisible(0);
    });
// 新增的打开UI界面菜单项
    QAction *openUiAction = menu->addAction("跑酷游戏");
    connect(openUiAction, &QAction::triggered, [this](){

        MainWindow *uiWindow = new MainWindow();


        uiWindow->setWindowTitle("跑酷游戏");
        uiWindow->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除

        // 显示窗口
        uiWindow->show();
        //赶走小八

            this->setVisible(0);

    });
    QAction *coldAction = menu->addAction("cold");
    connect(coldAction, &QAction::triggered, [this](){
        showAction(RoleAct::cold);
    });

    QAction *flyAction = menu->addAction("fly");
    connect(flyAction, &QAction::triggered, [this](){
        showAction(RoleAct::fly);
    });

    QAction *happyAction = menu->addAction("happy");
    connect(happyAction, &QAction::triggered, [this](){
        showAction(RoleAct::happy);
    });

    QAction *jumpAction = menu->addAction("jump");
    connect(jumpAction, &QAction::triggered, [this](){
        showAction(RoleAct::jump);
    });

    QAction *liedownAction = menu->addAction("liedown");
    connect(liedownAction, &QAction::triggered, [this](){
        showAction(RoleAct::liedown);
    });

    QAction *oioioiAction = menu->addAction("oioioi");
    connect(oioioiAction, &QAction::triggered, [this](){
        showAction(RoleAct::oioioi);
    });


    QAction *sayhelloAction = menu->addAction("sayhello");
    connect(sayhelloAction, &QAction::triggered, [this](){
        showAction(RoleAct::sayhello);
    });

    menu->addAction(hideAction);

}
