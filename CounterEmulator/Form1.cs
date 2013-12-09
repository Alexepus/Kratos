using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CounterEmulator.Properties;

namespace CounterEmulator
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            TbThreshold_TextChanged(this, null);
            TbLastCounter_TextChanged(this, null);
            TbProtectionThreshold_TextChanged(this, null);
            TbLastCounter_TextChanged(this, null);
            TbCountTime_TextChanged(this, null);
        }

        private int _countTime;
        private int _threshold;
        private int _currentCounter;
        private int _lastCounter;
        private int _protectionThreshold;
        private const byte UnitAddress = 0x40;
        private bool _configLoaded = false;

        private void ChbStartMode_CheckedChanged(object sender, EventArgs e)
        {
            Config.StartMode = ChbStartMode.Checked;
        }

        private void ChbWithAccountOfTime_CheckedChanged(object sender, EventArgs e)
        {
            Config.WithAccountOfTime = ChbWithAccountOfTime.Checked;
        }

        private void ChbHighVoltageInhibit_CheckedChanged(object sender, EventArgs e)
        {
            Config.HighVoltageInhibit = ChbHighVoltageInhibit.Checked;
        }

        private void BtConnect_Click(object sender, EventArgs e)
        {
            Config.ComPort = CobComPort.Text;

            try
            {
                if(serialPort1.IsOpen)
                    serialPort1.Close();
                serialPort1.PortName = Config.ComPort;
                serialPort1.Open();
                LbConnectStatus.Text = serialPort1.IsOpen ? "Порт открыт" : "Отключено";
            }
            catch (Exception ex)
            {
                LbConnectStatus.Text = ex.Message;
            }


            Config.Save();
        }

        Settings Config { get { return Settings.Default; } }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Config.Save();
            if (serialPort1.IsOpen)
                serialPort1.Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            TbCountTime_TextChanged(sender, e);
            TbThreshold_TextChanged(sender, e);
            TbCurrentCounter_TextChanged(sender, e);
            TbLastCounter_TextChanged(sender, e);
            BtConnect_Click(sender, e);
        }

        private string CoerceValue(string val, int lowerLimit, int higherLimit)
        {
            int intVal;
            int.TryParse(val, out intVal);
            if (intVal < lowerLimit)
                intVal = lowerLimit;
            if (intVal > higherLimit)
                intVal = higherLimit;
            return intVal.ToString();
        }

        private void HandleTextChange(TextBox tb, int lowerLimit, int higherLimit, out int val)
        {
            int.TryParse(tb.Text, out val);
            if (val < lowerLimit)
                val = lowerLimit;
            if (val > higherLimit)
                val = higherLimit;
            string coerced = val.ToString();
            if(coerced != tb.Text)
            {
                tb.Text = coerced;
            }
            toolTip1.SetToolTip(tb, string.Format("0x{0:X}", val));
        }

        private void TbCountTime_TextChanged(object sender, EventArgs e)
        {
            HandleTextChange(TbCountTime, 0, 65535, out _countTime);
            Config.CountTime = TbCountTime.Text;
        }

        private void TbThreshold_TextChanged(object sender, EventArgs e)
        {
            HandleTextChange(TbThreshold, 0, 255, out _threshold);
            Config.Threshold = TbThreshold.Text;
        }

        private void TbProtectionThreshold_TextChanged(object sender, EventArgs e)
        {
            HandleTextChange(TbProtectionThreshold, 0, 65535, out _protectionThreshold);
            Config.ProtectionThreshold = TbProtectionThreshold.Text;
        }

        private void TbCurrentCounter_TextChanged(object sender, EventArgs e)
        {
            HandleTextChange(TbCurrentCounter, 0, 16777215, out _currentCounter);
            Config.CurrentCounter = TbCurrentCounter.Text;
        }

        private void TbLastCounter_TextChanged(object sender, EventArgs e)
        {
            HandleTextChange(TbLastCounter, 0, 16777215, out _lastCounter);
            Config.LastCounter = TbLastCounter.Text;
        }

        private void serialPort1_ErrorReceived(object sender, System.IO.Ports.SerialErrorReceivedEventArgs e)
        {
            LbConnectStatus.Text = "Ошибка: " + e.EventType;
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            byte[] buffer = new byte[32];
            int actualRead = serialPort1.Read(buffer, 0, buffer.Length);

            string sbuffer = Encoding.ASCII.GetString(buffer.Take(actualRead).ToArray());
            var commands = sbuffer.Split(new char[]{'\r'}, StringSplitOptions.RemoveEmptyEntries);
            foreach (var command in commands)
            {
                var parsedCmd = AdamMessage.Parse(command);
                if (parsedCmd.StartMark != '#')
                    return;
                if (parsedCmd.UnitAddress != UnitAddress)
                    return;
                if (!parsedCmd.CommandNumber.HasValue || parsedCmd.CommandNumber.Value > 8)
                {
                    return;
                }

                var reply = parsedCmd.CreateReply();
                switch (parsedCmd.CommandNumber.Value)
                {
                    case 0: // Чтение имени модуля
                        reply.CommandData = "AmpCh_N1_2012\\0";
                        break;
                    case 1: // Чтение состояния
                        reply.CommandData = string.Format("{0}{1}{2}", ChbStartMode.Checked ? "1" : "0",
                                                          ChbHighVoltageInhibit.Checked ? "1" : "0",
                                                          _configLoaded? "1" : "0");
                        break;
                    case 2: // Старт/стоп без учета времени
                        if (parsedCmd.CommandData.Length == 1)
                        {
                            bool startMode = parsedCmd.CommandData[0] == '1';
                            Invoke((Action)(() =>
                            {
                                ChbStartMode.Checked = startMode;
                                ChbWithAccountOfTime.Checked = false;
                            }));
                        }
                        reply.CommandData = ChbStartMode.Checked ? "1" : "0";
                        break;
                    case 3: // Старт/стоп c учетом времени
                        if (parsedCmd.CommandData.Length == 1)
                        {
                            bool startMode = parsedCmd.CommandData[0] == '1';
                            Invoke((Action) (() =>
                                                 {
                                                     ChbStartMode.Checked = startMode;
                                                     ChbWithAccountOfTime.Checked = true;
                                                 }));
                        }
                        reply.CommandData = ChbStartMode.Checked ? "1" : "0";
                        break;
                    case 4: // Задание или чтение времени работы счетчика
                        if (parsedCmd.CommandData.Length == 4)
                        {
                            int time = int.Parse(parsedCmd.CommandData, NumberStyles.HexNumber);
                            Invoke((Action)(()=>TbCountTime.Text = time.ToString()));
                            _configLoaded = true;
                        }
                        reply.CommandData = _countTime.ToString("X4");
                        break;
                    case 5: // Задание или чтение порога
                        if (parsedCmd.CommandData.Length == 2)
                        {
                            int threshold = int.Parse(parsedCmd.CommandData, NumberStyles.HexNumber);
                            Invoke((Action)(()=>TbThreshold.Text = threshold.ToString()));
                            _configLoaded = true;
                        }
                        reply.CommandData = _threshold.ToString("X4");
                        break;
                    case 6: // Задание или чтение параметров охлаждения
                        if (parsedCmd.CommandData.Length == 4)
                        {
                            int threshold = int.Parse(parsedCmd.CommandData, NumberStyles.HexNumber);
                            Invoke((Action)(()=>TbProtectionThreshold.Text = threshold.ToString()));
                            _configLoaded = true;
                        }
                        var rnd = new Random();
                        int threshholdDif = rnd.Next(1, 10);
                        int measured = _protectionThreshold + (ChbHighVoltageInhibit.Checked ? -threshholdDif : threshholdDif);
                        if (measured < 0)
                            measured = 0;
                        reply.CommandData = string.Format("{0:X4}{1:X4}{2}", _protectionThreshold, measured,
                                                          ChbHighVoltageInhibit.Checked ? 0 : 1);
                        break;
                    case 7: // Чтение законченного или предыдущего значения счетчика
                        reply.CommandData = ChbStartMode.Checked
                                                ? _lastCounter.ToString("X6")
                                                : _currentCounter.ToString("X6");
                        break;
                    case 8: // Чтение текущего значения счетчика
                        reply.CommandData = _currentCounter.ToString("X6");
                        break;
                    default:
                        reply.StartMark = AdamMessage.StartMarks.ReplyError;
                        break;
                }
                serialPort1.Write(reply.ToString());
            }
        }

        private void LabelSend_Click(object sender, EventArgs e)
        {
            var form = new SendForm(serialPort1);
            form.ShowDialog();
        }
    }
}
