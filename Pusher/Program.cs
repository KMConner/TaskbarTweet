using System;
using System.Threading.Tasks;
using System.Threading;

namespace Pusher
{
    class Program
    {
        static void Main(string[] args)
        {
            var server = new PipeServer(@"tweet");
            var cts = new CancellationTokenSource();
            Console.CancelKeyPress += (s, e) => cts.Cancel();
            server.ReceiveConnectionAsync(cts.Token).Wait();
        }
    }
}
