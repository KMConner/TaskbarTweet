using System;
using System.IO;
using System.IO.Pipes;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Newtonsoft.Json;
using CoreTweet;

namespace Pusher
{
    class PipeServer
    {
        private readonly string pipeName;

        private readonly Tokens token;
        public PipeServer(string pipeName, AccountSettings[] settings)
        {
            this.pipeName = pipeName;
            AccountSettings setting = settings[0];
            token = Tokens.Create(setting.ConsumerKey, setting.ConsumerKeySecret, setting.AccessToken, setting.AccessTokenSecret);
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
                        {
                            string pipeString = reader.ReadLine();
                            Console.WriteLine(pipeString);
                            PipeData data = JsonConvert.DeserializeObject<PipeData>(pipeString);
                            await token.Statuses.UpdateAsync(data.GetText(), cancellationToken: cancellationToken);
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
