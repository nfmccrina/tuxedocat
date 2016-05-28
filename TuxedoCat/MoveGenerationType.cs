using System;

namespace TuxedoCat
{
    [Flags]
    public enum MoveGenerationType
    {
        Legal = 1,
        Captures = 2,
        NonCaptures = 4
    }
}