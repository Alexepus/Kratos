namespace SerialLinkAnalyzer
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
            this.MailslotReaderWorker = new System.ComponentModel.BackgroundWorker();
            this.BtSend = new System.Windows.Forms.Button();
            this.tbLog = new System.Windows.Forms.TextBox();
            this.Chart = new ZedGraph.ZedGraphControl();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.checkBox3 = new System.Windows.Forms.CheckBox();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.BtClearLog = new System.Windows.Forms.Button();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbSendCmdInterval = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.cbSendCommand = new System.Windows.Forms.CheckBox();
            this.tbSendCommand = new System.Windows.Forms.TextBox();
            this.timerSendCommand = new System.Windows.Forms.Timer(this.components);
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // MailslotReaderWorker
            // 
            this.MailslotReaderWorker.WorkerReportsProgress = true;
            this.MailslotReaderWorker.WorkerSupportsCancellation = true;
            this.MailslotReaderWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.MailslotReader_DoWork);
            this.MailslotReaderWorker.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.MailslotReader_ProgressChanged);
            // 
            // BtSend
            // 
            this.BtSend.Location = new System.Drawing.Point(141, 23);
            this.BtSend.Name = "BtSend";
            this.BtSend.Size = new System.Drawing.Size(97, 23);
            this.BtSend.TabIndex = 1;
            this.BtSend.Text = "Отправить";
            this.BtSend.UseVisualStyleBackColor = true;
            this.BtSend.Click += new System.EventHandler(this.button1_Click);
            // 
            // tbLog
            // 
            this.tbLog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbLog.Location = new System.Drawing.Point(6, 64);
            this.tbLog.Multiline = true;
            this.tbLog.Name = "tbLog";
            this.tbLog.Size = new System.Drawing.Size(756, 464);
            this.tbLog.TabIndex = 2;
            // 
            // Chart
            // 
            this.Chart.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.Chart.Location = new System.Drawing.Point(0, 72);
            this.Chart.Name = "Chart";
            this.Chart.ScrollGrace = 0;
            this.Chart.ScrollMaxX = 0;
            this.Chart.ScrollMaxY = 0;
            this.Chart.ScrollMaxY2 = 0;
            this.Chart.ScrollMinX = 0;
            this.Chart.ScrollMinY = 0;
            this.Chart.ScrollMinY2 = 0;
            this.Chart.Size = new System.Drawing.Size(768, 462);
            this.Chart.TabIndex = 3;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(776, 560);
            this.tabControl1.TabIndex = 4;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.checkBox3);
            this.tabPage1.Controls.Add(this.checkBox2);
            this.tabPage1.Controls.Add(this.checkBox1);
            this.tabPage1.Controls.Add(this.BtClearLog);
            this.tabPage1.Controls.Add(this.tbLog);
            this.tabPage1.Controls.Add(this.BtSend);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(768, 534);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Журнал";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // checkBox3
            // 
            this.checkBox3.AutoSize = true;
            this.checkBox3.Location = new System.Drawing.Point(8, 41);
            this.checkBox3.Name = "checkBox3";
            this.checkBox3.Size = new System.Drawing.Size(110, 17);
            this.checkBox3.TabIndex = 6;
            this.checkBox3.Text = "Незапрошенные";
            this.checkBox3.UseVisualStyleBackColor = true;
            // 
            // checkBox2
            // 
            this.checkBox2.AutoSize = true;
            this.checkBox2.Location = new System.Drawing.Point(8, 23);
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.Size = new System.Drawing.Size(84, 17);
            this.checkBox2.TabIndex = 5;
            this.checkBox2.Text = "Исходящие";
            this.checkBox2.UseVisualStyleBackColor = true;
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(8, 4);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(77, 17);
            this.checkBox1.TabIndex = 4;
            this.checkBox1.Text = "Входящие";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // BtClearLog
            // 
            this.BtClearLog.Location = new System.Drawing.Point(676, 23);
            this.BtClearLog.Name = "BtClearLog";
            this.BtClearLog.Size = new System.Drawing.Size(75, 23);
            this.BtClearLog.TabIndex = 3;
            this.BtClearLog.Text = "Очистить";
            this.BtClearLog.UseVisualStyleBackColor = true;
            this.BtClearLog.Click += new System.EventHandler(this.btClearLog_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.label6);
            this.tabPage2.Controls.Add(this.label5);
            this.tabPage2.Controls.Add(this.label4);
            this.tabPage2.Controls.Add(this.textBox5);
            this.tabPage2.Controls.Add(this.textBox4);
            this.tabPage2.Controls.Add(this.textBox3);
            this.tabPage2.Controls.Add(this.label3);
            this.tabPage2.Controls.Add(this.label2);
            this.tabPage2.Controls.Add(this.tbSendCmdInterval);
            this.tabPage2.Controls.Add(this.label1);
            this.tabPage2.Controls.Add(this.cbSendCommand);
            this.tabPage2.Controls.Add(this.tbSendCommand);
            this.tabPage2.Controls.Add(this.Chart);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(768, 534);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "График";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(433, 39);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(30, 13);
            this.label6.TabIndex = 13;
            this.label6.Text = "байт";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(331, 39);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(40, 13);
            this.label5.TabIndex = 13;
            this.label5.Text = "Длина";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(197, 39);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(61, 13);
            this.label4.TabIndex = 12;
            this.label4.Text = "Смещение";
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(377, 36);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(50, 20);
            this.textBox5.TabIndex = 11;
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(264, 36);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(50, 20);
            this.textBox4.TabIndex = 11;
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(124, 36);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(50, 20);
            this.textBox3.TabIndex = 10;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(8, 39);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(110, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Принимать команду";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(672, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(21, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "мс";
            // 
            // tbSendCmdInterval
            // 
            this.tbSendCmdInterval.Location = new System.Drawing.Point(580, 6);
            this.tbSendCmdInterval.Name = "tbSendCmdInterval";
            this.tbSendCmdInterval.Size = new System.Drawing.Size(86, 20);
            this.tbSendCmdInterval.TabIndex = 7;
            this.tbSendCmdInterval.Leave += new System.EventHandler(this.tbSendCmdInterval_Leave);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(527, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "каждые";
            // 
            // cbSendCommand
            // 
            this.cbSendCommand.AutoSize = true;
            this.cbSendCommand.Location = new System.Drawing.Point(8, 6);
            this.cbSendCommand.Name = "cbSendCommand";
            this.cbSendCommand.Size = new System.Drawing.Size(123, 17);
            this.cbSendCommand.TabIndex = 5;
            this.cbSendCommand.Text = "Посылать команду";
            this.cbSendCommand.UseVisualStyleBackColor = true;
            this.cbSendCommand.CheckedChanged += new System.EventHandler(this.cbSendCommand_CheckedChanged);
            // 
            // tbSendCommand
            // 
            this.tbSendCommand.Location = new System.Drawing.Point(137, 6);
            this.tbSendCommand.Name = "tbSendCommand";
            this.tbSendCommand.Size = new System.Drawing.Size(370, 20);
            this.tbSendCommand.TabIndex = 4;
            this.tbSendCommand.Leave += new System.EventHandler(this.tbSendCommand_Leave);
            // 
            // timerSendCommand
            // 
            this.timerSendCommand.Tick += new System.EventHandler(this.timerSendCommand_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(776, 560);
            this.Controls.Add(this.tabControl1);
            this.Name = "Form1";
            this.Text = "SerialLink Analyzer";
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.ComponentModel.BackgroundWorker MailslotReaderWorker;
        private System.Windows.Forms.Button BtSend;
        private System.Windows.Forms.TextBox tbLog;
        private ZedGraph.ZedGraphControl Chart;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button BtClearLog;
        private System.Windows.Forms.CheckBox checkBox3;
        private System.Windows.Forms.CheckBox checkBox2;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbSendCmdInterval;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox cbSendCommand;
        private System.Windows.Forms.TextBox tbSendCommand;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.Timer timerSendCommand;
    }
}

