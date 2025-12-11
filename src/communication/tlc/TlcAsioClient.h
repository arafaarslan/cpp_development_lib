#ifndef __TLC_ASIO_CLIENT__
#define __TLC_ASIO_CLIENT__

#include "TlcAsioUtils.h"

class TlcAsioClient{

    public:

        TlcAsioClient();
        void runClient();
                       

    private : 
        asio::io_context io;

};

#endif