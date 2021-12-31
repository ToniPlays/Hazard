using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class MeshComponent : Component
    {
        

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_Play_Native(ulong id);
    }
}
