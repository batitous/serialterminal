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
#include <QFileInfo>
#include <QSettings>
#include <QtCore/QDebug>

#ifdef Q_OS_UNIX

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "../include/Utils.h"
#include "../include/Settings.h"

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
     struct dirent *read;
     DIR *rep;
     int list;
     char *dirname = new char(100);

     list=0 ;
     dirname = (char*)"/dev/\0";

     rep = opendir(dirname);
     char * pch;
     QStringList listPorts;
     QString str = "/dev/";
     while((read = readdir(rep)))
     {
         pch=strstr(read->d_name,"cu");
         if(pch!=NULL)
         {
             pch=strstr(read->d_name,"Bluetooth");
             if(pch==NULL)
             {
                str = "/dev/";
                str += read->d_name;
                listPorts<<str;
            }
         }
     }
     closedir(rep);


     /*QStringList listPorts;
     struct stat my_stat;

     //Stor on a string each path of serial port files
     for(int i = 0; i < NUMBER_OF_DEVICES; i++)
     {
         if(stat(devices_list[i], &my_stat) == 0)	//if there are informations about the file (path on the first param), they are stocked on the second param and return 0
         {
             listPorts.append((QString)devices_list[i]);
         }
     }*/

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

#endif
