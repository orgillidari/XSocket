﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TCP : Interface {

	// Use this for initialization
	void Start () {
        Create();
        Connect();
	}
	
	// Update is called once per frame
	void Update () {
        if (IsRun())
            OnRun();

        SendStream s = new SendStream();
        s.WriteType(MGS_TYPE.MSG_BYTESTREAM);
        s.WriteInt8(1);
        s.WriteInt16(2);
        s.WriteInt32(3);
        s.WriteInt64(4);
        s.WriteFloat(5.6f);
        s.WriteDouble(7.8);
        s.WriteString("client");
        int[] b = { 1, 2, 3, 4, 5 };
        s.WriteInt32s(b);
        s.Finish();
        SendMsg(s.Array);
    }

    private void OnDestroy()
    {
        Close();
    }

    public override void OnMsg(byte[] data)
    {

    }
}