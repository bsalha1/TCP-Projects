#pragma once

#include <string>
#include <list>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

class Util
{
    public:
        static list<string> split(const string& s, const char& c)
        {
            string buff{""};
            list<string> list;
            
            for(auto n : s)
            {
                if(n != c)
                {
                    buff += n;
                }
                else if(n == c && buff != "")
                {
                    list.push_back(buff);
                    buff = "";
                    }
            }

            if(buff != "")
            {
                list.push_back(buff);
            }

            return list;
        }
};
