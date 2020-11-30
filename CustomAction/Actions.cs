using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Reflection;
using System.IO;

namespace CustomAction
{
    static class Actions
    {
        private const string ServiceName = "TaskbarTweet";
        public static void Install()
        {
            string baseDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string pusherPath = Path.Join(baseDir, "Pusher.exe");
            Console.WriteLine(pusherPath);
            Process.Start("sc.exe", $"create {ServiceName} binPath=\"{pusherPath}\" start=auto").WaitForExit();
        }

        public static void Uninstall()
        {
            string baseDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string pusherPath = Path.Join(baseDir, "Pusher.exe");
            Process.Start("sc.exe", $"delete {ServiceName}").WaitForExit();
        }
    }
}
