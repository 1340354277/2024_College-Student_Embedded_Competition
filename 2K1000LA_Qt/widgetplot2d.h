#ifndef WIDGETPLOT2D_H
#define WIDGETPLOT2D_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include "qcustomplot.h"
#include "qmath.h"

namespace Ui {
class WidgetPlot2D;
}

class WidgetPlot2D : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetPlot2D(QWidget *parent = 0);
    ~WidgetPlot2D();

    void initGraphName(QStringList name);

private:
    Ui::WidgetPlot2D *ui;

    QVector<QCheckBox*> isShowCheckBoxVector;              // 是否显示勾选框
    QVector<QLabel*> valueLabelVector;                     // label数值显示
    QVector<QPushButton*>   GraphColorPushButtonVector;    // 曲线颜色选择按钮
    QMap<QString, int> nameToGraphMap;                     // 曲线名称对应曲线序号
    QVector<double> valueVector;                           // 存储曲线的当前值
    QTime time;

    void initQCP();
    void initWidget();
    void setTheme(QColor axis, QColor background);
    bool isDirExist(QString fullPath);
    void savePlotPng();


private slots:
    void changeGraphVisible();
    void changeGraphColor();
    void plotOperation();
    void horzScrollBarChanged(int value);
    void changePlotTheme();


public slots:
    void addData(QString name, double data);
    void label_set_text(double ddata1, double ddata2, double ddata3, double ddata4, double ddata5, double ddata6 );
};

#endif // WIDGETPLOT2D_H
