using System;
using System.Runtime.InteropServices;
using System.Runtime.Serialization;
using System.Text;

namespace Pusher
{
    [DataContract]
    class PipeData
    {
        [DataMember(Name = "text")]
        public ushort[] Wchars { get; set; }

        [DataMember(Name = "ConsumerKey")]
        public ushort[] ConsumerKeyChars { get; set; }

        [DataMember(Name = "ConsumerSecret")]
        public ushort[] ConsumerSecretChars { get; set; }

        [DataMember(Name ="accessToken")]
        public ushort[] AccessTokenChars { get; set; }

        [DataMember(Name = "accessTokenSecret")]
        public ushort[] AccessTokenSecretChars { get; set; }

        public string Text => GetText(Wchars);

        public string ConsumerKey => GetText(ConsumerKeyChars);

        public string ConsumerSecret => GetText(ConsumerSecretChars);

        public string AccessToken => GetText(AccessTokenChars);

        public string AccessTokenSecret => GetText(AccessTokenSecretChars);

        private static string GetText(ushort[] chars)
        {
            ReadOnlySpan<ushort> textSpan = chars.AsSpan();
            return Encoding.Unicode.GetString(MemoryMarshal.Cast<ushort, byte>(textSpan));
        }
    }
}
