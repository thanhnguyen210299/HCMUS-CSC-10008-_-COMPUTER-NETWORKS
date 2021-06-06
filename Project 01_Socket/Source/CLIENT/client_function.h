#ifndef CLIENT_FUNCTION_H
#define CLIENT_FUNCTION_H

#include <string>
#include <vector>
using namespace std;

string fetchMessage(string message, int t);

vector<string> split_package(string message);

string decode(string pass);

#endif // CLIENT_FUNCTION_H
