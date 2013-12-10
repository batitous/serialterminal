/*
The MIT License (MIT)

Copyright (c) 2013 Baptiste Burles, Sylvain Fay-Chatelard

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <QDebug>
#include <QMessageBox>
#include <QComboBox>
#include <QStringListModel>
#include <QInputDialog>
#include <QFile>
#include <QFileDialog>

#include "../include/Settings.h"
#include "../include/MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    isStart = false;
    isHexWorkspace = true;

    mAbout = new AboutUi(this);

    threadBuffer = (unsigned char*) malloc(BUFFER_SIZE);
    hexBuffer = (char*) malloc(4000);

    mSerialThread = new SerialThread(threadBuffer,BUFFER_SIZE);

    numberOfCharacter = 0;

    temp.reserve(1000);
    asciiOriginalBuffer.reserve(1000);

    QPixmap pix = QPixmap(":/MainUi/Start.png");
    iconStart = new QIcon(pix);
    pix = QPixmap(":/MainUi/Stop.png");
    iconStop = new QIcon(pix);
    pix = QPixmap(":/MainUi/Ascii.png");
    iconAscii = new QIcon(pix);
    pix = QPixmap(":/MainUi/Hex.png");
    iconHex = new QIcon(pix);
    pix = QPixmap(":/MainUi/DSR_R.png");
    iconDSR_R = new QIcon(pix);
    pix = QPixmap(":/MainUi/DTR_R.png");
    iconDTR_R = new QIcon(pix);
    pix = QPixmap(":/MainUi/RTS_R.png");
    iconRTS_R = new QIcon(pix);
    pix = QPixmap(":/MainUi/CTS_R.png");
    iconCTS_R = new QIcon(pix);
    pix = QPixmap(":/MainUi/DCD_R.png");
    iconDCD_R = new QIcon(pix);
    pix = QPixmap(":/MainUi/RNG_R.png");
    iconRNG_R = new QIcon(pix);
    pix = QPixmap(":/MainUi/DSR_V.png");
    iconDSR_V = new QIcon(pix);
    pix = QPixmap(":/MainUi/DTR_V.png");
    iconDTR_V = new QIcon(pix);
    pix = QPixmap(":/MainUi/RTS_V.png");
    iconRTS_V = new QIcon(pix);
    pix = QPixmap(":/MainUi/CTS_V.png");
    iconCTS_V = new QIcon(pix);
    pix = QPixmap(":/MainUi/DCD_V.png");
    iconDCD_V = new QIcon(pix);
    pix = QPixmap(":/MainUi/RNG_V.png");
    iconRNG_V = new QIcon(pix);

    mSerialPort = new QComboBox;
    Utils::setPortComList(mSerialPort);
    mSerialPort->setCurrentIndex(0);

    mBaudrate = new QComboBox;
    QStringList baudrate;
    baudrate << tr("230400") << tr("115200") << tr("57600") << tr("56000") << tr("38400") << tr("28800") << tr("19200") << tr("14400") << tr("9600") << tr("4800") << tr("2400") << tr("1200") << tr("600") << tr("300") << tr("110");
    mBaudrate->addItems(baudrate);
    mBaudrate->setCurrentIndex(1);

    mStartStop = new QAction(tr("StartStop"), this);
    mStartStop->setShortcut(tr("Ctrl+Return"));
    mStartStop->setToolTip(tr("Start communication"));
    mStartStop->setStatusTip(tr("Start communication"));
    mStartStop->setWhatsThis(tr("Start communication"));
    mStartStop->setIcon(*iconStart);

    mHexAscii = new QAction(tr("HexAscii"), this);
    mHexAscii->setShortcut(tr("Ctrl+A"));
    mHexAscii->setToolTip(tr("Change to Ascii workspace"));
    mHexAscii->setStatusTip(tr("Change to Ascii workspace"));
    mHexAscii->setWhatsThis(tr("Change to Ascii workspace"));
    mHexAscii->setIcon(*iconAscii);

    mClearButton = new QAction(tr("Clear"), this);
    mClearButton->setShortcut(tr("Ctrl+E"));
    mClearButton->setToolTip(tr("Clean output ascii and hex"));
    mClearButton->setStatusTip(tr("Clean output ascii and hex"));
    mClearButton->setWhatsThis(tr("Clean output ascii and hex"));
    mClearButton->setIcon( QIcon(QPixmap(":/MainUi/Delete.png")));

    mConfigToolbar = addToolBar(tr("Pointer type"));
    mConfigToolbar->addWidget(mSerialPort);
    mConfigToolbar->addWidget(mBaudrate);
    mConfigToolbar->addAction(mStartStop);
    mConfigToolbar->addAction(mHexAscii);
    mConfigToolbar->addAction(mClearButton);

    ui->setupUi(this);

    connect( mStartStop, SIGNAL(triggered() ), this, SLOT(eventStartStop()) );
    connect( mHexAscii, SIGNAL(triggered() ), this, SLOT(eventHexAscii()) );
    connect( mClearButton, SIGNAL(triggered() ), this, SLOT(eventClearButton()) );
    connect( ui->actionQuit, SIGNAL(triggered() ), this, SLOT(eventQuit()) );
    connect( ui->actionAbout, SIGNAL(triggered() ), this, SLOT(eventAbout()) );
    connect( ui->actionSaveInputOutput, SIGNAL(triggered() ), this, SLOT(eventSaveInputOutput()) );
    connect( ui->actionSaveTerminalContent, SIGNAL(triggered() ), this, SLOT(eventSaveTerminalContent()) );
    connect( ui->actionSave_Input_Output, SIGNAL(triggered() ), this, SLOT(eventSaveInputOutput()) );
    connect( ui->actionSave_Terminal_Content, SIGNAL(triggered() ), this, SLOT(eventSaveTerminalContent()) );
    connect( mSerialPort, SIGNAL(currentIndexChanged(const QString &)),this, SLOT(eventSerialPort()));
    connect( mBaudrate, SIGNAL(currentIndexChanged(const QString &)),this, SLOT(eventBaudrate()));
    connect( ui->addOutputFrame, SIGNAL(clicked()), this, SLOT(eventAddOutputFrame()));
    connect( ui->deleteOutputFrame, SIGNAL(clicked()), this, SLOT(eventDeleteOutputFrame()));
    connect( ui->sendOutputFrame, SIGNAL(clicked()), this, SLOT(eventSendOutputFrame()));
    connect( ui->outputTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(eventDoubleClickOutputTable(QModelIndex)));
    connect( ui->addOutputFrameHex, SIGNAL(clicked()), this, SLOT(eventAddOutputFrameHex()));
    connect( ui->deleteOutputFrameHex, SIGNAL(clicked()), this, SLOT(eventDeleteOutputFrameHex()));
    connect( ui->sendOutputFrameHex, SIGNAL(clicked()), this, SLOT(eventSendOutputFrameHex()));
    connect( ui->outputTableHex, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(eventDoubleClickOutputTableHex(QModelIndex)));
    connect( ui->directAsciiText, SIGNAL(textChanged(QString)), this, SLOT(eventDirectAsciiChanged(QString)));
    connect( ui->directHexText, SIGNAL(textChanged(QString)), this, SLOT(eventDirectHexChanged(QString)));
    connect( ui->actionSetDTR, SIGNAL(triggered(bool)), this, SLOT(eventSetDTR(bool)));
    connect( ui->actionSetRTS, SIGNAL(triggered(bool)), this, SLOT(eventSetRTS(bool)));

    connect( ui->actionClear_log, SIGNAL(triggered()), this, SLOT(eventClearLog()));
    connect( ui->actionClear_Ascii_Hex_log, SIGNAL(triggered()), this, SLOT(eventClearAsciiHexLog()));
    connect( ui->actionClear_Ascii_log, SIGNAL(triggered()), this, SLOT(eventClearAsciiLog()));
    connect( ui->actionControl_Status, SIGNAL(triggered(bool)), this, SLOT(eventHideControlStatus(bool)));
    connect( ui->actionActivate_Control_Line, SIGNAL(triggered(bool)), this, SLOT(eventActivateControlLine(bool)) );

    connect( ui->actionOutput_Ascii, SIGNAL(triggered(bool)), this, SLOT(eventHideOutputAscii(bool)));
    connect( ui->actionOutput_Hex, SIGNAL(triggered(bool)), this, SLOT(eventHideOutputHex(bool)));
    connect( ui->actionLog, SIGNAL(triggered(bool)), this, SLOT(eventHideLog(bool)));
    connect( ui->actionOpenFrameProject, SIGNAL(triggered()), this, SLOT(eventOpenFrameProject()));

    ui->terminalOutputHex->setVerticalScrollBar(ui->verticalScrollBar);
    ui->terminalOutputAscii->setVerticalScrollBar(ui->verticalScrollBar);

    ui->dockOutputAscii->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    ui->dockOutputHex->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    ui->dockLog->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    QFont font = ui->terminalOutputAscii->font();
#ifdef Q_OS_DARWIN
    font.setPointSize(14);
#else
    font.setPointSize(10);
#endif
    ui->terminalOutputAscii->setFont(font);
    ui->terminalOutputHex->setFont(font);
    ui->terminalOutputAsciiOnly->setFont(font);

    ui->terminalOutputAsciiOnly->setVisible(false);

    if (Settings::getFirstStart()==true)
    {
        ui->dockLog->setVisible(Settings::getLogStatus());
        ui->actionLog->setChecked(Settings::getLogStatus());
        ui->dockOutputAscii->setVisible(Settings::getOutputAsciiStatus());
        ui->actionOutput_Ascii->setChecked(Settings::getOutputAsciiStatus());
        ui->dockOutputHex->setVisible(Settings::getOutputHexStatus());
        ui->actionOutput_Hex->setChecked(Settings::getOutputHexStatus());
        ui->ControlToolBar->setVisible(Settings::getControlStatus());
        ui->actionControl_Status->setChecked(Settings::getControlStatus());
        if (Settings::getTerminalStatus()==true)
        {
            eventHexAscii();
        }
    }
    else
    {
        Settings::setFirstStart(true);
        Settings::setControlStatus(true);
        Settings::setLogStatus(true);
        Settings::setOutputAsciiStatus(true);
        Settings::setOutputHexStatus(true);
        Settings::setTerminalStatus(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStartStopButton(QString text, QIcon & icon)
{
    mStartStop->setToolTip(text);
    mStartStop->setStatusTip(text);
    mStartStop->setWhatsThis(text);
    mStartStop->setIcon(icon);
}

bool MainWindow::startCommunication()
{
    if (initUART((unsigned char*)mSerialPort->currentText().toLocal8Bit().constData(),mBaudrate->currentText().toInt())!=UART_OK)
    {
        QMessageBox::critical(this, tr("Open failed"),tr("Failed, an error occured during opening serial port..."));
        return false;
    }

    connect( mSerialThread, SIGNAL(controlState(bool,bool,bool,bool,bool,bool)), this, SLOT(receiveControlState(bool,bool,bool,bool,bool,bool)));
    connect( mSerialThread, SIGNAL(charReceived(int,int)),this, SLOT(eventDataReceived(int,int)));

    mSerialThread->start();

    return true;
}

void MainWindow::stopCommunication()
{
    disconnect( mSerialThread, SIGNAL(controlState(bool,bool,bool,bool,bool,bool)),0,0);
    disconnect( mSerialThread, SIGNAL(charReceived(int,int)),0,0);

    UInt32 counter = 10;
    while (mSerialThread->stop() == false)
    {
        counter--;
        if (counter==0)
        {
            break;
        }
        waitMs(200);
    }

    closeUART();
}

void MainWindow::eventQuit()
{
    if (isStart==true)
    {
        stopCommunication();
    }

    qDebug() << "MainWindow::eventQuit now exit !";

    this->close();
}

void MainWindow::eventStartStop()
{
    if (isStart == false && mSerialPort->currentText()!="")
    {
        qDebug()<<"MainWindow::eventStartStop start";

        if (startCommunication()==false)
        {
            return;
        }

        setStartStopButton(tr("Stop communication"), *iconStop);

        isStart = true;
    }
    else
    {
        qDebug()<<"MainWindow::eventStartStop stop";

        setStartStopButton(tr("Start communication"), *iconStart);

        stopCommunication();

        isStart = false;
    }
}

void MainWindow::eventHexAscii()
{
    if(isHexWorkspace == false)
    {
        mHexAscii->setToolTip(tr("Change to Ascii workspace"));
        mHexAscii->setStatusTip(tr("Change to Ascii workspace"));
        mHexAscii->setWhatsThis(tr("Change to Ascii workspace"));
        mHexAscii->setIcon(*iconAscii);
        ui->terminalOutputAsciiOnly->setVisible(false);
        ui->terminalOutputAscii->setVisible(true);
        ui->terminalOutputHex->setVisible(true);
        Settings::setTerminalStatus(isHexWorkspace);
        isHexWorkspace = true;
    }
    else
    {
        mHexAscii->setToolTip(tr("Change to Hex workspace"));
        mHexAscii->setStatusTip(tr("Change to Hex workspace"));
        mHexAscii->setWhatsThis(tr("Change to Hex workspace"));
        mHexAscii->setIcon(*iconHex);
        ui->terminalOutputAscii->setVisible(false);
        ui->terminalOutputHex->setVisible(false);
        ui->terminalOutputAsciiOnly->setVisible(true);
        Settings::setTerminalStatus(isHexWorkspace);
        isHexWorkspace = false;
    }
}

void MainWindow::eventClearButton()
{
    ui->terminalOutputAscii->clear();
    ui->terminalOutputHex->clear();
    ui->terminalOutputAsciiOnly->clear();
}

void MainWindow::eventAbout()
{
    mAbout->show();
}

void MainWindow::eventSaveInputOutput()
{
    qDebug()<<"eventSaveInputOutput";
    QString final="";
    final += "terminal.numberOfAsciiFrame=";
    final += QString().setNum(ui->outputTable->count());
    final += "\r\n";
    for(int i=0;i<ui->outputTable->count();i++)
    {
        final += "terminal.asciiFrame";
        final += QString().setNum(i);
        final += "=";
        final += ui->outputTable->item(i)->text().toLocal8Bit().constData();
        final += "\r\n";
    }
    final += "terminal.numberOfHexFrame=";
    final += QString().setNum(ui->outputTableHex->count());
    final += "\r\n";
    for(int i=0;i<ui->outputTableHex->count();i++)
    {
        final += "terminal.hexFrame";
        final += QString().setNum(i);
        final += "=";
        final += ui->outputTableHex->item(i)->text().toLocal8Bit().constData();
        final += "\r\n";
    }
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save Frame Project"),Settings::getProjectPath());
    if(fileName!="")
    {
       Settings::setProjectPath(fileName);
       QFile file(fileName);
       file.remove(fileName);
       file.open(QFile::ReadWrite);
       QTextStream ts(&file);
       ts << final;
       file.close();
    }
}

void MainWindow::eventOpenFrameProject()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Frame Project"),Settings::getProjectPath());
    if(fileName=="") return;

    Settings::setProjectPath(fileName);
 /*   Properties *p;
    UInt8 *prop;

    p = LoadProperties(	(UInt8 *)fileName.toAscii().constData());
    if(p==0)
    {
        return;
    }

    prop = GetProperties( p, (UInt8 *)"terminal.numberOfAsciiFrame");
    if(prop==0)
    {
            QMessageBox::critical(this, tr("Open failed"),tr("Failed, file not support..."));
            FreeProperties(p);
            return;
    }

    ui->outputTable->clear();
    ui->outputTableHex->clear();

    int number;
    Int8 frame[22];
    sprintf(frame,"terminal.asciiFrame0");
    int result = sscanf((const char *)prop,"%d", &number);
    if(result==0)
    {
        qDebug()<<"No ascii frame!\n";
    }
    qDebug()<<number;
    for(int i=0;i<number;i++)
    {
        frame[19] = QString().setNum(i).toAscii().constData()[0];
        if(i>=10)
            frame[20] = QString().setNum(i).toAscii().constData()[1];
        else
            frame[20] = '\0';
        if(i>=100)
            frame[21] = QString().setNum(i).toAscii().constData()[2];
        else
            frame[21] = '\0';
        qDebug("%s",frame);
        ui->outputTable->addItem((const char*)GetProperties(p,(UInt8*)frame));
    }

    prop = GetProperties( p, (UInt8 *)"terminal.numberOfHexFrame");

    sprintf(frame,"terminal.hexFrame0");
    result = sscanf((const char *)prop,"%d", &number);
    if(result==0)
    {
            qDebug()<<"No hex frame!\n";
    }
    qDebug()<<number;
    for(int i=0;i<number;i++)
    {
        frame[17] = QString().setNum(i).toAscii().constData()[0];
        if(i>=10)
            frame[18] = QString().setNum(i).toAscii().constData()[1];
        else
            frame[18] = '\0';
        if(i>=100)
            frame[19] = QString().setNum(i).toAscii().constData()[2];
        else
            frame[19] = '\0';
        qDebug("%s",frame);
        ui->outputTableHex->addItem((const char*)GetProperties(p,(UInt8*)frame));
    }*/
}

