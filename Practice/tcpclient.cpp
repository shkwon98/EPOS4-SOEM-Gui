#include "tcpclient.h"

TcpClient::TcpClient()
{

}

void TcpClient::PrintThis()
{
    qDebug() << "my address: " << this <<endl;
}
