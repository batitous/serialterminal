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
#include "../include/Settings.h"

#include <QFileInfo>

QSettings *		Settings::settings = new QSettings( 
	QSettings::NativeFormat, QSettings::UserScope,
        "Kliplab","SerialTerminal"
	);

void Settings::setFirstStart(bool pIsFirstStart)
{
        settings->setValue("IsFirstStart",pIsFirstStart);
}

bool Settings::getFirstStart()
{
        return settings->value("IsFirstStart").toBool();
}

void Settings::setLogStatus(bool pHide)
{
        settings->setValue("LogStatus",pHide);
}

bool Settings::getLogStatus()
{
        return settings->value("LogStatus").toBool();
}

void Settings::setOutputAsciiStatus(bool pHide)
{
        settings->setValue("AsciiStatus",pHide);
}

bool Settings::getOutputAsciiStatus()
{
        return settings->value("AsciiStatus").toBool();
}

void Settings::setOutputHexStatus(bool pHide)
{
        settings->setValue("HexStatus",pHide);
}

bool Settings::getOutputHexStatus()
{
        return settings->value("HexStatus").toBool();
}

void Settings::setControlStatus(bool pHide)
{
        settings->setValue("ControlStatus",pHide);
}

bool Settings::getControlStatus()
{
        return settings->value("ControlStatus").toBool();
}

QString Settings::getPortCom()
{
	return settings->value("COM").toString();
}

void Settings::setPortCom(QString portcom)
{
        settings->setValue("COM", QVariant(portcom));
}

void Settings::setTerminalStatus(bool pHide)
{
        settings->setValue("TerminalStatus",pHide);
}

bool Settings::getTerminalStatus()
{
        return settings->value("TerminalStatus").toBool();
}

void Settings::setProjectPath(QString path)
{
        settings->setValue("ProjectPath",path);
}

QString Settings::getProjectPath()
{
        return settings->value("ProjectPath").toString();
}

void Settings::setWindowSize(int x, int y, int width,int height)
{
        settings->setValue("WindowX", x);
        settings->setValue("WindowY", y);
        settings->setValue("WindowHeight", height);
        settings->setValue("WindowWidth", width);
}

void Settings::getWindowSize(int* x, int* y, int* width, int* height)
{
        *x = settings->value("WindowX").toInt();
        *y = settings->value("WindowY").toInt();
        *height = settings->value("WindowHeight").toInt();
        *width = settings->value("WindowWidth").toInt();
}
