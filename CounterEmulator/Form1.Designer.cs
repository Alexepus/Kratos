namespace CounterEmulator
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.ChbWithAccountOfTime = new System.Windows.Forms.CheckBox();
            this.ChbHighVoltageInhibit = new System.Windows.Forms.CheckBox();
            this.ChbStartMode = new System.Windows.Forms.CheckBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label8 = new System.Windows.Forms.Label();
            this.TbProtectionThreshold = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.TbThreshold = new System.Windows.Forms.TextBox();
            this.TbCountTime = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.TbLastCounter = new System.Windows.Forms.TextBox();
            this.TbCurrentCounter = new System.Windows.Forms.TextBox();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.BtConnect = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.LbConnectStatus = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.CobComPort = new System.Windows.Forms.ComboBox();
            this.LabelSend = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            this.serialPort1.ErrorReceived += new System.IO.Ports.SerialErrorReceivedEventHandler(this.serialPort1_ErrorReceived);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(18, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(32, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Порт";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.ChbWithAccountOfTime);
            this.groupBox1.Controls.Add(this.ChbHighVoltageInhibit);
            this.groupBox1.Controls.Add(this.ChbStartMode);
            this.groupBox1.Location = new System.Drawing.Point(16, 86);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(310, 100);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Состояние";
            // 
            // ChbWithAccountOfTime
            // 
            this.ChbWithAccountOfTime.AutoSize = true;
            this.ChbWithAccountOfTime.Checked = global::CounterEmulator.Properties.Settings.Default.WithAccountOfTime;
            this.ChbWithAccountOfTime.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::CounterEmulator.Properties.Settings.Default, "WithAccountOfTime", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.ChbWithAccountOfTime.Location = new System.Drawing.Point(53, 43);
            this.ChbWithAccountOfTime.Name = "ChbWithAccountOfTime";
            this.ChbWithAccountOfTime.Size = new System.Drawing.Size(118, 17);
            this.ChbWithAccountOfTime.TabIndex = 4;
            this.ChbWithAccountOfTime.Text = "С учетом времени";
            this.ChbWithAccountOfTime.UseVisualStyleBackColor = true;
            this.ChbWithAccountOfTime.CheckedChanged += new System.EventHandler(this.ChbWithAccountOfTime_CheckedChanged);
            // 
            // ChbHighVoltageInhibit
            // 
            this.ChbHighVoltageInhibit.AutoSize = true;
            this.ChbHighVoltageInhibit.Checked = global::CounterEmulator.Properties.Settings.Default.HighVoltageInhibit;
            this.ChbHighVoltageInhibit.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::CounterEmulator.Properties.Settings.Default, "HighVoltageInhibit", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.ChbHighVoltageInhibit.Location = new System.Drawing.Point(33, 67);
            this.ChbHighVoltageInhibit.Name = "ChbHighVoltageInhibit";
            this.ChbHighVoltageInhibit.Size = new System.Drawing.Size(114, 17);
            this.ChbHighVoltageInhibit.TabIndex = 3;
            this.ChbHighVoltageInhibit.Text = "Запрет высокого";
            this.ChbHighVoltageInhibit.UseVisualStyleBackColor = true;
            this.ChbHighVoltageInhibit.CheckedChanged += new System.EventHandler(this.ChbHighVoltageInhibit_CheckedChanged);
            // 
            // ChbStartMode
            // 
            this.ChbStartMode.AutoSize = true;
            this.ChbStartMode.Checked = global::CounterEmulator.Properties.Settings.Default.StartMode;
            this.ChbStartMode.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::CounterEmulator.Properties.Settings.Default, "StartMode", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.ChbStartMode.Location = new System.Drawing.Point(33, 19);
            this.ChbStartMode.Name = "ChbStartMode";
            this.ChbStartMode.Size = new System.Drawing.Size(103, 17);
            this.ChbStartMode.TabIndex = 2;
            this.ChbStartMode.Text = "Режим \"Старт\"";
            this.ChbStartMode.UseVisualStyleBackColor = true;
            this.ChbStartMode.CheckedChanged += new System.EventHandler(this.ChbStartMode_CheckedChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.TbProtectionThreshold);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.TbThreshold);
            this.groupBox2.Controls.Add(this.TbCountTime);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Location = new System.Drawing.Point(16, 193);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(310, 97);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Конфигурация";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(49, 69);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(81, 13);
            this.label8.TabIndex = 5;
            this.label8.Text = "Порог защиты";
            // 
            // TbProtectionThreshold
            // 
            this.TbProtectionThreshold.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::CounterEmulator.Properties.Settings.Default, "ProtectionThreshold", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.TbProtectionThreshold.Location = new System.Drawing.Point(139, 66);
            this.TbProtectionThreshold.Name = "TbProtectionThreshold";
            this.TbProtectionThreshold.Size = new System.Drawing.Size(100, 20);
            this.TbProtectionThreshold.TabIndex = 4;
            this.TbProtectionThreshold.Text = global::CounterEmulator.Properties.Settings.Default.ProtectionThreshold;
            this.TbProtectionThreshold.TextChanged += new System.EventHandler(this.TbProtectionThreshold_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(10, 42);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(123, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "Порог детектирования";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(245, 17);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(40, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "*10 мс";
            // 
            // TbThreshold
            // 
            this.TbThreshold.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::CounterEmulator.Properties.Settings.Default, "Threshold", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.TbThreshold.Location = new System.Drawing.Point(140, 39);
            this.TbThreshold.Name = "TbThreshold";
            this.TbThreshold.Size = new System.Drawing.Size(100, 20);
            this.TbThreshold.TabIndex = 1;
            this.TbThreshold.Text = global::CounterEmulator.Properties.Settings.Default.Threshold;
            this.TbThreshold.TextChanged += new System.EventHandler(this.TbThreshold_TextChanged);
            // 
            // TbCountTime
            // 
            this.TbCountTime.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::CounterEmulator.Properties.Settings.Default, "CountTime", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.TbCountTime.Location = new System.Drawing.Point(140, 13);
            this.TbCountTime.Name = "TbCountTime";
            this.TbCountTime.Size = new System.Drawing.Size(100, 20);
            this.TbCountTime.TabIndex = 1;
            this.TbCountTime.Text = global::CounterEmulator.Properties.Settings.Default.CountTime;
            this.TbCountTime.TextChanged += new System.EventHandler(this.TbCountTime_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 16);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(128, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Время работы счетчика";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.TbLastCounter);
            this.groupBox3.Controls.Add(this.TbCurrentCounter);
            this.groupBox3.Location = new System.Drawing.Point(16, 299);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(310, 67);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Счет";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 41);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(123, 13);
            this.label6.TabIndex = 3;
            this.label6.Text = "Предыдущее значение";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 16);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(102, 13);
            this.label5.TabIndex = 3;
            this.label5.Text = "Текущее значение";
            // 
            // TbLastCounter
            // 
            this.TbLastCounter.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::CounterEmulator.Properties.Settings.Default, "LastCounter", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.TbLastCounter.Location = new System.Drawing.Point(140, 38);
            this.TbLastCounter.Name = "TbLastCounter";
            this.TbLastCounter.Size = new System.Drawing.Size(100, 20);
            this.TbLastCounter.TabIndex = 1;
            this.TbLastCounter.Text = global::CounterEmulator.Properties.Settings.Default.LastCounter;
            this.TbLastCounter.TextChanged += new System.EventHandler(this.TbLastCounter_TextChanged);
            // 
            // TbCurrentCounter
            // 
            this.TbCurrentCounter.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::CounterEmulator.Properties.Settings.Default, "CurrentCounter", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.TbCurrentCounter.Location = new System.Drawing.Point(140, 9);
            this.TbCurrentCounter.Name = "TbCurrentCounter";
            this.TbCurrentCounter.Size = new System.Drawing.Size(100, 20);
            this.TbCurrentCounter.TabIndex = 1;
            this.TbCurrentCounter.Text = global::CounterEmulator.Properties.Settings.Default.CurrentCounter;
            this.TbCurrentCounter.TextChanged += new System.EventHandler(this.TbCurrentCounter_TextChanged);
            // 
            // BtConnect
            // 
            this.BtConnect.Location = new System.Drawing.Point(182, 17);
            this.BtConnect.Name = "BtConnect";
            this.BtConnect.Size = new System.Drawing.Size(75, 23);
            this.BtConnect.TabIndex = 7;
            this.BtConnect.Text = "Соединить";
            this.BtConnect.UseVisualStyleBackColor = true;
            this.BtConnect.Click += new System.EventHandler(this.BtConnect_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.LbConnectStatus);
            this.groupBox4.Controls.Add(this.label7);
            this.groupBox4.Controls.Add(this.CobComPort);
            this.groupBox4.Controls.Add(this.BtConnect);
            this.groupBox4.Controls.Add(this.label1);
            this.groupBox4.Location = new System.Drawing.Point(15, 7);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(311, 68);
            this.groupBox4.TabIndex = 8;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Соединение";
            // 
            // LbConnectStatus
            // 
            this.LbConnectStatus.AutoSize = true;
            this.LbConnectStatus.Location = new System.Drawing.Point(61, 45);
            this.LbConnectStatus.Name = "LbConnectStatus";
            this.LbConnectStatus.Size = new System.Drawing.Size(63, 13);
            this.LbConnectStatus.TabIndex = 9;
            this.LbConnectStatus.Text = "Отключено";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(17, 45);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(47, 13);
            this.label7.TabIndex = 8;
            this.label7.Text = "Статус: ";
            // 
            // CobComPort
            // 
            this.CobComPort.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::CounterEmulator.Properties.Settings.Default, "ComPort", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.CobComPort.FormattingEnabled = true;
            this.CobComPort.Items.AddRange(new object[] {
            "COM1",
            "COM2",
            "COM3",
            "COM4",
            "COM5",
            "COM6",
            "COM7",
            "COM8",
            "COM9",
            "COM10",
            "COM11",
            "COM12"});
            this.CobComPort.Location = new System.Drawing.Point(54, 17);
            this.CobComPort.Name = "CobComPort";
            this.CobComPort.Size = new System.Drawing.Size(121, 21);
            this.CobComPort.TabIndex = 1;
            this.CobComPort.Text = global::CounterEmulator.Properties.Settings.Default.ComPort;
            // 
            // LabelSend
            // 
            this.LabelSend.AutoSize = true;
            this.LabelSend.Location = new System.Drawing.Point(324, 1);
            this.LabelSend.Name = "LabelSend";
            this.LabelSend.Size = new System.Drawing.Size(13, 13);
            this.LabelSend.TabIndex = 9;
            this.LabelSend.Text = "+";
            this.LabelSend.Click += new System.EventHandler(this.LabelSend_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(342, 382);
            this.Controls.Add(this.LabelSend);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Эмулятор AmpCh_N1_2012";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox CobComPort;
        private System.Windows.Forms.CheckBox ChbStartMode;
        private System.Windows.Forms.CheckBox ChbHighVoltageInhibit;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox ChbWithAccountOfTime;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox TbThreshold;
        private System.Windows.Forms.TextBox TbCountTime;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox TbLastCounter;
        private System.Windows.Forms.TextBox TbCurrentCounter;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Button BtConnect;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label LbConnectStatus;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox TbProtectionThreshold;
        private System.Windows.Forms.Label LabelSend;
    }
}

