#include "client_function.h"

string fetchMessage(string message, int t)
{
    int j=0;
    for (j=0; j<message.length(); j++)
    {
        if (message[j]=='[') t--;
        if (t==0) break;
    }
    j++;
    string res="";
    while (message[j]!=']' && j<message.length())
        res=res+message[j++];
    return res;
}

vector<string> split_package(string message)
{
    string mess="";
    vector<string> r;
    for (int i = 0; i < message.length(); i++)
    {
        if (message[i] != '`')
        {
            mess += message[i];
        }
        else
        {
            r.push_back(mess);
            mess="";
        }
    }
    return r;
}

string decode(string pass)
{
    string key="chatserverclient";
    string result="";
    int len = key.length();
    for (int i=0; i<pass.length(); i++)
    {
        char t = (key[i % len] + pass[i]);
        if (t != '[' && t != ']' && t != '`')
            result+=t;
    }
    return result;
}
