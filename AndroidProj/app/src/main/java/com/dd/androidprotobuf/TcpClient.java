package com.dd.androidprotobuf;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.google.protobuf.GeneratedMessageLite;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

import BaseCmd.Cmd;

/**
 * Created by hadisi on 2016/6/28.
 */

public class TcpClient extends  BaseSocketConnection{


    private static TcpClient mTcpClientConnector;
    private Socket mClient;
    private Thread mConnectThread;


    public static TcpClient getInstance() {
        if (mTcpClientConnector == null)
            mTcpClientConnector = new TcpClient();
        return mTcpClientConnector;
    }

    public  TcpClient()
    {
        m_MessageRoute = new MessageRoute(this,new ClientMessageDispatcher());
    }


    public  MessageRoute GetMessageRoute()
    {
        return m_MessageRoute;
    }



    @Override
    public OutputStream GetOutputStream() throws IOException {
        return  mClient.getOutputStream();
    }
    @Override
    public Socket GetSocket()
    {
        return  mClient;
    }

    public void creatConnect(final String mSerIP, final int mSerPort) {
        if (mConnectThread == null) {
            mConnectThread = new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        Log.d("Tcp Connect", "run:ConnectAndProcess ");
                        ConnectAndProcess(mSerIP, mSerPort);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            });
            mConnectThread.start();
        }
    }

    /**
     * 与服务端进行连接
     *
     * @throws IOException
     */
    private void ConnectAndProcess(String mSerIP, int mSerPort) throws IOException {


        Log.d("Tcp Connect", "before mClient create");
        if (mClient == null) {
            mClient = new Socket(mSerIP, mSerPort);
        }

        Intent i = new Intent(BaseProcessor.MessageFilter);
        i.putExtra("Connect Success",true);
        MainActivity.Instance.sendBroadcast(i);


        while (!mClient.isClosed())
        {
            m_MessageRoute.ProcessReceive(mClient.getInputStream());
            m_MessageRoute.ProcessSend(mClient.getOutputStream());
        }


    }


    /**
     * 断开连接
     *
     * @throws IOException
     */
    public void disconnect() throws IOException {
        if (mClient != null) {
            mClient.close();
            mClient = null;
        }
    }


}