void MainWindow::eventSaveTerminalContent()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save Ascii Content"),Settings::getProjectPath());

    qDebug()<<"MainWindow::eventSaveTerminalContent " << fileName;

    if(fileName!="")
    {
       Settings::setProjectPath(fileName);
       QFile file(fileName);
       file.remove(fileName);
       file.open(QFile::ReadWrite);
       QTextStream ts(&file);
       ts << ui->terminalOutputAsciiOnly->toPlainText();
       file.close();
    }
}

void MainWindow::eventSerialPort()
{
    restartCommunication();
}

void MainWindow::eventBaudrate()
{
    restartCommunication();
}

void MainWindow::restartCommunication()
{
    if (isStart)
    {
        qDebug()<<"MainWindow::restartCom "<<mSerialPort->currentText().toLocal8Bit().constData()<<mBaudrate->currentText().toInt();

        ui->logText->appendPlainText("Reconnect to "+mSerialPort->currentText()+" "+mBaudrate->currentText());

        while (mSerialThread->stop()==false)
        {
            waitMs(500);
        }

        closeUART();

        if ( initUART((unsigned char*)mSerialPort->currentText().toLocal8Bit().constData(),mBaudrate->currentText().toInt())!=UART_OK)
        {
            QMessageBox::critical(this, tr("Open failed"),tr("Failed, an error occured during opening serial port..."));

            setStartStopButton(tr("Start communication"), *iconStart);

            isStart = false;
            return;
        }

        setStartStopButton(tr("Stop communication"), *iconStop);

        mSerialThread->start();

        isStart = true;
    }
}

