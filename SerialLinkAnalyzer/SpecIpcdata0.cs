using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using  System.Runtime.InteropServices;


namespace SerialLinkAnalyzer
{
    enum SerialMgsTypes{SERIAL_SYNC_MSG=0, SERIAL_ASYNC_MSG=1,SERIAL_ERROR_COM_FAILED=2,SERIAL_ERROR_TIMEOUT=3}
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public class SpecIpcdata0
    {
        public SpecIpcdata0() { comMsg = new ComMsg(); }
        public int CommandType;//Тип сообщения из потока: {SERIAL_SYNC_MSG, SERIAL_ASYNC_MSG,SERIAL_ERROR_COM_FAILED,SERIAL_ERROR_TIMEOUT}
	    public ComMsg comMsg;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 65)]
	    public string StringMsg;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public class ComMsg
    {
        public ComMsg() { WaitForReply = true; Buf = new byte[20]; }
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]
        public	byte[] Buf;
        public	int MsgSize;
        public	bool WaitForReply;
        public	UInt32 TimeStamp; //Служит для выявления таймаутов, заполняется автоматически (после отправки и после 
					     //получения целого сообщения)
    };
}
