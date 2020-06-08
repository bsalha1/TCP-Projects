#pragma once

#include <iostream>
#include <string>

#include "Request.h"

using namespace std;

class RequestPing : public Request
{
    public:
        RequestPing() : Request("Ping")
        {
            
        }
};