using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

using Util;

namespace Tester
{
    static class Program
    {
        /// <summary>
        /// 해당 응용 프로그램의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            if (!INIMgr.ExistFile())
            {
                MsgBox.Error("Load error : " + INIMgr.GetFileName(), "Error");
                return;
            }

            MainForm main = new MainForm();
            if (!main.Init())
            {
                MsgBox.Error("Failed initialize Tester", "Error");
                return;
            }

            Application.Run(main);
        }
    }
}
