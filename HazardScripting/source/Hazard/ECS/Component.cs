﻿using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{
    public abstract class Component
    {
        public Entity parentEntity { get; set; }
    }
}
