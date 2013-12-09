using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CounterEmulator
{
    public partial class SendForm : Form
    {
        private SerialPort _serialPort;
        public SendForm(SerialPort port)
        {
            _serialPort = port;
            InitializeComponent();
        }

        private void ButtonSend_Click(object sender, EventArgs e)
        {
            _serialPort.Write("#"+TextBoxAddress.Text+TextBoxCommand.Text+TextBoxData.Text+"\r");
        }
    }
}
