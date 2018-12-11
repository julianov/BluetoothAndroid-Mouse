#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   /* int x, y;
    x=100;
    y=40;
   /* screen=QGuiApplication::primaryScreen();
    cursor= new QCursor();
    cursor->setPos(screen , x,y);
*/ /*
    SetCursorPos(x, y);
    INPUT    *Input=new INPUT;
      // left down
      Input->type      = INPUT_MOUSE;
      Input->mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
    SendInput(1,Input,sizeof(INPUT));
    Input->mi.dwFlags  = MOUSEEVENTF_LEFTUP;
    SendInput(1,Input,sizeof(INPUT));

*/

    QRect rec = QApplication::desktop()->screenGeometry();
    height = rec.height();
    width = rec.width();



    //Bluetooth server initialization

    const QBluetoothAddress &localAdapter = QBluetoothAddress();

    QBluetoothLocalDevice localDevice;
    QString localDeviceName;

    // Check if Bluetooth is available on this device
    if (localDevice.isValid()) {

        // Turn Bluetooth on
        localDevice.powerOn();

        // Read local device name
        localDeviceName = localDevice.name();

        // Make it visible to others
        localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

        // Get connected devices
        QList<QBluetoothAddress> remotes;
        remotes = localDevice.connectedDevices();
    }

    rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(rfcommServer, SIGNAL(newConnection()), this, SLOT(clientConnected()));
    bool result = rfcommServer->listen(localAdapter);
    if (!result) {
        qWarning() << "Cannot bind chat server to" << localAdapter.toString();
        return;
    }
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("Bt Chat Server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             tr("Example bluetooth chat server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("qt-project.org"));
    static const QLatin1String serviceUuid("00001101-0000-1000-8000-00805F9B34FB");
    serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                             publicBrowse);
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;

    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);
    serviceInfo.registerService(localAdapter);


        sendMessage(QString::number(100));

     tengo_x, tengo_y, tengo_z=false;

     x, y, z=0;
     primera_vez=true;
     segunda_vez=true;
     pos_x=pos_y=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clientConnected()
{
    QBluetoothSocket *socket = rfcommServer->nextPendingConnection();
    if (!socket){
        qDebug()<<"no pasó nada"<<endl;
        return;
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    clientSockets.append(socket);
    emit clientConnected(socket->peerName());
    qDebug()<<"hay algo conectado"<<endl;
}

void MainWindow::readSocket()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

   QByteArray array=socket->readAll();
    //qDebug()<<"Read All: "+array.toHex()<<endl;
    sendMessage(QString::number('g'));//g==103

    BYTE * pByte = reinterpret_cast<byte*>(array.data());

    QByteArray second;
    for (int i=0; i< array.length();i++){
        second.append(pByte[i]);
        second.append('-');
    }
    for (int i=0;i<array.length();i++){
        if(pByte[i]=='x'&&i<array.length()-2){
            tengo_x=true;
            x=(pByte[i+1]*256)+pByte[i+2];
        }
        if(pByte[i]=='y'&&i<array.length()-2){
            tengo_y=true;
            y=(pByte[i+1]*256)+pByte[i+2];
        }
        if(pByte[i]=='z'&&i<array.length()-2){
            tengo_z=true;
            z=(pByte[i+1]*256)+pByte[i+2];
        }

    }

    if (tengo_x&&tengo_y&&tengo_z){

       // qDebug()<<(x/100)-300<<endl<<(y/100)-300<<endl<<(z/100)-300<<endl<<endl;
        tengo_x=tengo_y=tengo_z=false;
        if (primera_vez){
           secuencia_x[0]=(x/100)-400;
           secuencia_y[0]=(y/100)-400;
           primera_vez=false;
        }
        else if (segunda_vez){
            secuencia_x[1]=secuencia_x[0];
            secuencia_y[1]=secuencia_y[0];
            secuencia_x[0]=(x/100)-400;
            secuencia_y[0]=(y/100)-400;
            segunda_vez=false;

        }
        else{
            secuencia_x[2]=(x/100)-400;
            secuencia_y[2]=(y/100)-400;
            qDebug()<<secuencia_x[2]<<endl<<secuencia_y[2]<<endl<<endl;
            if (secuencia_x[2]-0.85*secuencia_x[1]-0.5*secuencia_x[0]<-0.5&&secuencia_x[2]-0.85*secuencia_x[1]-0.5*secuencia_x[0]>-1){
                if (pos_x-2>=0)
                    pos_x-=2;
            }

            if (secuencia_x[2]-0.85*secuencia_x[1]-0.5*secuencia_x[0]<-1){
                if (pos_x-4>=0)
                    pos_x-=4;
            }

            if (secuencia_x[2]-0.85*secuencia_x[1]-0.5*secuencia_x[0]>0.5&&secuencia_x[2]-0.85*secuencia_x[1]-0.5*secuencia_x[0]<1){
                if (pos_x+2<=width)
                    pos_x+=2;
            }

            if (secuencia_x[2]-0.85*secuencia_x[1]-0.5*secuencia_x[0]>1){
                if (pos_x+4<=width)
                    pos_x+=4;
            }

            if (secuencia_y[2]-0.85*secuencia_y[1]-0.5*secuencia_y[0]<-0.5&&secuencia_y[2]-0.85*secuencia_y[1]-0.5*secuencia_y[0]>-1){
                if (pos_y-2>=0)
                    pos_y-=2;
            }

            if (secuencia_y[2]-0.85*secuencia_y[1]-0.5*secuencia_y[0]<-1){
                if (pos_y-4>=0)
                    pos_y-=4;
            }

                    //incremento -1 pos_y
            if (secuencia_y[2]-0.85*secuencia_y[1]-0.5*secuencia_y[0]>0.5&&secuencia_y[2]-0.85*secuencia_y[1]-0.5*secuencia_y[0]<1){
                if (pos_y+2<=height)
                    pos_y+=2;
            }
            if (secuencia_y[2]-0.85*secuencia_y[1]-0.5*secuencia_y[0]>1){
                if (pos_y+4<=height)
                    pos_y+=4;
            }

            secuencia_x[0]=secuencia_x[1];
            secuencia_y[0]=secuencia_y[1];
            secuencia_x[1]=secuencia_x[2];
            secuencia_y[1]=secuencia_y[2];
            SetCursorPos(pos_x, pos_y );
        }

       // posicion(&x,&y,(x/100)-300,(y/100)-300);
    }

}

void MainWindow::clientDisconnected()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    emit clientDisconnected(socket->peerName());

    clientSockets.removeOne(socket);

    socket->deleteLater();
}

void MainWindow::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8();

    foreach (QBluetoothSocket *socket, clientSockets)
        socket->write(text);
}

void MainWindow::stopServer()
{
    // Unregister service
    serviceInfo.unregisterService();

    // Close sockets
    qDeleteAll(clientSockets);

    // Close server
    delete rfcommServer;
    rfcommServer = 0;
}

void MainWindow::posicion(float *x, float *y, float valor_x, float valor_y){

    static double cantidad=0;
    static double cantidad_2=0;
    static double sum_x=0;
    static double sum_y=0;

    qDebug()<<valor_x<<endl<<valor_y<<endl;

    sum_x+=valor_x;
    sum_y+=valor_y;
    cantidad_2++;
    if (cantidad_2>20){
    sum_x=sum_x/20;
    sum_y=sum_y/20;
    cantidad++;
    *x= (sum_x*cantidad)+(valor_x*qPow(cantidad,2));
    *y= (sum_y*cantidad)+(valor_y*qPow(cantidad,2));
    cantidad_2=0;
    }

}
