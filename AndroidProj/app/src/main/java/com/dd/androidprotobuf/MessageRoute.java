package com.dd.androidprotobuf;

import android.util.Log;

import com.google.protobuf.GeneratedMessageLite;
import com.google.protobuf.MessageLite;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.Socket;

import BaseCmd.Cmd;

public class MessageRoute {

    public  MessageRoute(BaseSocketConnection bc,BaseMessageDispatcher dispather)
    {
        m_MessageDispatcher = dispather;
        m_BaseSocketConnection = bc;
    }


    private  BaseSocketConnection m_BaseSocketConnection;
    private  BaseMessageDispatcher m_MessageDispatcher = null;

    private  String HeadString = "pbh\0";

    public  static String JavaClass2ProtoTypeName(String className)
    {
        String stype = className.replaceAll("class BaseCmd.Cmd\\$","BaseCmd.");
        return stype;
    }
    public  static  String ProtoTypeName2JavaClassName(String typeName)
    {
        String className = typeName.replaceAll("BaseCmd\\.","class BaseCmd.Cmd\\$");
        return className;
    }



    public void ProcessReceive(InputStream buf) throws IOException {
        Parse(buf);
    }
    public void ProcessSend(OutputStream buf)
    {

    }

    private void Parse(InputStream buf) throws IOException {


        Log.d("Tcp Connect", "before Parse");
        InputStream inputStream = buf;

        byte []head = new byte[4];
        inputStream.read(head,0,4);

        String shead = new String(head);
        if(shead.equals(HeadString))
        {
            byte[] btotallen = new byte[4];
            inputStream.read(btotallen,0,4);
            int totallen = bytesToIntLittle(btotallen,0);

            byte[] bheadlen = new byte[4];
            inputStream.read(bheadlen,0,4);
            int headlen = bytesToIntLittle(bheadlen,0);


            byte[] bheaddata = new byte[headlen];
            byte[] bbodydata = new byte[totallen - headlen - 8];

            inputStream.read(bheaddata);
            inputStream.read(bbodydata);

            Cmd.CmdType headdata = Cmd.CmdType.parseFrom(bheaddata);

            Object bodydatamsg = parseDynamic(headdata.getType(),bbodydata);



            //do dispatch
            m_MessageDispatcher.Dispatcher(this,bodydatamsg.getClass().toString(),(GeneratedMessageLite)bodydatamsg);
        }
        else
        {

            Log.d("Tcp Connect　Head not right:", shead);
        }
    }


    /**
     * 发送数据
     *
     * @param data 需要发送的内容
     */
    public <T extends GeneratedMessageLite> void Send(final T msg) throws IOException {


        new Thread(new Runnable() {
            @Override
            public void run() {
                new Thread(new WriteRunnable(m_BaseSocketConnection.GetSocket(),msg)).start();
            }
        }).start();

    }




    public static Object parseDynamic(String type, byte[] bytes) {
        try {

            type = ProtoTypeName2JavaClassName(type);

            type = type.replace("class ","");

            Log.d("Tcp Connect　parseDynamic:", type);
            Class<?> clazz = Class.forName(type);
            Method method = clazz.getDeclaredMethod("parseFrom", byte[].class);
            return method.invoke(null, bytes);
        } catch (NoSuchMethodException e) {
            throw new IllegalArgumentException("Non-message type", e);
        } catch (IllegalAccessException e) {
            throw new IllegalArgumentException("Non-message type", e);
        } catch (InvocationTargetException e) {
            // TODO: Work out what exactly you want to do.
            throw new IllegalArgumentException("Bad data?", e);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        return null;
    }


    /**
     * 以大端模式将int转成byte[]
     */
    public static byte[] intToBytesBig(int value) {
        byte[] src = new byte[4];
        src[0] = (byte) ((value >> 24) & 0xFF);
        src[1] = (byte) ((value >> 16) & 0xFF);
        src[2] = (byte) ((value >> 8) & 0xFF);
        src[3] = (byte) (value & 0xFF);
        return src;
    }

    /**
     * 以小端模式将int转成byte[]
     *
     * @param value
     * @return
     */
    public static byte[] intToBytesLittle(int value) {
        byte[] src = new byte[4];
        src[3] = (byte) ((value >> 24) & 0xFF);
        src[2] = (byte) ((value >> 16) & 0xFF);
        src[1] = (byte) ((value >> 8) & 0xFF);
        src[0] = (byte) (value & 0xFF);
        return src;
    }

    /**
     * 以大端模式将byte[]转成int
     */
    public static int bytesToIntBig(byte[] src, int offset) {
        int value;
        value = (int) (((src[offset] & 0xFF) << 24)
                | ((src[offset + 1] & 0xFF) << 16)
                | ((src[offset + 2] & 0xFF) << 8)
                | (src[offset + 3] & 0xFF));
        return value;
    }

    /**
     * 以小端模式将byte[]转成int
     */
    public static int bytesToIntLittle(byte[] src, int offset) {
        int value;
        value = (int) ((src[offset] & 0xFF)
                | ((src[offset + 1] & 0xFF) << 8)
                | ((src[offset + 2] & 0xFF) << 16)
                | ((src[offset + 3] & 0xFF) << 24));
        return value;
    }




    // You can put this class outside activity with public scope
    class ReaderRunnable implements Runnable {
        Socket socket;

        public ReaderRunnable(Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            if (socket != null && socket.isConnected()) {
                try {
                    OutputStream out = new BufferedOutputStream(socket.getOutputStream());
                    //Do reader code
                } catch (IOException e) {
                    e.printStackTrace();
                }

            } else {
                //Handle error case
            }
        }
    }

    // You can put this class outside activity with public scope
    class WriteRunnable implements Runnable {
        Socket socket;
        GeneratedMessageLite msg;
        public WriteRunnable(Socket socket,GeneratedMessageLite msg) {
            this.socket = socket;
            this.msg = msg;
        }

        @Override
        public void run() {
            if (socket != null && socket.isConnected()) {
                try {


                    byte[] bbody = msg.toByteArray();
                    String stype = msg.getClass().toString();
                    stype = JavaClass2ProtoTypeName(stype);
                    int bodylen = bbody.length;



                    Cmd.CmdType headdata = Cmd.CmdType.newBuilder().setType(stype).setSize(bodylen).build();

                    byte[] bhead = headdata.toByteArray();
                    int headlen = bhead.length;


                    int totallen = 8 + headlen + bodylen;


                    byte[]  bshead = HeadString.getBytes();

                    OutputStream outputStream = socket.getOutputStream();
                    outputStream.write(bshead,0,4);
                    outputStream.write(intToBytesLittle(totallen),0,4);
                    outputStream.write(intToBytesLittle(headlen),0,4);
                    outputStream.write(bhead);
                    outputStream.write(bbody);


                    //Do writer code
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else {
                //Handle error case
            }
        }
    }
}
