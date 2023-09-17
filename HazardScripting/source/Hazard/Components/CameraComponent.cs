using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class CameraComponent : Component
    {
        public CameraComponent() : base(0) { }
        internal CameraComponent(ulong ID) : base(ID) { }
        public float FOV
        {
            get
            {
                unsafe
                {
                    return InternalCalls.CameraComponent_GetFOV_Native(ParentEntity.ID);
                }
            }
            set
            {
                unsafe
                {
                    InternalCalls.CameraComponent_SetFOV_Native(ParentEntity.ID, value);
                }
            }
        }
        public float OrthographicSize
        {
            get
            {
                unsafe
                {
                    return InternalCalls.CameraComponent_GetOrthoSize_Native(ParentEntity.ID);
                }
            }
            set
            {
                unsafe
                {
                    InternalCalls.CameraComponent_SetOrthoSize_Native(ParentEntity.ID, value);
                }
            }
        }
        public ClippingPlane Clipping
        {
            get
            {
                unsafe
                {
                    Vector2 clip = InternalCalls.CameraComponent_GetClipping_Native(ParentEntity.ID);
                    return new ClippingPlane(clip.x, clip.y);
                }
            }
            set
            {
                unsafe
                {
                    InternalCalls.CameraComponent_SetClipping_Native(ParentEntity.ID, value.ZNear, value.ZFar);
                }
            }
        }
        public Projection GetProjection()
        {
            unsafe
            {
                return (Projection)InternalCalls.CameraComponent_GetProjection_Native(ParentEntity.ID);
            }
        }
        public void SetProjection(Projection projection)
        {
            unsafe
            {
                InternalCalls.CameraComponent_SetProjection_Native(ParentEntity.ID, (int)projection);
            }
        }
        public bool IsActive()
        {
            unsafe
            {
                return InternalCalls.Component_IsActive_Native(ParentEntity.ID, typeof(CameraComponent));
            }
        }
        public void SetActive(bool active)
        {
            unsafe
            {
                InternalCalls.Component_SetActive_Native(ParentEntity.ID, active, typeof(CameraComponent));
            }
        }
    }
}
