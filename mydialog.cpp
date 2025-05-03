#include "mydialog.h"

mydialog::mydialog(QWidget *parent)
    : QWidget{parent}
{

    void Widget::openUiFile() {
        // 创建并显示新的对话框（假设MyDialog是你创建的对话框类）
        MyDialog *dialog = new MyDialog(this);
        dialog->exec(); // 这个会阻塞执行，直到对话框关闭
    }

}
