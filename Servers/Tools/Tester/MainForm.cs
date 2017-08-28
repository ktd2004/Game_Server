using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reflection;

namespace Tester
{
    public partial class MainForm : Form
    {
        protected string m_sWebUrl = "";
        protected string m_sHost = "localhost";
        protected ushort m_iPort = 0;
        protected string m_sVersion = "0.0.0";

        protected int m_iCreated = 0;
        protected double m_fTickCnt = 0.0f;

        protected TesterWorker m_Worker = null;
        protected Thread[] m_Threads;

        public MainForm()
        {
            InitializeComponent();
        }

        public bool Init()
        {
            m_sWebUrl = INIMgr.ReadString("Tester", "url", "");
            m_sHost = INIMgr.ReadString("Tester", "host", "localhost");
            m_iPort = (ushort)INIMgr.ReadInt("Tester", "port", 9000);
            m_sVersion = INIMgr.ReadString("Tester", "version", "0.0.0");
            m_Worker = new TesterWorker();

            return true;
        }

        private void CtrlEnable(bool bEnable)
        {
            this.TXT_CREATED.Enabled = bEnable;
            this.TXT_TICK.Enabled = bEnable;
            this.BTN_RUN.Enabled = bEnable;
            this.BTN_CLEAR.Enabled = bEnable;
        }

        private void OnCreatedInput(object sender, KeyPressEventArgs e)
        {
            if (!Char.IsDigit(e.KeyChar) && e.KeyChar != Convert.ToChar(Keys.Back) && e.KeyChar != Convert.ToChar(Keys.Delete))
            {
                e.Handled = true;
            }
        }

        private void OnTickInput(object sender, KeyPressEventArgs e)
        {
            if (!Char.IsDigit(e.KeyChar) && e.KeyChar != Convert.ToChar(Keys.Back) && e.KeyChar != Convert.ToChar(Keys.Delete))
            {
                e.Handled = true;
            }
        }

        private void OnRunClick(object sender, EventArgs e)
        {
            if (m_Worker.IsRunning() && m_Threads != null)
            {
                return;
            }

            string sCreated = this.TXT_CREATED.Text;
            sCreated = sCreated.Trim();
            if (sCreated == "" || sCreated == "0")
            {
                this.TXT_CREATED.Text = sCreated = "1";
            }

            string sTickCnt = this.TXT_TICK.Text;
            sTickCnt = sTickCnt.Trim();
            if (sTickCnt == "" || sTickCnt == "0")
            {
                this.TXT_TICK.Text = sTickCnt = "0.25";
            }

            m_iCreated = Convert.ToInt32(sCreated);
            m_fTickCnt = Convert.ToDouble(sTickCnt);

            if (m_Threads == null)
            {
                m_Threads = new Thread[m_iCreated];
            }

            for (int i = 0; i < m_iCreated; ++i)
            {
                m_Threads[i] = new Thread(() => m_Worker.WorkerMain(m_sWebUrl, i, m_sVersion, m_fTickCnt, this.TXT_LOG));
                m_Threads[i].Start();
            }

            CtrlEnable(false);
        }

        private void OnStopClick(object sender, EventArgs e)
        {
            if (!m_Worker.IsRunning() || m_Threads == null)
            {
                return;
            }

            this.TXT_LOG.AppendText("\r\n");

            m_Worker.Stop();
            for (int i = 0; i < m_iCreated; ++i)
            {
 //               m_Threads[i].Abort();
            }

            Array.Clear(m_Threads, 0, m_iCreated - 1);
            m_Threads = null;
            CtrlEnable(true);
        }

        private void OnClearClick(object sender, EventArgs e)
        {
            this.TXT_LOG.Clear();
        }

        private void OnResize(object sender, EventArgs e)
        {
            // button
            this.BTN_CLEAR.Location = new Point(ClientSize.Width - BTN_CLEAR.Size.Width - 12, BTN_CLEAR.Location.Y);
            this.BTN_STOP.Location = new Point(BTN_CLEAR.Location.X - BTN_STOP.Size.Width - 6, BTN_STOP.Location.Y);
            this.BTN_RUN.Location = new Point(BTN_STOP.Location.X - BTN_RUN.Size.Width - 6, BTN_RUN.Location.Y);

            // log edit
            this.TXT_LOG.Size = new Size(ClientSize.Width - 26, ClientSize.Height - 59);
        }

        private void OnClosing(object sender, FormClosingEventArgs e)
        {
            if (m_Threads != null)
            {
                for (int i = 0; i < m_iCreated; ++i)
                {
                    m_Threads[i].Abort();
                }

                Array.Clear(m_Threads, 0, m_iCreated - 1);
                m_Threads = null;
            }
        }
    }
}
