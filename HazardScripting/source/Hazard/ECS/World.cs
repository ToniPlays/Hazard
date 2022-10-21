using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class World : Asset
    {
        public World() : base(0) { }
        internal World(ulong id) : base(id) { }
        public string GetPath() { return ""; }
        public Entity[] GetEntitiesWith<T>() where T : Component, new() 
        { 
            return null; 
        }
        public Entity[] GetEntities() { return null; }
    }
}
