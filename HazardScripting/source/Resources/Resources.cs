using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{
    public class Resources
    {
        public static ulong GetHandle(string asset) {
            return 0;
        }
        public static AssetMetadata GetMetadata(ulong handle)
        {
            return new AssetMetadata();
        }
        public static T Get<T>(ulong handle) where T : Asset, new() 
        { 
            return null; 
        }
    }
}
