using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;
using ZedGraph;

namespace SerialLinkAnalyzer
{
    public partial class Form1 : Form
    {
        GraphPane ChartPane;

        public Form1()
        {
            InitializeComponent();
            ChartInitialize();
            MailslotReaderWorker.RunWorkerAsync();
        }

        // Потоковая функция чтения из mailslot \\.\mailslot\Spec_SerialOut
        private void MailslotReader_DoWork(object sender, DoWorkEventArgs e)
        {
            string fullSlotname = @"\\.\mailslot\Spec_SerialOut";
            IntPtr hMailSlot = MailslotsApi.CreateMailslot(fullSlotname, 0, 0, IntPtr.Zero);
            while(!MailslotReaderWorker.CancellationPending)
            {
                int msgCount = 0, nextSize = 0;
                MailslotsApi.GetMailslotInfo(hMailSlot, 0, ref nextSize, ref msgCount, IntPtr.Zero);
                if (msgCount > 0)
                {
                    byte[] dataRaw = MailslotsApi.ReadFileByteArray(hMailSlot, 1000);
                    SpecIpcdata0 data = BinaryStructConverter.FromByteArray<SpecIpcdata0>(dataRaw);
                    MailslotReaderWorker.ReportProgress(msgCount, data);
                }
                else
                    Thread.Sleep(1);
            }
            MailslotsApi.CloseHandle(hMailSlot);

        }

        private void MailslotReader_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            SpecIpcdata0 data = (SpecIpcdata0)e.UserState;
            if (tbLog.Text.Length > 0)
                tbLog.Text += "\r\n";
            tbLog.Text += SpecIpcdata0ToLog(data);
        }

        private string SpecIpcdata0ToLog(SpecIpcdata0 data)
        {
            //if (!string.IsNullOrEmpty(data.StringMsg))
            //    return data.StringMsg;
            string res;
            if (data.CommandType == (int)SerialMgsTypes.SERIAL_SYNC_MSG)
                res = "S ";
            else if (data.CommandType == (int)SerialMgsTypes.SERIAL_ASYNC_MSG)
                res = "A ";
            else if (data.CommandType == (int)SerialMgsTypes.SERIAL_ERROR_COM_FAILED)
                res = "ERROR_COM_FAILED";
            else if (data.CommandType == (int)SerialMgsTypes.SERIAL_ERROR_TIMEOUT)
                res = "ERROR_TIMEOUT";
            else
                res = "Unknown Message ";
            res += FormatCommand(data.comMsg.Buf, data.comMsg.MsgSize);
            return res;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string fullSlotname = @"\\.\mailslot\Spec_SerialOut";
            IntPtr hMailSlot = MailslotsApi.CreateFile(fullSlotname, FileAccess.Write, FileShare.ReadWrite, 0, FileMode.Open, 0, IntPtr.Zero);
            if(hMailSlot.ToInt32() != MailslotsApi.INVALID_HANDLE_VALUE)
            {
                uint written;
                System.Threading.NativeOverlapped lpOverlapped = new System.Threading.NativeOverlapped();
                SpecIpcdata0 data = new SpecIpcdata0();
                data.CommandType = (int)SerialMgsTypes.SERIAL_SYNC_MSG;
                data.comMsg.Buf[0] = 0x12;
                data.comMsg.Buf[1] = 0x55;
                data.comMsg.MsgSize = 2;
                //data.StringMsg = "Hi!";
                byte[] bdata = BinaryStructConverter.ToByteArray(data);
                MailslotsApi.WriteFile(hMailSlot, bdata, (uint)bdata.Length, out written, ref lpOverlapped);
            }


        }

        private void btClearLog_Click(object sender, EventArgs e)
        {
            tbLog.Text = "";
        }

        private void ChartInitialize()
        {

            ChartPane = Chart.GraphPane;
            ChartPane.Title.IsVisible = false;

            ChartPane.XAxis.Type = AxisType.Date;
            ChartPane.XAxis.Scale.Format = "HH:mm:ss";

            ChartPane.XAxis.MajorGrid.IsVisible = true;
            ChartPane.YAxis.MajorGrid.IsVisible = true;
            ChartPane.XAxis.MajorGrid.DashOff = 0;
            ChartPane.YAxis.MajorGrid.DashOff = 0;
            ChartPane.XAxis.MajorGrid.Color = Color.LightGray;
            ChartPane.YAxis.MajorGrid.Color = Color.LightGray;

            //ChartPane.XAxis.Scale.MakeNewScale()

        }

        private void tbSendCommand_TextChanged(object sender, EventArgs e)
        {
        }

        private byte[] ParseCommand(string text)
        {
            byte[] b = new byte[text.Length];
            int i = 0;
            string[] ss = text.Split(new char[]{' ', ',', ':', ';'}, StringSplitOptions.RemoveEmptyEntries);
            byte converted;
            foreach(string s in ss)
            {
                string s1 =s;
                while(s1.Length > 2)
                {
                    b[i++] = byte.TryParse(s1.Substring(0, 2), System.Globalization.NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture, out converted) ? converted : (byte)0;
                    s1 = s1.Substring(2, s1.Length - 2);
                }
                b[i++] = byte.TryParse(s1, System.Globalization.NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture, out converted) ? converted : (byte)0;
            }
            byte[] res = new byte[i];
            Array.Copy(b, res, i);
            return res;
        }

        private string FormatCommand(byte[] cmdData, int length)
        {
            string res="";
            for (int i = 0; i < length; i++)
                res += cmdData[i].ToString("x2") + " ";
            return res;
        }

        private void tbSendCommand_Leave(object sender, EventArgs e)
        {
            byte[] data = ParseCommand(tbSendCommand.Text);
            tbSendCommand.Text = FormatCommand(data, data.Length);
        }

        private void cbSendCommand_CheckedChanged(object sender, EventArgs e)
        {
            timerSendCommand.Enabled = cbSendCommand.Checked;
            if (cbSendCommand.Checked)
            {
                tbSendCmdInterval_Leave(null, null);
                timerSendCommand_Tick(null, null);
            }

        }

        private void timerSendCommand_Tick(object sender, EventArgs e)
        {
            SendCommand(ParseCommand(tbSendCommand.Text));
        }

        private void tbSendCmdInterval_Leave(object sender, EventArgs e)
        {
            uint res;
            timerSendCommand.Interval = (int)(UInt32.TryParse(tbSendCmdInterval.Text, out res) ? res : 1000u);
            tbSendCmdInterval.Text = timerSendCommand.Interval.ToString();
        }

        void SendCommand(byte[] data)
        {

        }
    }
}
