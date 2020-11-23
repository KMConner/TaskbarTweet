using System;
using System.IO;
using System.IO.Pipes;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using CoreTweet;
using Newtonsoft.Json;

namespace Pusher
{
    class PipeServer
    {
        private readonly string pipeName;

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
                            string pipeString = reader.ReadLine();
                            PipeData data = JsonConvert.DeserializeObject<PipeData>(pipeString);
                            Tokens token = Tokens.Create(data.ConsumerKey, data.ConsumerSecret, data.AccessToken, data.AccessTokenSecret);
                            try
                            {
                                await token.Statuses.UpdateAsync(data.Text, cancellationToken: cancellationToken);
                            }
                            catch
                            {
                                await writer.WriteLineAsync("-");
                                throw;
                            }
                            await writer.WriteLineAsync("+");
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
