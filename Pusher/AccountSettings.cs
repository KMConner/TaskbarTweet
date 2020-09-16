using System;
using System.IO;
using System.Runtime.Serialization;
using Newtonsoft.Json;

namespace Pusher
{
    [DataContract]
    class AccountSettings
    {
        [DataMember(Name = "consumer-key")]
        public string ConsumerKey { get; set; }

        [DataMember(Name = "consumer-key-secret")]
        public string ConsumerKeySecret { get; set; }

        [DataMember(Name = "access-token")]
        public string AccessToken { get; set; }

        [DataMember(Name = "access-token-secret")]
        public string AccessTokenSecret { get; set; }

        public bool Validate()
        {
            if (string.IsNullOrEmpty(ConsumerKey) || string.IsNullOrEmpty(ConsumerKeySecret)
                || string.IsNullOrEmpty(AccessToken) || string.IsNullOrEmpty(AccessTokenSecret))
            {
                return false;
            }
            return true;
        }

        public static AccountSettings[] LoadSettings(string filePath)
        {
            AccountSettings[] settings = JsonConvert.DeserializeObject<AccountSettings[]>(File.ReadAllText(filePath));
            if (settings.Length != 1)
            {
                throw new Exception("The number of accounts must be 1.");
            }
            if (!settings[0].Validate())
            {
                throw new Exception("The configuration is invalid.");
            }
            return settings;
        }
    }
}
