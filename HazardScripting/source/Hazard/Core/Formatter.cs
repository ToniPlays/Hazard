using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class Formatter
    {
        public static string Format(string format, object[] parameters) => string.Format(format, parameters);
        public static string Format(object value) => value != null ? value.ToString() : "";
    }
}
