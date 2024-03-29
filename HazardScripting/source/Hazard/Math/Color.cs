﻿using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace Hazard
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Color
    {
        public float r, g, b, a;

        public Color(float r, float g, float b, float a = 1.0f) {
            this.r = r;
            this.g = g;
            this.b = b;
            this.a = a;
        }

        public static Color RandomColor() 
        { 
            return new Color(Math.RandomRange(0, 1.0f), Math.RandomRange(0, 1.0f), Math.RandomRange(0, 1.0f));        
        }

        public override string ToString()
        {
            return String.Format("[R: {0}, G: {1}, B: {2}, A: {3}]", r, g, b, a);
        }

        public static Color White = new Color(1.0f, 1.0f, 1.0f);
        public static Color Black = new Color(0.0f, 0.0f, 0.0f);
        public static Color Gray = new Color(0.5f, 0.5f, 0.5f);
    }
}
