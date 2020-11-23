using System.Diagnostics;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;

namespace Pusher
{
    class Program
    {
        static void Main(string[] args)
        {
            CreateHostBuilder(args, !Debugger.IsAttached).Build().Run();
        }

        public static IHostBuilder CreateHostBuilder(string[] args, bool isWindowsService)
        {
            IHostBuilder hostBuilder = Host.CreateDefaultBuilder(args);
            if (isWindowsService)
            {
                hostBuilder = hostBuilder.UseWindowsService();
            }

            return hostBuilder.ConfigureServices((hostContext, services) =>
            {
                services.AddHostedService<ServiceWorker>();
            });
        }
    }
}
