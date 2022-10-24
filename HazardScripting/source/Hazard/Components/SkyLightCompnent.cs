using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class SkyLightComponent : Component
    {
        public SkyLightComponent() : base(0) { }
        internal SkyLightComponent(ulong ID) : base(ID) { }

        public float Intensity
        {
            get => InternalCalls.SkyLightComponent_GetIntensity_Native(ParentEntity.ID);
            set => InternalCalls.SkyLightComponent_SetIntensity_Native(ParentEntity.ID, value);
        }
        public Rendering.EnvironmentMap GetEnvironmentMap()
        {
            ulong id = InternalCalls.SkyLightComponent_GetEnvironmentMap_Native(ParentEntity.ID);
            if (id == 0) 
                return null;

            return new Rendering.EnvironmentMap(id);
        }
        public void SetEnvironmentMap(Rendering.EnvironmentMap environmentMap)
        {
            InternalCalls.SkyLightComponent_SetEnvironmentMap_Native(ParentEntity.ID, environmentMap.ID);
        }
    }
}
