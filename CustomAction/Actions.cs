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
            string comPath = Path.Join(baseDir, "TaskbarTweet.dll");
            Console.WriteLine(pusherPath);
            Process.Start("sc.exe", $"create {ServiceName} binPath=\"{pusherPath}\" start=auto").WaitForExit();
            Process.Start("net.exe", $"start \"{ServiceName}\"").WaitForExit();
            Process.Start("regsvr32.exe", $"/s \"{comPath}\"").WaitForExit();
        }

        public static void Uninstall()
        {
            string baseDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string comPath = Path.Join(baseDir, "TaskbarTweet.dll");

            Process.Start("net.exe", $"stop \"{ServiceName}\"").WaitForExit();
            Process.Start("sc.exe", $"delete {ServiceName}").WaitForExit();
            Process.Start("regsvr32.exe", $"/s /u \"{comPath}\"").WaitForExit();
        }
    }
}
