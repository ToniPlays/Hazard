using System;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Resource
    {
        protected ulong ID { get; private set; }

        protected Resource() { ID = 0; }
        internal Resource(ulong id) {
            ID = id;
        }
        ~Resource() {
            Resource_Destroy_Native(ID);
        }

        protected static T CreateNative<T>() where T : Resource, new() { 
            ulong resourceID = Resource_Create_Native(typeof(T));

            T resource = new T
            {
                ID = resourceID
            };
            return resource;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong Resource_Create_Native(Type type);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Resource_Destroy_Native(ulong id);
    }
}
