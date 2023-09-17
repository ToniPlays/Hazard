using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class WorldManager
    {
        public static bool LoadWorld(World world)
        {
            unsafe
            {
                return InternalCalls.WorldManager_LoadWorld_Native(world.ID);
            }
        }
        public static bool LoadWorld(string name)
        {
            unsafe
            {
                return InternalCalls.WorldManager_LoadWorldFromString_Native(name);
            }
        }
        public static World GetCurrentWorld()
        {
            unsafe
            {
                ulong id = InternalCalls.WorldManager_GetCurrentWorld_Native();
                if (id == 0)
                    return null;
                return new World(id);
            }
        }

        public static bool ReloadWorld()
        {
            return LoadWorld(GetCurrentWorld());
        }
    }
}
