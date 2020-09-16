using System.Runtime.Serialization;

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
    }
}
