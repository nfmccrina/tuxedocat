using System;
using System.Collections.Concurrent;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TuxedoCat
{
    class Program : ICECPQuitCommandHandler
    {
        public Program()
        {
            _tokenSource = new CancellationTokenSource();
        }

        public void Run()
        {
            ICommandFactory commandFactory = new CommandFactory(this);
            IUserInterface ui = new BasicUserInterface(commandFactory);
            Engine engine = new Engine(ui);
            ui.Attach(engine);

            var uiTask = Task.Run(() => ui.Start(_tokenSource.Token));
            uiTask.Wait();
        }

        public void Quit()
        {
            _tokenSource.Cancel();
        }

        static void Main(string[] args)
        {
            Program p = new Program();

            p.Run();
        }

        private CancellationTokenSource _tokenSource;
    }
}
