#ifndef MYDIALOG_H
#define MYDIALOG_H

Error in " Cpp.includeStatement('QWidget', Cpp.cxxHeaderSuffix(), ['QObject', 'QWidget', 'QMainWindow', 'QQuickItem', 'QSharedData'], 'C:/Users/Le'novo/Desktop/first/tablecat02')": SyntaxError: Expected token `)'#include <QWidget>

class mydialog : public QWidget
{
    Q_OBJECT
public:
    explicit mydialog(QWidget *parent = nullptr);

signals:
};

#endif // MYDIALOG_H