void MainWindow::setHexBuffer(int end, int pStartPosition)
{
    int i;
    int j = 0;
    int k = 0;

    for ( i = 0 ; i< end; i++)
    {
        asciiOriginalBuffer[i] = threadBuffer[i+pStartPosition];
        hexBuffer[i+k] = threadBuffer[i+pStartPosition]>>4;
        if (hexBuffer[i+k]<=0x09)
        {
            hexBuffer[i+k]+=0x30;
        }
        else
        {
            hexBuffer[i+k]-=0x0A;
            hexBuffer[i+k]+=0x41;
        }

        hexBuffer[i+(++k)] = threadBuffer[i+pStartPosition]&0x0F;
        if (hexBuffer[i+k]<=0x09)
        {
            hexBuffer[i+k]+=0x30;
        }
        else
        {
            hexBuffer[i+k]-=0x0A;
            hexBuffer[i+k]+=0x41;
        }

        hexBuffer[i+(++k)] = ' ';
        if (threadBuffer[i+pStartPosition]>=0x7F || threadBuffer[i+pStartPosition]<=0x20)
        {
            temp[i+j] = '.';
        }
        else
        {
            temp[i+j] = threadBuffer[i+pStartPosition];
        }

        numberOfCharacter++;
        if (numberOfCharacter==8)
        {
            temp[i+(++j)] = ' ';
            hexBuffer[i+(++k)] = ' ';
        }
        else if (numberOfCharacter==16)
        {
            numberOfCharacter=0;
            temp[i+(++j)] = '\r';
            temp[i+(++j)] = '\n';
            hexBuffer[i+(++k)] = '\r';
            hexBuffer[i+(++k)] = '\n';
        }
    }
    hexBuffer[i+k] = '\0';
}

