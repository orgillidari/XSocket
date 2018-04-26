﻿using System;
using UnityEngine;

public class MyClient : CppTcpClient {

	// Use this for initialization
	void Start () {
        Init();
    }
	
    private void OnDestroy()
    {
        Close();
    }

    public override void OnMsg(IntPtr data)
    {
        //测试接收数据
        CppRecvStream r = new CppRecvStream(data);
        sbyte n1 = r.ReadInt8();
        Int16 n2 = r.ReadInt16();
        Int32 n3 = r.ReadInt32();
        Int64 n4 = r.ReadInt64();
        float n5 = r.ReadFloat();
        double n6 = r.ReadDouble();
        string n7 = r.ReadString();

        Debug.Log(n1);
        Debug.Log(n2);
        Debug.Log(n3);
        Debug.Log(n4);
        Debug.Log(n5);
        Debug.Log(n6);
        Debug.Log(n7);
    }

    public override void OnUpdate()
    {
        Open();
        Connect();

        //测试发送数据
        CppSendStream s = new CppSendStream();
        s.WriteInt8(1);
        s.WriteInt16(2);
        s.WriteInt32(3);
        s.WriteInt64(4);
        s.WriteFloat(5.6f);
        s.WriteDouble(7.8);
        s.WriteString("Client!");
        s.Finish(MGS_TYPE.MSG_BYTESTREAM);
        SendStream(s.Obj);
    }
}