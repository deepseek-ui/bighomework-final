#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMap>
#include<QList>
#include<QUrl>
#include<QTimer>
#include<QEvent>
#include<QMouseEvent>
#include<QPoint>
#include<QContextMenuEvent>
#include<QMenu>
class QPaintEvent;

// 定义动作枚举的命名空间
namespace Act{
Q_NAMESPACE
enum RoleAct{
    cold,
    fly,
    happy,
    jump,
    liedown,
    oioioi,
    sayhello};
Q_ENUM_NS(RoleAct)

}
using namespace Act;


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    //显示动作
    void showAction(RoleAct k);


protected:
    //绘制事件
    void paintEvent(QPaintEvent*event)override;

    //菜单栏事件
    void contextMenuEvent(QContextMenuEvent *event) override;


private:
    //把资源映射到动作中
    QMap<RoleAct,QList<QUrl>>action_map;
    //定时器
    QTimer*timer;
    //当前动作
    RoleAct cur_role_act;
    //当前图片资源
    QUrl cur_role_pix;
    //右键菜单栏
    QMenu*menu;

    //加载角色动作资源
    void loadRoleActRes();
    //初始化菜单
    void initMenu();
};

// 拖拽事件过滤器类
class DragFilter:public QObject{
    Q_OBJECT
public:
    bool eventFilter(QObject*obj,QEvent*event)
    {
        auto w=dynamic_cast<QWidget*>(obj);
        if(!w)return false;
        if(event->type()==QEvent::MouseButtonPress){
            auto e=dynamic_cast<QMouseEvent*>(event);
            if(e){
                // 记录鼠标按下位置
                pos=e->pos();
            }
        }
        else if(event->type()==QEvent::MouseMove)
        {
            auto e=dynamic_cast<QMouseEvent*>(event);
            if(e){
                if(e->buttons()&Qt::MouseButton::LeftButton)
                {
                    // 移动窗口
                    w->move(e->globalPos()-pos);
                }
            }
        }
        return QObject::eventFilter(obj,event);
    }
private:
     // 记录鼠标位置
    QPoint pos;
};
#endif
