#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCursor>
#include <QScreen>
#include <QtBluetooth/QBluetoothServer>
#include <QBluetoothServiceInfo>

#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>

#include <qbluetoothuuid.h>
#include <qbluetoothserver.h>
#include <qbluetoothservicediscoveryagent.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothlocaldevice.h>

#include <windows.h>
#include <WinUser.h>
//debemos incluir la librería user32.dll en el instalador

#include <QDesktopWidget>

#include <QtMath>

#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void messageReceived(const QString &sender, const QString &message);
    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);

public:
    explicit MainWindow(QWidget *parent = 0);
    void startServer(const QBluetoothAddress &localAdapter = QBluetoothAddress());
    void stopServer();
    void posicion(float *x, float *y, float valor_x, float valor_y);
    ~MainWindow();

public slots:
    void sendMessage(const QString &message);

private slots:
    void clientConnected();
    void clientDisconnected();
    void readSocket();

private:
    Ui::MainWindow *ui;
    QCursor *cursor;
    QScreen *screen;
    QBluetoothServer *rfcommServer;
    QBluetoothServiceInfo serviceInfo;
    QList<QBluetoothSocket *> clientSockets;

    bool tengo_x, tengo_y;
    float x, y;

    bool primera_vez, segunda_vez;

    float secuencia_x[3];
    float secuencia_y[3];

    int pos_x, pos_y;

    int height, width;

    INPUT    *Input;

};

#endif // MAINWINDOW_H
