using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;

namespace SerialLinkAnalyzer
{
    public class MailslotsReader : IDisposable
    {
        private IntPtr _hMailSlot = IntPtr.Zero;
        public MailslotsReader(string mailslotName, uint maxMessageSize, uint readTimeout)
        {
            _hMailSlot = MailslotsApi.CreateMailslot(mailslotName, maxMessageSize, readTimeout, IntPtr.Zero);
        }

        /// <summary>
        /// Если есть данные для чтения, возвращает их, иначе через readTimeout возвращает пустой массив байт
        /// </summary>
        public byte[] ReadData()
        {
            int msgCount = 0, nextSize = 0;
            MailslotsApi.GetMailslotInfo(_hMailSlot, 0, ref nextSize, ref msgCount, IntPtr.Zero);
            if (msgCount > 0)
            {
                byte[] dataRaw = MailslotsApi.ReadFileByteArray(_hMailSlot, (uint)nextSize);
                return dataRaw;
            }
            return new byte[0];
        }

        public int GetMessageCount()
        {
            int msgCount = 0, nextSize = 0;
            MailslotsApi.GetMailslotInfo(_hMailSlot, 0, ref nextSize, ref msgCount, IntPtr.Zero);
            return msgCount;
        }

        public void Dispose()
        {
            if(_hMailSlot != IntPtr.Zero)
            {
                MailslotsApi.CloseHandle(_hMailSlot);
                _hMailSlot = IntPtr.Zero;
            }
        }
    }

    public class MailslotsWriter : IDisposable
    {
        private IntPtr _hMailSlot = IntPtr.Zero;

        public void Dispose()
        {
            if (_hMailSlot != IntPtr.Zero)
            {
                MailslotsApi.CloseHandle(_hMailSlot);
                _hMailSlot = IntPtr.Zero;
            }
        }
    }
 
    public class MailslotsApi
    {
        #region DLL Imports
        [DllImport("kernel32.dll")]
        public static extern IntPtr CreateMailslot(string lpName,
                                            uint nMaxMessageSize,
                                            uint lReadTimeout,
                                            IntPtr lpSecurityAttributes);

        [DllImport("kernel32.dll")]
        public static extern bool GetMailslotInfo(IntPtr hMailslot,
                                           int lpMaxMessageSize,
                                           ref int lpNextSize,
                                           ref int lpMessageCount,
                                           IntPtr lpReadTimeout);

        [DllImport("Kernel32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern IntPtr CreateFile(
            string fileName,
            [MarshalAs(UnmanagedType.U4)] FileAccess fileAccess,
            [MarshalAs(UnmanagedType.U4)] FileShare fileShare,
            int securityAttributes,
            [MarshalAs(UnmanagedType.U4)] FileMode creationDisposition,
            int flags,
            IntPtr template);

        [DllImport("kernel32.dll", SetLastError = true)]
        static public extern bool ReadFile(IntPtr hFile, [Out] byte[] lpBuffer, uint nNumberOfBytesToRead,
           out uint lpNumberOfBytesRead, IntPtr lpOverlapped);
        
        static public byte[] ReadFileByteArray(IntPtr hFile, uint nNumberOfBytesToRead)
        {
            byte[] dataRaw = new byte[nNumberOfBytesToRead];
            uint lpNumberOfBytesRead;
            ReadFile(hFile, dataRaw, nNumberOfBytesToRead, out lpNumberOfBytesRead, IntPtr.Zero);
            byte[] data = new byte[lpNumberOfBytesRead];
            Array.Copy(dataRaw, data, data.Length);
            return data;
        }

        //[DllImport("kernel32.dll", SetLastError = true)]
        //public static extern bool ReadFile(
        //    IntPtr hFile,                      // handle to file
        //    void* lpBuffer,                // data buffer
        //    int nNumberOfBytesToRead,       // number of bytes to read
        //    int* lpNumberOfBytesRead,    // number of bytes read
        //    int overlapped
        //    );

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool WriteFile(
          IntPtr hFile,
          byte[] lpBuffer,
          uint nNumberOfBytesToWrite,
          out uint lpNumberOfBytesWritten,
          [In] ref System.Threading.NativeOverlapped lpOverlapped);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool CloseHandle(IntPtr hHandle);

        public const Int32 INVALID_HANDLE_VALUE = -1;
        #endregion
    }
}
