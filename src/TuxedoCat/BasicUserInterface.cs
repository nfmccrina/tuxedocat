using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TuxedoCat
{
    public class BasicUserInterface : Subject, IUserInterface
    {
        public BasicUserInterface(ICommandFactory commandFactory)
        {
            _commandFactory = commandFactory;
            _observers = new List<IObserver>();
        }

        public async Task Start(CancellationToken token)
        {
            Stream cin = Console.OpenStandardInput();
            Byte[] buf = new Byte[256];
            StringBuilder sb = new StringBuilder();

            while (!token.IsCancellationRequested)
            {
                await cin.ReadAsync(buf, 0, 256);
                sb.Append(Encoding.ASCII.GetString(buf));
                Command = _commandFactory.BuildCommand(sb.ToString().Split('\n')[0].Split(' ').Select(s => s.Trim()));
                sb.Clear();

                Notify();
            }
        }

        public Command Command { get; private set; }

        private ICommandFactory _commandFactory;
    }
}