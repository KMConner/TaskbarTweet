using System;
using System.IO;
using System.IO.Pipes;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Newtonsoft.Json;

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
                            var serializer = new DataContractJsonSerializer(typeof(PipeData));
                            string pipeString = reader.ReadLine();
                            PipeData data = JsonConvert.DeserializeObject<PipeData>(pipeString);
                            Console.WriteLine(data.GetText());
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
