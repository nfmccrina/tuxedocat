using System;
using System.IO;

namespace TuxedoCat
{
    public class Generator
    {
        private static ulong[] FileMask;
        private static ulong[] RankMask;
        private static ulong[] SWNEMask;
        private static ulong[] NWSEMask;

        private static ulong[] KnightAttacks;
        private static ulong[] KingAttacks;
        private static ulong[] RayAttacksN;
        private static ulong[] RayAttacksS;
        private static ulong[] RayAttacksE;
        private static ulong[] RayAttacksW;
        private static ulong[] RayAttacksNE;
        private static ulong[] RayAttacksNW;
        private static ulong[] RayAttacksSE;
        private static ulong[] RayAttacksSW;

        public void GenerateArrays()
        {
            InitializeFileMask();
            InitializeKingAttacks();
            InitializeKnightAttacks();
            InitializeNWSEMask();
            InitializeRankMask();
            InitializeRayAttacksE();
            InitializeRayAttacksN();
            InitializeRayAttacksNE();
            InitializeRayAttacksNW();
            InitializeRayAttacksS();
            InitializeRayAttacksSE();
            InitializeRayAttacksSW();
            InitializeRayAttacksW();
            InitializeSWNEMask();

            string outputString = WriteArray(FileMask, "FileMask");
            outputString += WriteArray(RankMask, "RankMask");
            outputString += WriteArray(SWNEMask, "SWNEMask");
            outputString += WriteArray(NWSEMask, "NWSEMask");
            outputString += WriteArray(KnightAttacks, "KnightAttacks");
            outputString += WriteArray(KingAttacks, "KingAttacks");
            outputString += WriteArray(RayAttacksN, "RayAttacksN");
            outputString += WriteArray(RayAttacksS, "RayAttacksS");
            outputString += WriteArray(RayAttacksE, "RayAttacksE");
            outputString += WriteArray(RayAttacksW, "RayAttacksW");
            outputString += WriteArray(RayAttacksNE, "RayAttacksNE");
            outputString += WriteArray(RayAttacksNW, "RayAttacksNW");
            outputString += WriteArray(RayAttacksSE, "RayAttacksSE");
            outputString += WriteArray(RayAttacksSW, "RayAttacksSW");

            File.WriteAllText("Z:\\documents\\tmp.txt", outputString);
        }

        private string WriteArray(ulong[] data, string tableName)
        {
            int currentLineLength;
            string output = "";

            output = "uint64_t ";
            output += tableName;
            output += "[";
            output += data.Length;
            output += "] = {";
            output += Environment.NewLine;

            currentLineLength = 0;

            for (int i = 0; i < data.Length; i++)
            {
                output += "0x";
                output += data[i].ToString("X16");
                output += "ULL";

                if (i != (data.Length - 1))
                {
                    output += ",";
                }

                currentLineLength += (data[i].ToString("X16").Length + 6);

                if (currentLineLength > 80)
                {
                    output += Environment.NewLine;
                    currentLineLength = 0;
                }
                else
                {
                    output += " ";
                }
            }

            output += Environment.NewLine;
            output += "};";
            output += Environment.NewLine;
            output += Environment.NewLine;

            return output;
        }

