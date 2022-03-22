#include "cdatalogger.h"


#include <fstream>
#include <chrono>
#include <iostream>
#include <QDir>
#include <QDataStream>

CDataLogger::CDataLogger(QObject *parent):
    QThread(parent)
{

}

CDataLogger::~CDataLogger()
{

}

void CDataLogger::run()
{
//    QString sModulePath = QCoreApplication::applicationDirPath();
    QString logName = createLogName();

//    qDebug() << "logName = " << logName;


    auto myfile = std::fstream(logName.toStdString(), std::ios::out | std::ios::binary);


    mNumDataSaved = 0;

    double startTimeBase = 0.0;

    int elapsedTimeInt = 0;
    int elapsedTimeIntPre = 0;

    queueDataPacket.clear();

    while(bRun)
    {
//        qDebug() << "Logging RUN!!!";


        //save created data packet
        int szPacket1 = queueDataPacket.size();

//        qDebug() << "szPacket1 = " << szPacket1;

        if (!queueDataPacket.empty())
        {
            //printf("1. start Log szPacket = %d\n", queueDataPacket.size());

            mMutex.lock();
            LOG_DATA curPacket = queueDataPacket.front();
            //printf("2. front Log szPacket = %d\n", queueDataPacket.size());
            queueDataPacket.pop_front();
            //printf("3. pop_front Log szPacket = %d\n", queueDataPacket.size());
            mMutex.unlock();

            //printf("4. end pop_front Log szPacket = %d\n", queueDataPacket.size());

//            qDebug() << "run curPacket.EM_Flat[5] = " << curPacket.EM_Flat[5];
//            qDebug() << "run curPacket.EM_Flat[6] = " << curPacket.EM_Flat[6];

            if (mNumDataSaved == 0)
                startTimeBase = (double)curPacket.timeStamp / 1e6;


            //////////////////////////////////////////////////

           int elapsedTimeIntOutput = curPacket.timeStamp - elapsedTimeIntPre;
           static int prevSz = 0;

           bool bFlag = false;

           /*
            if (elapsedTimeIntOutput != 2000)
            {
//				_SerialPacket.message_out(elapsedTimeIntOutput);

                printf("elapsedTimeIntOutput in the Logging Class = %d, sz = %d, prev = %d\n", elapsedTimeIntOutput, queueDataPacket.size(), prevSz);
                bFlag = true;

            }
            prevSz = queueDataPacket.size();



            elapsedTimeIntPre = curPacket.timeStamp;
*/

            //////////////////////////////////////////////////

            mEncodePos = 0;
            write(curPacket);
            myfile.write((const char*)buff, LOG_DATA_SIZE); //array

            mNumDataSaved++;
//            qDebug() << "mNumDataSaved = " << mNumDataSaved;

            double endTime = (double)curPacket.timeStamp / 1e6;

            mLoggingTime = endTime - startTimeBase; //in seconds

            int szPacket2 = queueDataPacket.size();
            if (((szPacket1 - 1) != szPacket2))
            {
//                printf("szPacket1: %d, szPacket2: %d\n", szPacket1, szPacket2);
//                qDebug() << "szPacket1 = " << szPacket1;
//                qDebug() << "szPacket2 = " << szPacket2;

            }


        }

//        msleep(10);    // Delay 100[ms]
    }

    myfile.close();

    if (mNumDataSaved == 0)
    {


        QFile::remove(logName);

//        DeleteFile(logName);

    }

}

QString CDataLogger::createLogName()
{

//    QString sModulePath = QCoreApplication::applicationDirPath();
    QString sModulePath;
    sModulePath.clear();

    QString logName = logFileName;

    time_t t = time(nullptr);
    struct tm tm;
    localtime_s(&tm, &t);

    QString strFilename;

    strFilename.sprintf(("Log_%d_%02d_%02d_%02d_%02d_%02d_"),
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);


    qDebug() << "logName = " << logName;

    sModulePath.append("files/logs/");
    sModulePath.append(strFilename);
    sModulePath.append(logName);
    sModulePath.append(".log");

    qDebug() << "sModulePath = " << sModulePath;


    QString strFullName = sModulePath;


    return strFullName;


}





void CDataLogger::init()
{

    bSetThread = false;
    bRun = false;
    mFile = NULL;
    memset(buff, 0, sizeof(buff));


}

void CDataLogger::LogStart()
{
    init();

    start();

    bRun = true;
    mNumDataSaved = 0;

//    loggingData();
//    create();


}
void CDataLogger::LogStop()
{
    bRun = false;
    //mNumDataSaved = 0;
    this->quit();

    terminate();
}
void CDataLogger::terminate()
{

    if (bSetThread)
    {
//        mainThread.detach();
//        this->de

        //mainThread.join();
        bSetThread = false;
    }


}
