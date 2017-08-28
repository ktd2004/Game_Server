using System;
using System.Windows.Forms;

namespace Util
{
    public class MsgBox
    {
        public static void Error(string sMsg, string sCaption)
        {
            MessageBox.Show(sMsg, sCaption, MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        public static void Info(string sMsg, string sCaption)
        {
            MessageBox.Show(sMsg, sCaption, MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
    }

    public static class Time
    {
        public static string GetCurrentTime()
        {
            string sFormat = "yyyy-MM-dd HH:mm:ss";
            return DateTime.Now.ToString(sFormat);
        }
    }
}