        private void InitializeFileMask()
        {
            FileMask = new ulong[64];
            ulong currentMask;
            ulong currentSquare = 0x0000000000000001UL;

            for (int index = 0; index < 64; index++)
            {
                currentMask = 0x0101010101010101UL;

                for (int shiftIndex = 0; shiftIndex < 8; shiftIndex++)
                {
                    if ((currentSquare & currentMask) != 0x0000000000000000UL)
                    {
                        FileMask[index] = currentMask;
                        break;
                    }

                    currentMask = currentMask << 1;
                }

                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeRankMask()
        {
            RankMask = new ulong[64];
            ulong currentMask;
            ulong currentSquare = 0x0000000000000001UL;

            for (int index = 0; index < 64; index++)
            {
                currentMask = 0x00000000000000FFUL;

                for (int shiftIndex = 0; shiftIndex < 8; shiftIndex++)
                {
                    if ((currentSquare & currentMask) != 0x0000000000000000UL)
                    {
                        RankMask[index] = currentMask;
                        break;
                    }

                    currentMask = currentMask << 8;
                }

                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeSWNEMask()
        {
            SWNEMask = new ulong[64];
            ulong currentMask;
            ulong currentSquare = 0x0000000000000001UL;

            for (int index = 0; index < 64; index++)
            {
                currentMask = 0x0000000000000080UL;

                for (int shiftIndex = 0; shiftIndex < 16; shiftIndex++)
                {
                    if ((currentSquare & currentMask) != 0x0000000000000000UL)
                    {
                        SWNEMask[index] = currentMask;
                        break;
                    }

                    currentMask = currentMask << 8;

                    if (shiftIndex < 7)
                    {
                        currentMask = currentMask | ((0x0000000000000001UL) << (6 - shiftIndex));
                    }
                }

                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeNWSEMask()
        {
            NWSEMask = new ulong[64];
            ulong currentMask;
            ulong currentSquare = 0x0000000000000001UL;

            for (int index = 0; index < 64; index++)
            {
                currentMask = 0x0000000000000001UL;

                for (int shiftIndex = 0; shiftIndex < 16; shiftIndex++)
                {
                    if ((currentSquare & currentMask) != 0x0000000000000000UL)
                    {
                        NWSEMask[index] = currentMask;
                        break;
                    }

                    currentMask = currentMask << 8;

                    if (shiftIndex < 7)
                    {
                        currentMask = currentMask | ((0x0000000000000001UL) << (shiftIndex + 1));
                    }
                }

                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeKnightAttacks()
        {
            KnightAttacks = new ulong[64];
            ulong currentSquare = 0x0000000000000001UL;
            ulong moves;

            for (int i = 0; i < 64; i++)
            {
                moves = 0x0000000000000000UL;
                moves = moves | ((currentSquare & 0x0000FEFEFEFEFEFEUL) << 15);
                moves = moves | ((currentSquare & 0x00007F7F7F7F7F7FUL) << 17);
                moves = moves | ((currentSquare & 0x003F3F3F3F3F3F3FUL) << 10);
                moves = moves | ((currentSquare & 0x3F3F3F3F3F3F3F00UL) >> 6);
                moves = moves | ((currentSquare & 0x7F7F7F7F7F7F0000UL) >> 15);
                moves = moves | ((currentSquare & 0xFEFEFEFEFEFE0000UL) >> 17);
                moves = moves | ((currentSquare & 0xFCFCFCFCFCFCFC00UL) >> 10);
                moves = moves | ((currentSquare & 0x00FCFCFCFCFCFCFCUL) << 6);

                KnightAttacks[i] = moves;
                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeKingAttacks()
        {
            KingAttacks = new ulong[64];
            ulong currentSquare = 0x0000000000000001UL;
            ulong moves;

            for (int i = 0; i < 64; i++)
            {
                moves = 0x0000000000000000UL;
                moves = moves | ((currentSquare & 0x00FFFFFFFFFFFFFFUL) << 8);
                moves = moves | ((currentSquare & 0x007F7F7F7F7F7F7FUL) << 9);
                moves = moves | ((currentSquare & 0x7F7F7F7F7F7F7F7FUL) << 1);
                moves = moves | ((currentSquare & 0x7F7F7F7F7F7F7F7FUL) >> 7);
                moves = moves | ((currentSquare & 0xFFFFFFFFFFFFFF00UL) >> 8);
                moves = moves | ((currentSquare & 0xFEFEFEFEFEFEFE00UL) >> 9);
                moves = moves | ((currentSquare & 0xFEFEFEFEFEFEFEFEUL) >> 1);
                moves = moves | ((currentSquare & 0x00FEFEFEFEFEFEFEUL) << 7);

                KingAttacks[i] = moves;
                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeRayAttacksN()
        {
            RayAttacksN = new ulong[64];
            ulong currentSquare = 0x0000000000000001UL;
            ulong tmpSquare;
            ulong moves;

            for (int i = 0; i < 64; i++)
            {
                moves = 0x0000000000000000UL;
                tmpSquare = currentSquare;

                while ((tmpSquare & 0x00FFFFFFFFFFFFFFUL) != 0x0000000000000000UL)
                {
                    tmpSquare = tmpSquare << 8;
                    moves = moves | tmpSquare;
                }

                RayAttacksN[i] = moves;
                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeRayAttacksS()
        {
            RayAttacksS = new ulong[64];
            ulong currentSquare = 0x0000000000000001UL;
            ulong tmpSquare;
            ulong moves;

            for (int i = 0; i < 64; i++)
            {
                moves = 0x0000000000000000UL;
                tmpSquare = currentSquare;

                while ((tmpSquare & 0xFFFFFFFFFFFFFF00UL) != 0x0000000000000000UL)
                {
                    tmpSquare = tmpSquare >> 8;
                    moves = moves | tmpSquare;
                }

                RayAttacksS[i] = moves;
                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeRayAttacksE()
        {
            RayAttacksE = new ulong[64];
            ulong currentSquare = 0x0000000000000001UL;
            ulong tmpSquare;
            ulong moves;

            for (int i = 0; i < 64; i++)
            {
                moves = 0x0000000000000000UL;
                tmpSquare = currentSquare;

                while ((tmpSquare & 0x7F7F7F7F7F7F7F7FUL) != 0x0000000000000000UL)
                {
                    tmpSquare = tmpSquare << 1;
                    moves = moves | tmpSquare;
                }

                RayAttacksE[i] = moves;
                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeRayAttacksW()
        {
            RayAttacksW = new ulong[64];
            ulong currentSquare = 0x0000000000000001UL;
            ulong tmpSquare;
            ulong moves;

            for (int i = 0; i < 64; i++)
            {
                moves = 0x0000000000000000UL;
                tmpSquare = currentSquare;

                while ((tmpSquare & 0xFEFEFEFEFEFEFEFEUL) != 0x0000000000000000UL)
                {
                    tmpSquare = tmpSquare >> 1;
                    moves = moves | tmpSquare;
                }

                RayAttacksW[i] = moves;
                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeRayAttacksNE()
        {
            RayAttacksNE = new ulong[64];
            ulong currentSquare = 0x0000000000000001UL;
            ulong tmpSquare;
            ulong moves;

            for (int i = 0; i < 64; i++)
            {
                moves = 0x0000000000000000UL;
                tmpSquare = currentSquare;

                while ((tmpSquare & 0x007F7F7F7F7F7F7FUL) != 0x0000000000000000UL)
                {
                    tmpSquare = tmpSquare << 9;
                    moves = moves | tmpSquare;
                }

                RayAttacksNE[i] = moves;
                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeRayAttacksNW()
        {
            RayAttacksNW = new ulong[64];
            ulong currentSquare = 0x0000000000000001UL;
            ulong tmpSquare;
            ulong moves;

            for (int i = 0; i < 64; i++)
            {
                moves = 0x0000000000000000UL;
                tmpSquare = currentSquare;

                while ((tmpSquare & 0x00FEFEFEFEFEFEFEUL) != 0x0000000000000000UL)
                {
                    tmpSquare = tmpSquare << 7;
                    moves = moves | tmpSquare;
                }

                RayAttacksNW[i] = moves;
                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeRayAttacksSE()
        {
            RayAttacksSE = new ulong[64];
            ulong currentSquare = 0x0000000000000001UL;
            ulong tmpSquare;
            ulong moves;

            for (int i = 0; i < 64; i++)
            {
                moves = 0x0000000000000000UL;
                tmpSquare = currentSquare;

                while ((tmpSquare & 0x7F7F7F7F7F7F7F00UL) != 0x0000000000000000UL)
                {
                    tmpSquare = tmpSquare >> 7;
                    moves = moves | tmpSquare;
                }

                RayAttacksSE[i] = moves;
                currentSquare = currentSquare << 1;
            }
        }

        private void InitializeRayAttacksSW()
        {
            RayAttacksSW = new ulong[64];
            ulong currentSquare = 0x0000000000000001UL;
            ulong tmpSquare;
            ulong moves;

            for (int i = 0; i < 64; i++)
            {
                moves = 0x0000000000000000UL;
                tmpSquare = currentSquare;

                while ((tmpSquare & 0xFEFEFEFEFEFEFE00UL) != 0x0000000000000000UL)
                {
                    tmpSquare = tmpSquare >> 9;
                    moves = moves | tmpSquare;
                }

                RayAttacksSW[i] = moves;
                currentSquare = currentSquare << 1;
            }
        }
    }
}