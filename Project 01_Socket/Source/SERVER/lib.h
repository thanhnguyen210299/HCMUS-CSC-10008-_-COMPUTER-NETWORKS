#ifndef LIB_H
#define LIB_H

#pragma once

#include <stdio.h>
#include <Windows.h>
#include <vector>
#include <QTcpSocket>
#include <QTcpServer>
#include <QWidget>
#include <QApplication>
#include <QBuffer>
#include <QFile>
#include <QtCore>
#include <QByteArray>
#include <iostream>
#include <fstream>

#pragma comment(lib, "ws2_32")

using namespace std;

#define MAX_LEN         3000
#define MAX_USERS		32
#define MAX_LEN_NAME	32
#define MAX_LEN_MESSAGE	256
#define MAX_FILE_NAME	50
#define LENGTH 512

#define SERVER_PORT		666

struct User
{
    string username;
    string password;
    bool online;
    QTcpSocket* socket;
};

struct GroupChat
{
    string group_name;
    int number;
    vector<User> users;
};

struct PrivateChat
{
    string client1;
    string client2;
};

#endif // LIB_H
