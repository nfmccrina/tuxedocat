namespace TuxedoCat
{
    public class Engine : IObserver
    {
        public Engine(IUserInterface ui)
        {
            _ui = ui;
        }

        public void Update(Subject s)
        {
            if (_ui == s)
            {
                _ui.Command.Execute();
            }
        }

        private IUserInterface _ui;
    }
}