void MainWindow::eventDataReceived(int pStartPosition,int pStopPosition)
{
    if(pStopPosition>pStartPosition)
    {
        temp.clear();
        asciiOriginalBuffer.clear();

        setHexBuffer( pStopPosition - pStartPosition, pStartPosition);

        ui->terminalOutputAsciiOnly->insertPlainText(asciiOriginalBuffer);
        ui->terminalOutputAscii->insertPlainText(temp);
        ui->terminalOutputHex->insertPlainText(hexBuffer);
        ui->terminalOutputAsciiOnly->centerCursor();
        ui->terminalOutputAscii->centerCursor();
        ui->terminalOutputHex->centerCursor();
    }
    else if(pStopPosition!=pStartPosition)
    {
        temp.clear();
        asciiOriginalBuffer.clear();

        setHexBuffer( BUFFER_SIZE - pStartPosition, pStartPosition);

        ui->terminalOutputAsciiOnly->insertPlainText(asciiOriginalBuffer);
        ui->terminalOutputAscii->insertPlainText(temp);
        ui->terminalOutputHex->insertPlainText(hexBuffer);

        temp.clear();
        asciiOriginalBuffer.clear();

        setHexBuffer( pStopPosition, 0);

        ui->terminalOutputAsciiOnly->insertPlainText(asciiOriginalBuffer);
        ui->terminalOutputAscii->insertPlainText(temp);
        ui->terminalOutputHex->insertPlainText(hexBuffer);
    }

}

