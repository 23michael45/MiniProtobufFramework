package com.dd.androidprotobuf;

import android.content.Intent;

import com.google.protobuf.GeneratedMessageLite;

import BaseCmd.Cmd;

public class SigninProcessor extends  BaseProcessor {

    @Override
    void Process(MessageRoute route, GeneratedMessageLite msg)
    {
        Cmd.rspSignin rsp = (Cmd.rspSignin)msg;
        rsp.getName();
        rsp.getSucc();


        Intent i = new Intent(BaseProcessor.MessageFilter);
        i.putExtra(Intent.EXTRA_TEXT,rsp.getName() + " " + rsp.getSucc());

        //i.setData()
        MainActivity.Instance.sendBroadcast(i);
    }
}
