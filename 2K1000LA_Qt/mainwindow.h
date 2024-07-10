#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <lora.h>
#include <wifi.h>
#include <onenet.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void AutoZoomButtonSize();
    void ResetButtonGeometry(QWidget *widget, double factorx, double factory);


private:
    Ui::MainWindow *ui;
    Lora *Lora_Serial;
    Wifi *Wifi_Serial;
    Onenet *my_Onenet;

    //SerialPort *PortA;
    SerialPort *PortB;
    QStringList m_portNameList;
    CObsBuffer ObsBufferA;
    CObsBuffer ObsBufferB;

    QFile       *m_fileA;
    QFile       *m_fileB;
    QDataStream *m_DatastreamA;
    QDataStream *m_DatastreamB;
    QTimer *m_showTimer;

private slots:
    void Show_Data(int portID, double ddata7, double ddata8, double ddata9);
    void Show_Data_XYZ(int portID, double ddata1, double ddata2, double ddata3, double ddata4, double ddata5, double ddata6);
    void SendData2Cloud(double temp, double press, double conc, double offset);

};
#endif // MAINWINDOW_H