void MainWindow::endLine()
{
    ui->terminalOutputAscii->insertPlainText("\r\n");
    ui->terminalOutputHex->insertPlainText("\r\n");
    ui->terminalOutputAsciiOnly->centerCursor();
    ui->terminalOutputAscii->centerCursor();
}

void MainWindow::endPart()
{
    ui->terminalOutputAscii->insertPlainText(" ");
    ui->terminalOutputHex->insertPlainText(" ");
    ui->terminalOutputAsciiOnly->centerCursor();
    ui->terminalOutputAscii->centerCursor();
}

void MainWindow::eventAddOutputFrame()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("OutputFrame"),
                                              tr("Frame:"), QLineEdit::Normal,
                                              "Type a frame here", &ok);
    if (ok && !text.isEmpty())
    {
        ui->outputTable->addItem(text);
    }
}

void MainWindow::eventDeleteOutputFrame()
{
    if (ui->outputTable->currentRow()>-1)
    {
        ui->outputTable->takeItem(ui->outputTable->currentRow());
    }
}

void MainWindow::eventSendOutputFrame()
{
    if (ui->outputTable->currentRow()>-1)
    {
        if (sendBufferToUART((unsigned char*)ui->outputTable->currentItem()->text().toLocal8Bit().constData(),
                         (unsigned int)ui->outputTable->currentItem()->text().toLocal8Bit().size())!=0)
        {
            ui->logText->appendPlainText("Failed to send");
            return;
        }
        ui->logText->appendPlainText("Frame send : "+ui->outputTable->currentItem()->text());

        qDebug()<<ui->outputTable->currentItem()->text().toLocal8Bit() << (unsigned int)ui->outputTable->currentItem()->text().size();

    }
}

