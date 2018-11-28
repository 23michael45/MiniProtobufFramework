package com.dd.androidprotobuf;

import com.google.protobuf.GeneratedMessageLite;

import java.util.Dictionary;

import BaseCmd.Cmd;

public class ClientMessageDispatcher extends  BaseMessageDispatcher {


    public  ClientMessageDispatcher()
    {
        Cmd.rspSignin rspSignin = Cmd.rspSignin.newBuilder().build();
        m_ProcessorMap.put(rspSignin.getClass().toString(), new SigninProcessor());


        Cmd.rspMove rspMove = Cmd.rspMove.newBuilder().build();
        m_ProcessorMap.put(rspMove.getClass().toString(),new MoveProcessor());
    }

}
