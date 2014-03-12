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
#include <stdio.h>
#include <QFileInfo>
#include <QSettings>
#include <QtCore/QDebug>

#include <POB/Biglibpob.h>

#include "../Includes/Utils.h"
#include "../Includes/Settings.h"

QString Utils::getFullPathFile(QString &file)
{
	QString temp;
	QFileInfo *info = new QFileInfo(file);

	temp = info->canonicalFilePath();
	delete info;

	return temp;
}

QString Utils::getRelativeFile(QString &projectPath,QString &file)
{
	QString temp;
	QString sep = "/";
	QFileInfo *info = new QFileInfo(file);

	QString f = QString(info->path()+sep);
    temp = Utils::getRelativePath(projectPath,f, sep);

	temp = temp + sep +info->fileName();
	delete info;

	return temp ;
}

QString Utils::getRelativeBaseName(QString &projectPath,QString &file)
{
	QString temp;
	QString sep = "/";
	QFileInfo *info = new QFileInfo(file);

	QString f = QString(info->path()+sep);
    temp = Utils::getRelativePath(projectPath,f, sep);

	temp = temp + sep +info->baseName();
	delete info;

	return  temp;
}

QString Utils::getRelativePath(QString &PathReference , QString &PathLink, QString &sep ) 
{
	if( PathReference == PathLink)
	{
		return "" ;
	}
	
	QStringList ArrPathRef ;
	QStringList ArrPathLink ;
	QString Path ;
	int k , maxLenght ;
	int numUp ;
	int numDiv = 1 ;
	
	ArrPathRef = PathReference.split(sep);
	ArrPathLink = PathLink.split(sep);
	
	if( ArrPathRef[0] != ArrPathLink[0] )
	{
		return PathLink ;
	}
	
	if( ArrPathLink.size() > ArrPathRef.size() )
	{
		maxLenght = ArrPathLink.size();;
	}
	else
	{
		maxLenght = ArrPathRef.size();
	}

	for(k=0 ; k < maxLenght ; k++ )
	{
		if( k > ArrPathLink.size() -1 
		   || k > ArrPathRef.size()-1
		   || (ArrPathLink[k] != ArrPathRef[k]) )
		{
			break ;
		}
		   
		numDiv = numDiv + 1 ;
	}
	
	numUp = ArrPathRef.size() - numDiv + 1 ;
	Path = "" ;

	if( numUp == 0 )
	{
		Path = "." ;
	}
	else
	{
		for(k=0 ; k < numUp /*- 1*/ ; k++ )
		{
			if( k == 0 ) Path = ".." ;
			else Path = Path + sep +".." ;
		}
	}
	
	for(k = (numDiv-1) ; k < ArrPathLink.size() - 1 ; k++ )
	{
		Path = Path +sep + ArrPathLink[k] ;
	}
	
	return Path ;
}

#include <QProcess>

bool Utils::runProcess(QString &workingDir,QString &exe, QString *output, int *exitCode)
{
	QProcess process;

	process.setWorkingDirectory(workingDir);

	process.setProcessChannelMode(QProcess::MergedChannels);

	process.start(exe);

	if(process.waitForStarted(10000)==false)
	{
		//todo error
		*output = process.errorString();

		return false;
	}

	if(process.waitForFinished(10000)==false)
	{
		//todo error
		*output = process.errorString();

		return false;
	}

	QByteArray temp = process.readAll();

//	printf("Result=%s\n", temp.data());

	*output = QString(temp);

	*exitCode = process.exitCode();

	return true;
}

int Utils::setPortComList(QComboBox *portCombo)
{
	// Read windows registry:
	QSettings settings("HKEY_LOCAL_MACHINE\\hardware\\devicemap\\serialcomm",QSettings::NativeFormat);

	// Get keys list:
	QStringList list = settings.childKeys();

	// Get user portcom from Settings:
	QString comport = Settings::getPortCom();

	portCombo->clear();

	qDebug()<<"portcom from Settings: "<<comport;

	// Add serial port to the combo box:
	for(int i=0; i < list.size() ; i++)
	{
		list[i].replace(QString("/"),QString("\\"));

		QString str = getPortCom("hardware\\devicemap\\serialcomm",list[i]);

		qDebug()<<"windows serial port: "<<str;

		portCombo->insertItem(i,str);

		if(comport==str)
		{
			portCombo->setCurrentIndex(i);
		}
		else
		{
			portCombo->setCurrentIndex(-1);
		}
	}

	return list.size();
}

#include <windows.h>


QString Utils::getPortCom(QString subkey, QString key)
{
	QString value = "";

	HKEY hKey;
	LPTSTR lpSubKey, pKey;
//	PTSTR  lpSubKey[256], pKey[256];
	LONG lResult;
	UInt8 data[256];
	DWORD size;
	int i,j;
	
	//warning all string are in Unicode (16 bits)!

	lpSubKey = (LPTSTR)malloc( 2*subkey.length()+2 );
	
	for(i=0;i<subkey.length();i++)
	{
		lpSubKey[i]=subkey[i].toAscii();
	}
	lpSubKey[i]=0;

	lResult = RegOpenKeyEx (HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ, &hKey);

	if (lResult != ERROR_SUCCESS) 
    {
		qDebug()<<"UploadSelectUi::getPortCom failed to open registry";
		free(lpSubKey);
		return value;
	}

	pKey = (LPTSTR)malloc( 2*key.length()+2 );
	for( i=0;i<key.length();i++)
	{
		pKey[i]=key[i].toAscii();
	}
	pKey[i]=0;

	size = 256;
	lResult = RegQueryValueEx(hKey, pKey,0,0, data, &size);
	if (lResult != ERROR_SUCCESS) 
    {
		qDebug()<<"UploadSelectUi::getPortCom failed to read registry";
		free(lpSubKey);
		free(pKey);
		return value;
	}

//	printf("Size=%d\n", size);

	for( i=0, j=0;j<size-2;i++,j=j+2)
	{
		value[i]=data[j];
	}

	RegCloseKey (hKey);

	free(lpSubKey);
	free(pKey);

	return value;
}
