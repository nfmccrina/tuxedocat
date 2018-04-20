using System.Collections.Generic;

namespace TuxedoCat
{
    public abstract class Subject
    {
        public virtual void Attach(IObserver observer)
        {
            _observers.Add(observer);
        }

        public virtual void Detach (IObserver observer)
        {
            _observers.Remove(observer);
        }

        public virtual void Notify ()
        {
            foreach (var observer in _observers)
            {
                observer.Update(this);
            }
        }

        protected List<IObserver> _observers;
    }
}