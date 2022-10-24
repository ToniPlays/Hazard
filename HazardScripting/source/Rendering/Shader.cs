using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard.Rendering
{
    public class Shader : Asset
    {
        public bool Set(string name, CubemapTexture cubemap)
        {
            return false;
        }
        public bool Set(string name, Image2D image)
        {
            return false;
        }
    }
}
