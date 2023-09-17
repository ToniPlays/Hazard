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
        //TODO: Binding
        public Entity[] GetEntitiesWith<T>() where T : Component, new()
        {
            unsafe
            {
                ulong[] entityIDs = InternalCalls.World_GetEntitiesWith_Native(ID, typeof(T));
                Entity[] entities = new Entity[entityIDs.Length];

                for (int i = 0; i < entityIDs.Length; i++)
                {
                    entities[i] = new Entity(entityIDs[i]);
                }
                return entities;
            }
        }
    }
}
