using System;
using System.Collections.Generic;
using System.Linq;

namespace TuxedoCat
{
    public class CommandFactory : ICommandFactory
    {
        public CommandFactory(ICECPQuitCommandHandler quitReceiver)
        {
            _quitReceiver = quitReceiver;
        }

        public Command BuildCommand(IEnumerable<string> args)
        {
            if (args.Count() == 0)
            {
                throw new ArgumentException("CommandFactory: Cannot create command with no arguments.");
            }

            if (args.First() == "quit")
            {
                return new CECPQuitCommand(_quitReceiver);
            }

            return null;
        }

        private ICECPQuitCommandHandler _quitReceiver;
    }
}