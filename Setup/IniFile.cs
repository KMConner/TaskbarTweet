using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Setup
{
    static class IniFile
    {
        [DllImport("kernel32", CharSet = CharSet.Unicode)]
        static extern long WritePrivateProfileString(string section, string key, string value, string filePath);

        [DllImport("kernel32", CharSet = CharSet.Unicode)]
        static extern int GetPrivateProfileString(string section, string key, string defaultValue, StringBuilder retVal, int size, string filePath);

        public static string Read(string filePath, string key, string section)
        {
            var retVal = new StringBuilder(255);
            GetPrivateProfileString(section, key, "", retVal, 255, filePath);
            return retVal.ToString();
        }

        public static void Write(string filePath, string key, string value, string section)
        {
            bool ret = WritePrivateProfileString(section, key, value, filePath) != 0;
            if (!ret)
            {
                throw new Exception("Failed to write to ini file");
            }
        }
    }
}
