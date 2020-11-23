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
            var cts = new CancellationTokenSource();
            Console.CancelKeyPress += (s, e) => cts.Cancel();
            server.ReceiveConnectionAsync(cts.Token).Wait();
        }
    }
}
