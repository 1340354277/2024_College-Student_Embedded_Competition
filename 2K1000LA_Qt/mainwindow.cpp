#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // widgetPlot2D为通过窗口提升得到的控件
    ui->widgetPlot2D->initGraphName(QStringList() << "温度" << "压强" << "气体浓度");

    /* 使用WidgetPlot2D绘制实时波形只需两步：
     * 1.初始化波形名称：函数initGraphName(QStringList)
     * 2.给对应的波形添加数据：函数addData(QString, double)
     * 波形名称和数据要一一对应。
     * 函数addData为公有槽函数，也可使用信号槽传递波形数据。*/

    /* 注册Onenet */
    my_Onenet = new Onenet();

    /* 注册Lora */
    Lora_Serial = new Lora();
    connect(Lora_Serial,SIGNAL(decode_complete_sig1(int, double, double, double)),this,SLOT(Show_Data(int, double, double, double)));
    connect(Lora_Serial,SIGNAL(decode_complete_sig(int, double, double, double, double, double, double)),this,SLOT(Show_Data_XYZ(int, double, double, double, double, double, double)));
    connect(Lora_Serial,SIGNAL(send_datatocloud_sig(double, double, double, double)),this,SLOT(SendData2Cloud(double, double, double, double)));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::Show_Data(int portID, double ddata7, double ddata8, double ddata9)
{
    if (portID == COM0)
    {
        ui->widgetPlot2D->addData("温度", ddata7);
        ui->widgetPlot2D->addData("压强", ddata8);
        ui->widgetPlot2D->addData("气体浓度", ddata9);
    }
}

void MainWindow::Show_Data_XYZ(int portID, double ddata1, double ddata2, double ddata3, double ddata4, double ddata5, double ddata6)
{
    if (portID == COM0)
    {
        ui->widgetPlot2D->label_set_text(ddata1, ddata2, ddata3, ddata4, ddata5, ddata6);
    }
}

void MainWindow::SendData2Cloud(double temp, double press, double conc, double offset)
{
    my_Onenet->OneNet_UploadData(temp, press, conc, offset);
}