void MainWindow::eventDoubleClickOutputTable(QModelIndex id)
{
    qDebug()<<ui->outputTable->itemAt(id.row(),id.column())->text();

    bool ok;
    QString text = QInputDialog::getText(this, tr("OutputFrame"),
                                              tr("Edit frame:"), QLineEdit::Normal,
                                              ui->outputTable->currentItem()->text(), &ok);
    if (ok && !text.isEmpty())
    {
        ui->outputTable->currentItem()->setText(text);;
    }
}

void MainWindow::eventAddOutputFrameHex()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("OutputFrameHex"),
                                              tr("Frame:"), QLineEdit::Normal,
                                              "Type an hexadecimal frame here", &ok);
    if (ok && !text.isEmpty())
    {
        while (checkHexValue(text)==false && ok)
        {
            QMessageBox::warning(this,"Hexadecimal value error","Please check your frame, one or more values are wrong","OK","Cancel");
            text = QInputDialog::getText(this, tr("OutputFrameHex"),
                                                      tr("Frame:"), QLineEdit::Normal,
                                                      "Type an hexadecimal frame here", &ok);
        }
        if (checkHexValue(text)==true)
        {
            ui->outputTableHex->addItem(text);
        }
    }
}

void MainWindow::eventDeleteOutputFrameHex()
{
    if (ui->outputTableHex->currentRow()>-1)
    {
        ui->outputTableHex->takeItem(ui->outputTableHex->currentRow());
    }
}

