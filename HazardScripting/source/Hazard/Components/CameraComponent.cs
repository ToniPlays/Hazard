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
            get => InternalCalls.CameraComponent_GetFOV_Native(ParentEntity.ID);
            set => InternalCalls.CameraComponent_SetFOV_Native(ParentEntity.ID, value);
        }
        public float OrthographicSize
        {
            get => InternalCalls.CameraComponent_GetOrthoSize_Native(ParentEntity.ID);
            set => InternalCalls.CameraComponent_SetOrthoSize_Native(ParentEntity.ID, value);
        }
        public ClippingPlane Clipping
        {
            get
            {
                InternalCalls.CameraComponent_GetClipping_Native(ParentEntity.ID, out Vector2 clip);
                return new ClippingPlane(clip.x, clip.y);
            }
            set => InternalCalls.CameraComponent_SetClipping_Native(ParentEntity.ID, value.ZNear, value.ZFar);
        }
        public Projection GetProjection() { return (Projection)InternalCalls.CameraComponent_GetProjection_Native(ParentEntity.ID); }
        public void SetProjection(Projection projection) { InternalCalls.CameraComponent_SetProjection_Native(ParentEntity.ID, (int)projection); }
        public bool IsActive() { return InternalCalls.Component_IsActive_Native(ParentEntity.ID, typeof(CameraComponent)); }
        public void SetActive(bool active) { InternalCalls.Component_SetActive_Native(ParentEntity.ID, active, typeof(CameraComponent)); }
    }
}
