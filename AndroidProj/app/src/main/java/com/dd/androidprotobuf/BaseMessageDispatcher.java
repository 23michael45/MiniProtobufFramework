package com.dd.androidprotobuf;

import com.google.protobuf.GeneratedMessageLite;

import java.util.Dictionary;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;

public class BaseMessageDispatcher {

    protected Map<String,BaseProcessor> m_ProcessorMap = new HashMap<String, BaseProcessor>();

    public void Dispatcher(MessageRoute route, String  typeName, GeneratedMessageLite msg)
    {
        if(m_ProcessorMap.containsKey(typeName))
        {
            m_ProcessorMap.get(typeName).Process(route,msg);
        }
    }
}
