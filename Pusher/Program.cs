using System;
using System.Threading.Tasks;
using System.Threading;

namespace Pusher
{
    class Program
    {
        static void Main(string[] args)
        {
            AccountSettings[] settings = AccountSettings.LoadSettings("settings.json");
            var server = new PipeServer(@"tweet", settings);
            server.ReceiveConnectionAsync(CancellationToken.None).Wait();
        }
    }
}
