package com.dd.androidprotobuf;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class BaseSocketConnection {
    protected  MessageRoute m_MessageRoute;

    void ReadData(InputStream buf)
    {

    }
    void SendData(OutputStream buf)
    {

    }

    OutputStream GetOutputStream() throws IOException {
        return  null;
    }

    Socket GetSocket()
    {
        return null ;
    }

}
