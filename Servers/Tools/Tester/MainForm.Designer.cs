namespace Tester
{
    partial class MainForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.BTN_RUN = new System.Windows.Forms.Button();
            this.BTN_STOP = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.TXT_CREATED = new System.Windows.Forms.TextBox();
            this.TXT_TICK = new System.Windows.Forms.TextBox();
            this.TXT_LOG = new System.Windows.Forms.TextBox();
            this.BTN_CLEAR = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // BTN_RUN
            // 
            this.BTN_RUN.Location = new System.Drawing.Point(498, 12);
            this.BTN_RUN.Name = "BTN_RUN";
            this.BTN_RUN.Size = new System.Drawing.Size(75, 23);
            this.BTN_RUN.TabIndex = 2;
            this.BTN_RUN.Text = "Run";
            this.BTN_RUN.UseVisualStyleBackColor = true;
            this.BTN_RUN.Click += new System.EventHandler(this.OnRunClick);
            // 
            // BTN_STOP
            // 
            this.BTN_STOP.Location = new System.Drawing.Point(579, 12);
            this.BTN_STOP.Name = "BTN_STOP";
            this.BTN_STOP.Size = new System.Drawing.Size(75, 23);
            this.BTN_STOP.TabIndex = 3;
            this.BTN_STOP.Text = "Stop";
            this.BTN_STOP.UseVisualStyleBackColor = true;
            this.BTN_STOP.Click += new System.EventHandler(this.OnStopClick);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(49, 12);
            this.label1.TabIndex = 2;
            this.label1.Text = "Created";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(187, 18);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(60, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "Tick(sec)";
            // 
            // TXT_CREATED
            // 
            this.TXT_CREATED.Location = new System.Drawing.Point(67, 14);
            this.TXT_CREATED.MaxLength = 5;
            this.TXT_CREATED.Name = "TXT_CREATED";
            this.TXT_CREATED.Size = new System.Drawing.Size(103, 21);
            this.TXT_CREATED.TabIndex = 0;
            this.TXT_CREATED.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.OnCreatedInput);
            // 
            // TXT_TICK
            // 
            this.TXT_TICK.Location = new System.Drawing.Point(253, 14);
            this.TXT_TICK.MaxLength = 5;
            this.TXT_TICK.Name = "TXT_TICK";
            this.TXT_TICK.Size = new System.Drawing.Size(103, 21);
            this.TXT_TICK.TabIndex = 1;
            this.TXT_TICK.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.OnTickInput);
            // 
            // TXT_LOG
            // 
            this.TXT_LOG.Location = new System.Drawing.Point(14, 47);
            this.TXT_LOG.Multiline = true;
            this.TXT_LOG.Name = "TXT_LOG";
            this.TXT_LOG.ReadOnly = true;
            this.TXT_LOG.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.TXT_LOG.Size = new System.Drawing.Size(721, 435);
            this.TXT_LOG.TabIndex = 4;
            // 
            // BTN_CLEAR
            // 
            this.BTN_CLEAR.Location = new System.Drawing.Point(660, 12);
            this.BTN_CLEAR.Name = "BTN_CLEAR";
            this.BTN_CLEAR.Size = new System.Drawing.Size(75, 23);
            this.BTN_CLEAR.TabIndex = 5;
            this.BTN_CLEAR.Text = "Clear";
            this.BTN_CLEAR.UseVisualStyleBackColor = true;
            this.BTN_CLEAR.Click += new System.EventHandler(this.OnClearClick);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(747, 494);
            this.Controls.Add(this.BTN_CLEAR);
            this.Controls.Add(this.TXT_LOG);
            this.Controls.Add(this.TXT_TICK);
            this.Controls.Add(this.TXT_CREATED);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.BTN_STOP);
            this.Controls.Add(this.BTN_RUN);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(640, 450);
            this.Name = "MainForm";
            this.Text = "Tester";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OnClosing);
            this.Resize += new System.EventHandler(this.OnResize);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button BTN_RUN;
        private System.Windows.Forms.Button BTN_STOP;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox TXT_CREATED;
        private System.Windows.Forms.TextBox TXT_TICK;
        private System.Windows.Forms.TextBox TXT_LOG;
        private System.Windows.Forms.Button BTN_CLEAR;
    }
}

