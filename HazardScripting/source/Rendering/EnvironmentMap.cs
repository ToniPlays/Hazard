using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Rendering
{
    public enum EnvironmentResource
    {
        RadianceMap = 0,
        IrradianceMap = 1,
        PreFilterMap = 2,
        BRDFLut = 3
    }

    public class EnvironmentMap : Asset
    {
        public EnvironmentMap() : base(0) { }
        internal EnvironmentMap(ulong ID) : base(ID) { }

        ~EnvironmentMap()
        {
            InternalCalls.EnvironmentMap_Destroy_Native(ID);
        }

        public CubemapTexture GetCubemapTexture(EnvironmentResource resource)
        {
            ulong id = InternalCalls.EnvironmentMap_GetCubemapTexture_Native(ID, (int)resource);
            if (id == 0) 
                return null;
            return new CubemapTexture(id);
        }
        public void SetCubemapTexture(EnvironmentResource resource, CubemapTexture cubemapTexture)
        {
            ulong id = cubemapTexture != null ? cubemapTexture.ID : 0;
            InternalCalls.EnvironmentMap_SetCubemapTexture_Native(ID, (int)resource, id);
        }

        public void GenerateRadiance(Sprite source)
        {
            InternalCalls.EnvironmentMap_GenerateRadiance_Native(ID, source.ID);
        }
        public void GenerateIrradiance(CubemapTexture source)
        {
            InternalCalls.EnvironmentMap_GenerateIrradiance_Native(ID, source.ID);
        }
        public void GeneratePrefilter(CubemapTexture source)
        {
            InternalCalls.EnvironmentMap_GeneratePrefilter_Native(ID, source.ID);
        }

        public static EnvironmentMap Create()
        {
            ulong id = InternalCalls.EnvironmentMap_Create_Native();
            return new EnvironmentMap(id);
        }
    }
}