void MainWindow::eventSendOutputFrameHex()
{
    qDebug()<<"eventSendOutputFrameHex"<<ui->outputTableHex->currentRow();

    if (ui->outputTableHex->currentRow()>-1)
    {
        unsigned char *result = new unsigned char(ui->outputTableHex->currentItem()->text().size()/2);
        unsigned char hex[2];
        int j = 0;
        if (checkHexValue(ui->outputTableHex->currentItem()->text())==false)
        {
            QMessageBox::warning(this,"Hexadecimal value error","Please check your frame, one or more values are wrong","OK","Cancel");
            return;
        }
        for (int i=0;i<ui->outputTableHex->itemAt(ui->outputTableHex->currentRow(),0)->text().size();i+=2)
        {
            hex[0]=ui->outputTableHex->currentItem()->text()[i].toLatin1();
            hex[1]=ui->outputTableHex->currentItem()->text()[i+1].toLatin1();
            hexToBin(hex,&result[j]);
            j++;
        }
        if (sendBufferToUART(result,((unsigned int)ui->outputTableHex->currentItem()->text().size()/2))!=UART_OK)
        {
            ui->logText->appendPlainText("Failed to send hexadecimal frame");
            qDebug()<<"ERROR";
            return;
        }
        ui->logText->appendPlainText("Frame : 0x"+ui->outputTableHex->currentItem()->text());

        qDebug()<<ui->outputTableHex->currentItem()->text().toLocal8Bit()<< (unsigned int)ui->outputTableHex->currentItem()->text().size()/2;
    }
}

void MainWindow::eventDoubleClickOutputTableHex(QModelIndex id)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("OutputFrameHex"),
                                              tr("Edit frame:"), QLineEdit::Normal,
                                              ui->outputTableHex->currentItem()->text(), &ok);
    if (ok && !text.isEmpty())
    {
        while (checkHexValue(text)==false && ok)
        {
            QMessageBox::warning(this,"Hexadecimal value error","Please check your frame, one or more values are wrong","OK","Cancel");
            text = QInputDialog::getText(this, tr("OutputFrameHex"),
                                                      tr("Frame:"), QLineEdit::Normal,
                                                      "Type an hexadecimal frame here", &ok);
        }
        if (checkHexValue(text)==true)
        {
            ui->outputTableHex->itemAt(id.row(),id.column())->setText(text);
            qDebug()<<ui->outputTableHex->currentItem()->text();
        }
    }
}

void MainWindow::eventDirectAsciiChanged(QString pText)
{
    if (pText.size()==1)
    {
        if (sendByteToUART((unsigned char)pText[0].toLatin1())!=UART_OK)
        {
            ui->logText->appendPlainText("Failed to send byte");
            return;
        }
        ui->logText->appendPlainText("Byte : "+pText);
        ui->directAsciiText->clear();
    }
}

void MainWindow::eventDirectHexChanged(QString pText)
{
    if (pText.size()==2)
    {
        unsigned char result[2];
        if (checkHexValue(pText) == true)
        {
            hexToBin((unsigned char*)pText.toLocal8Bit().constData(),result);
            if (sendByteToUART(result[0])!=UART_OK)
            {
                ui->logText->appendPlainText("Failed to send hexadecimal byte");
                return;
            }
            ui->logText->appendPlainText("Byte : 0x"+pText);
            ui->directHexText->clear();
        }
    }
}

