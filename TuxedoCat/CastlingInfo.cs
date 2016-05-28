using System;

namespace TuxedoCat
{
    [Flags]
    public enum CastlingInfo
    {
        NONE = 0,
        WHITE_SHORT = 1,
        WHITE_LONG = 2,
        BLACK_SHORT = 4,
        BLACK_LONG = 8
    }
}