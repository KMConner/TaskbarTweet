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

        public string GetText()
        {
            ReadOnlySpan<ushort> textSpan = Wchars.AsSpan();
            var oge = MemoryMarshal.Cast<ushort, byte>(textSpan);
            return Encoding.Unicode.GetString(MemoryMarshal.Cast<ushort, byte>(textSpan));
        }
    }
}
