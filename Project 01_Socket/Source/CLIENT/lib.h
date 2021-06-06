#ifndef LIB_H
#define LIB_H

#define SERVER_IP		"127.0.0.1"
#define SERVER_PORT		666

#define THREAD_WRITE	0
#define THREAD_LISTEN	1

#define MAX_LEN_NAME	3000
#define MAX_LEN_MESSAGE	256
#define MAX_FILE_NAME	50
#define LENGTH 512

#include <QTcpServer>
#include <QTcpSocket>
#include <QApplication>
#include <QWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <string>
using namespace  std;

struct File
{
    qint64 bytesWritten;
    qint64 bytesToWrite;
    qint64 totalBytes;
    qint64 fileNameSize;
    QString fileName;
};


#endif // LIB_H
