using System.Collections.Generic;

namespace TuxedoCat
{
    public interface ICommandFactory
    {
        Command BuildCommand(IEnumerable<string> args);
    }
}