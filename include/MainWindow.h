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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QComboBox>

#include <babcode.h>

#include "Utils.h"
#include "AboutUi.h"
#include "SerialThread.h"

#define BUFFER_SIZE 32768

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow              *ui;
    QToolBar                    *mConfigToolbar;
    QComboBox                   *mSerialPort;
    QComboBox                   *mBaudrate;
    QAction                     *mStartStop;
    QAction                     *mHexAscii;
    QAction                     *mClearButton;

    bool                        isStart;
    bool                        isHexWorkspace;
    int                         numberOfCharacter;
    unsigned char               *threadBuffer;
    QString                     temp;

    AboutUi                     *mAbout;
    SerialThread*               mSerialThread;
    QString                     asciiOriginalBuffer;
    char                        *hexBuffer;

    QPixmap                     *pix;
    QIcon                       *iconStart;
    QIcon                       *iconStop;
    QIcon                       *iconAscii;
    QIcon                       *iconHex;
    QIcon                       *iconDSR_R;
    QIcon                       *iconDTR_R;
    QIcon                       *iconRTS_R;
    QIcon                       *iconCTS_R;
    QIcon                       *iconDCD_R;
    QIcon                       *iconRNG_R;
    QIcon                       *iconDSR_V;
    QIcon                       *iconDTR_V;
    QIcon                       *iconRTS_V;
    QIcon                       *iconCTS_V;
    QIcon                       *iconDCD_V;
    QIcon                       *iconRNG_V;

    void setStartStopButton(QString text, QIcon & icon );

    bool startCommunication();
    void stopCommunication();
    void restartCommunication();

    void setHexBuffer(int end, int pStartPosition);

    void endPart();
    void endLine();
    bool checkHexValue(QString value);

private slots:
    void eventQuit();
    void eventAbout();
    void eventHexAscii();
    void eventClearButton();
    void eventBaudrate();
    void eventStartStop();
    void eventSerialPort();
    void eventAddOutputFrame();
    void eventSaveInputOutput();
    void eventSendOutputFrame();
    void eventOpenFrameProject();
    void eventAddOutputFrameHex();
    void eventDeleteOutputFrame();
    void eventSendOutputFrameHex();
    void eventSaveTerminalContent();
    void eventDataReceived(int,int);
    void eventDeleteOutputFrameHex();
    void eventDirectHexChanged(QString pText);
    void eventDirectAsciiChanged(QString pText);
    void eventDoubleClickOutputTable(QModelIndex);
    void eventDoubleClickOutputTableHex(QModelIndex);

    void eventClearLog();
    void eventClearAsciiHexLog();
    void eventClearAsciiLog();
    void eventHideControlStatus(bool);
    void eventActivateControlLine(bool);
    void eventHideOutputAscii(bool);
    void eventHideOutputHex(bool);
    void eventHideLog(bool);

    void eventSetDTR(bool);
    void eventSetRTS(bool);
    void receiveControlState(bool,bool,bool,bool,bool,bool);
};

#endif // MAINWINDOW_H
