using System.Threading;
using System.Threading.Tasks;

namespace TuxedoCat
{
    public interface IUserInterface : ISubject
    {
        Task Start(CancellationToken token);
        Command Command { get; }
    }
}