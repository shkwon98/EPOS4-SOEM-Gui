#ifndef CDATALOGGER_H
#define CDATALOGGER_H

#include <QThread>
#include <QDebug>

#include <mutex>
#include <queue>

#include "Macro.h"
#include <QCoreApplication>

struct LogStatus {

    double loggingTime = 0.0;
    double loggingTimeBase = 0.0;

    uint32_t numDataSaved = 0;
    uint32_t numDataInQueue = 0;

};

class CDataLogger : public QThread
{
//    Q_OBJECT

private:
    std::mutex mMutex;
    QString createLogName();
    FILE* mFile;

    LOG_DATA curHSRPacket;
    std::deque<LOG_DATA> queueDataPacket;

    unsigned int mEncodePos = 0;     //<  Current position in packet for encoding.
    unsigned int mLength;        //<  Length of packet.
    unsigned int mDecodePos = 0;     //<  Current position in packet for decoding.

    unsigned int prevTimeStamp = 0;

    QString logFileName;


    unsigned char buff[LOG_DATA_SIZE*100];

    //template<class T> int convert(T &val);
    //template<class T> int write(T &val);

    /*
    template<class T> int convert(T &val) {
        if (!mReadPacket)
            return 0;

        unsigned int bytes = sizeof(val);
        if (mDecodePos + bytes <= mLength)
        {
            memcpy(&val, &mReadPacket[mDecodePos], bytes);

            mDecodePos += bytes;
            return bytes;
        }
        else
            return 0;
    }
    */

    template<class T> int write(T &val) {
        unsigned int bytes = sizeof(val);


        if (mEncodePos + bytes <= LOG_DATA_SIZE)
        {
            memcpy(&buff[mEncodePos], &val, bytes);

            mEncodePos += bytes;
            return bytes;
        }
        else
            return 0;
    }


public:
    explicit CDataLogger(QObject *parent = nullptr);
    ~CDataLogger();

    bool bRun = false;

    void setQueuePacket(LOG_DATA &data)
    {

        mMutex.lock();

        queueDataPacket.push_back(data);

        //printf("Pushback szPacket = %d\n", queueDataPacket.size());
        mMutex.unlock();
        ////////////////////////////////////////////////

        int elapsedTimeIntOutput = data.timeStamp - prevTimeStamp;

        /*
        if (elapsedTimeIntOutput != 2000)
        {

            printf("elapsedTimeIntOutput in the setQueuePacket Func = %d\n", elapsedTimeIntOutput);

        }
        */

//        qDebug() << "setQueuePacket curPacket.EM_Flat[5] = " << data.EM_Flat[5];
//        qDebug() << "setQueuePacket curPacket.EM_Flat[6] = " << data.EM_Flat[6];

        prevTimeStamp = data.timeStamp;


        ////////////////////////////////////////////////

    }

    double mLoggingTime = 0;
    uint32_t mNumDataSaved = 0;
    uint32_t mNumDataInQueue= 0;

    LogStatus getLoggingStatus()
    {
        LogStatus logStatus;

        logStatus.loggingTime = mLoggingTime;
        logStatus.numDataSaved = mNumDataSaved;
        logStatus.numDataInQueue = mNumDataInQueue;


        return logStatus;

    }


    void setLogFileName(QString logFileName)
    {
        this->logFileName = logFileName;
    }

    bool bSetThread = false;

    void init();
    void LogStart();
    void LogStop();
    void terminate();

    inline bool IsRunning() { return bRun; }

protected:
    void run();

};

#endif // CDATALOGGER_H