bool MainWindow::checkHexValue(QString value)
{
    bool check = false;
    for (int i=0;i<value.size();i+=2)
    {
        if ((value[i].toLatin1()>=0x30 && value[i].toLatin1()<=0x39)
            || (value[i].toLatin1()>=0x41 && value[i].toLatin1()<=0x46)
            || (value[i].toLatin1()>=0x61 && value[i].toLatin1()<=0x66))
        {
            check = true;
        }
        else
        {
            return false;
        }
        if ((value[i+1].toLatin1()>=0x30 && value[i+1].toLatin1()<=0x39)
            || (value[i+1].toLatin1()>=0x41 && value[i+1].toLatin1()<=0x46)
            || (value[i+1].toLatin1()>=0x61 && value[i+1].toLatin1()<=0x66))
        {
            check = true;
        }
        else
        {
            return false;
        }
    }
    return check;
}

void MainWindow::eventSetDTR(bool pState)
{
    QIcon icon;
    if (pState == true)
    {
        icon = *iconDTR_V;
        UARTSetDTR();
    }
    else
    {
        icon= *iconDTR_R;
        UARTClearDTR();
    }
    ui->actionSetDTR->setIcon(icon);
}

void MainWindow::eventSetRTS(bool pState)
{
    QIcon icon;
    if (pState == true)
    {
        icon = *iconRTS_V;
        UARTSetRTS();
    }
    else
    {
        icon = *iconRTS_R;
        UARTClearRTS();
    }
    ui->actionSetRTS->setIcon(icon);
}

void MainWindow::receiveControlState(bool pDSR,bool pDTR,bool pRTS,bool pCTS,bool pDCD,bool pRNG)
{
    if (!pDSR)
    {
        ui->actionIsDSR->setIcon(*iconDSR_V);
    }
    else
    {
        ui->actionIsDSR->setIcon(*iconDSR_R);
    }

    if (pDTR)
    {
        ui->actionIsDSR->setIcon(*iconDTR_V);
    }
    else
    {
        ui->actionIsDSR->setIcon(*iconDTR_R);
    }

    if (pRTS)
    {
        ui->actionIsDSR->setIcon(*iconRTS_V);
    }
    else
    {
        ui->actionIsDSR->setIcon(*iconRTS_R);
    }

    if (!pCTS)
    {
        ui->actionIsDSR->setIcon(*iconCTS_V);
    }
    else
    {
        ui->actionIsDSR->setIcon(*iconCTS_R);
    }

    if (!pDCD)
    {
        ui->actionIsDSR->setIcon(*iconDCD_V);
    }
    else
    {
        ui->actionIsDSR->setIcon(*iconDCD_R);
    }

    if (!pRNG)
    {
        ui->actionIsDSR->setIcon(*iconRNG_V);
    }
    else
    {
        ui->actionIsDSR->setIcon(*iconRNG_R);
    }
}

void MainWindow::eventClearLog()
{
    ui->logText->clear();
}

void MainWindow::eventClearAsciiHexLog()
{
    ui->terminalOutputAscii->clear();
    ui->terminalOutputHex->clear();
}

void MainWindow::eventClearAsciiLog()
{
    ui->terminalOutputAsciiOnly->clear();
}

void MainWindow::eventActivateControlLine(bool activate)
{
    mSerialThread->setActiveControlLine(activate);
}

void MainWindow::eventHideControlStatus(bool pHide)
{
    Settings::setControlStatus(pHide);
    ui->ControlToolBar->setVisible(pHide);
}

void MainWindow::eventHideOutputAscii(bool pHide)
{
    Settings::setOutputAsciiStatus(pHide);
    ui->dockOutputAscii->setVisible(pHide);
}

void MainWindow::eventHideOutputHex(bool pHide)
{
    Settings::setOutputHexStatus(pHide);
    ui->dockOutputHex->setVisible(pHide);
}

void MainWindow::eventHideLog(bool pHide)
{
    Settings::setLogStatus(pHide);
    ui->dockLog->setVisible(pHide);
}
