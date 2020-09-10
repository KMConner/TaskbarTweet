using System;
using System.Threading.Tasks;
using System.Threading;

namespace Pusher
{
    class Program
    {
        static void Main(string[] args)
        {
            var server = new PipeServer(@"\\.\\pipe\\tweet");
            server.ReceiveConnectionAsync(CancellationToken.None).Wait();
        }
    }
}
