// Copyright 2009 POB-Technology. All rights reserved.
// POB-Technology PROPRIETARY/CONFIDENTIAL.
//



#include <QFileInfo>
#include <QSettings>
#include <QtCore/QDebug>

#ifdef Q_OS_UNIX

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../Includes/Utils.h"
#include "../Includes/Settings.h"

#define NUMBER_OF_DEVICES 16

const char *devices_list[NUMBER_OF_DEVICES] = {
  "/dev/ttyS0",
  "/dev/ttyS1",
  "/dev/ttyS2",
  "/dev/ttyS3",
  "/dev/tts/0",
  "/dev/tts/1",
  "/dev/tts/2",
  "/dev/tts/3",
  "/dev/ttyUSB0",
  "/dev/ttyUSB1",
  "/dev/ttyUSB2",
  "/dev/ttyUSB3",
  "/dev/usb/tts/0",
  "/dev/usb/tts/1",
  "/dev/usb/tts/2",
  "/dev/usb/tts/3"
};


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
     //   workingDir.replace(QString("\\"),QString("/"));
        exe.replace(QString("\\"),QString("/"));

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
     QStringList listPorts;
     struct stat my_stat;

     //Stor on a string each path of serial port files
     for(int i = 0; i < NUMBER_OF_DEVICES; i++)
     {
         if(stat(devices_list[i], &my_stat) == 0)	//if there are informations about the file (path on the first param), they are stocked on the second param and return 0
         {
             listPorts.append((QString)devices_list[i]);
         }
     }

     if(listPorts.size() == 0)
     {
         //"No valid serial device found in /dev, sorry !\nYou should have at least one of these :\n/dev/ttyS*\n/dev/tts/*\n/dev/ttyUSB*\n/dev/usb/tts/*\n"
         return false;
     }
     else
     {
         // Add serial port to the combo box

         // Get user portcom from Settings:
         QString comport = Settings::getPortCom();

         portCombo->clear();

         //Insert serial port ont the combo box
         for(int i=0; i < listPorts.size() ; i++)
         {
             portCombo->insertItem(i,listPorts[i]);
             portCombo->setCurrentIndex(i);
             if(comport==listPorts[i])
             {
                 portCombo->setCurrentIndex(i);
             }
             else
             {
                 portCombo->setCurrentIndex(-1);
             }
           }//..for
     }//..else

     return listPorts.size();
}
/*
//#include <windows.h>


QString Utils::getPortCom(QString subkey, QString key)
{
        QString value = NULL, tmp = NULL;

        for(int i=0; subkey[i] != '\0'; i++)
        {
        }

        value = subkey;


        return value;
}
*/

#endif
