using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false)]
    public class VisualFunc : Attribute
    {
        private string Name;
        public VisualFunc(string name)
        {
            this.Name = name;
        }
    }
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false)]
    public class ExposeFunc : Attribute
    {
        public ExposeFunc()
        {
        }
    }
}
