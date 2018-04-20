namespace TuxedoCat
{
    public class CECPQuitCommand : Command
    {
        public CECPQuitCommand(ICECPQuitCommandHandler receiver)
        {
            _receiver = receiver;
        }

        public override void Execute()
        {
            _receiver.Quit();
        }

        private ICECPQuitCommandHandler _receiver;
    }
}