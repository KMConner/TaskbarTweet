using System.Threading;
using System.Threading.Tasks;
using Microsoft.Extensions.Hosting;

namespace Pusher
{
    class ServiceWorker : BackgroundService
    {
        protected override Task ExecuteAsync(CancellationToken stoppingToken)
        {
            var server = new PipeServer(@"tweet");
            return server.ReceiveConnectionAsync(stoppingToken);
        }
    }
}
