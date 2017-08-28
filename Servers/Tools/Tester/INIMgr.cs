using System;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;

public static class INIMgr
{
    private const int m_iBufSize = 255;
    private static string m_sFileName = Directory.GetCurrentDirectory() + "\\Tester.ini";

    [DllImport("kernel32")]
    private static extern long WritePrivateProfileString(string section, string key, string val, string path);

    [DllImport("kernel32")]
    private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string path);

    [DllImport("kernel32")]
    private static extern uint GetPrivateProfileInt(string section, string key, int def, string path);
    
    public static bool ExistFile()
    {
        return File.Exists(m_sFileName);
    }

    public static string GetFileName()
    {
        return m_sFileName;
    }

    public static string ReadString(string section, string key, string def)
    {
        StringBuilder str = new StringBuilder(m_iBufSize);
        GetPrivateProfileString(section, key, def, str, m_iBufSize, m_sFileName);
        return str.ToString();
    }

    public static int ReadInt(string section, string key, int def)
    {
        return (int)GetPrivateProfileInt(section, key, def, m_sFileName);
    }

    public static bool ReadBool(string section, string key, bool def)
    {
        int defVal = (def) ? 1 : 0;
        uint iResult = GetPrivateProfileInt(section, key, defVal, m_sFileName);
        return (iResult != 0) ? true : false;
    }
}
