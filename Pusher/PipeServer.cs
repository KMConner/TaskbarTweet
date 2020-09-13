using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.IO.Pipes;
using System.Threading.Tasks;
using System.Threading;

namespace Pusher
{
    class PipeServer
    {
        private string pipeName;
        public PipeServer(string pipeName)
        {
            this.pipeName = pipeName;
        }

        public async Task ReceiveConnectionAsync(CancellationToken cancellationToken)
        {
            while (!cancellationToken.IsCancellationRequested)
            {
                using (var pipe = new NamedPipeServerStream(pipeName, PipeDirection.InOut))
                {
                    try
                    {
                        await pipe.WaitForConnectionAsync(cancellationToken);

                        using (var reader = new StreamReader(pipe, Encoding.ASCII))
                        using (var writer = new StreamWriter(pipe, Encoding.ASCII))
                        {
                            Console.WriteLine(reader.ReadLine());
                            writer.Write("OK");
                            writer.Flush();
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("Error: " + ex.Message);
                    }
                }
            }
        }
    }
}
