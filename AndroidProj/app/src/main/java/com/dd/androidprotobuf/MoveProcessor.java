package com.dd.androidprotobuf;

import android.content.Intent;

import com.google.protobuf.GeneratedMessageLite;

import BaseCmd.Cmd;
import BaseCmd.Cmd.rspMove;

public class MoveProcessor extends  BaseProcessor {

    @Override
    void Process(MessageRoute route, GeneratedMessageLite msg)
    {
        rspMove rsp = (rspMove)msg;
        rsp.getError();


        Intent i = new Intent(BaseProcessor.MessageFilter);
        i.putExtra(Intent.EXTRA_TEXT,rsp.getError());
        MainActivity.Instance.sendBroadcast(i);
    }
}
