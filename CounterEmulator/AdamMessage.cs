using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;

namespace CounterEmulator
{
    public class AdamMessage
    {
        public char? StartMark;
        public byte? UnitAddress;
        public byte? CommandNumber;
        public string CommandData = "";

        public static class StartMarks
        {
            public const char Request = '#';
            public const char Reply = '@';
            public const char ReplyError = '%';
        }

        public AdamMessage CreateReply()
        {
            var rep = new AdamMessage();
            rep.StartMark = StartMarks.Reply;
            rep.UnitAddress = UnitAddress;
            rep.CommandNumber = CommandNumber;
            return rep;
        }

        public static AdamMessage Parse(string s)
        {
            var cmd = new AdamMessage();
            if (s.Length >= 1)
                cmd.StartMark = s[0];
            if (s.Length >= 3)
            {
                byte b;
                if (byte.TryParse(s.Substring(1, 2), NumberStyles.HexNumber, CultureInfo.InvariantCulture, out b))
                    cmd.UnitAddress = b;
            }
            if (s.Length >= 5)
            {
                byte b;
                if (byte.TryParse(s.Substring(3, 2), NumberStyles.HexNumber, CultureInfo.InvariantCulture, out b))
                    cmd.CommandNumber = b;
            }
            if (s.Length >= 6)
                cmd.CommandData = s.Substring(5, s.Length - 5);
            return cmd;
        }

        public override string ToString()
        {
            return string.Format("{0}{1:X2}{2:X2}{3}\r", StartMark, UnitAddress, CommandNumber, CommandData);
        }
    }
}
