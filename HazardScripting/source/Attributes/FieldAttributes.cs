using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{
    [AttributeUsage(AttributeTargets.Field)]
    public class ShowInProperties : Attribute
    {
        public ShowInProperties()
        {
        }
    }
    [AttributeUsage(AttributeTargets.Field)]
    public class HideInProperties : Attribute
    {
        public HideInProperties()
        {
        }
    }
    [AttributeUsage(AttributeTargets.Field)]
    public class SerializeField : Attribute
    {
        public SerializeField()
        {
        }
    }
    [AttributeUsage(AttributeTargets.Field)]
    public class Slider : Attribute 
    {
        float Min, Max;
        public Slider(float min, float max) {
            Min = min;
            Max = max;
        }
    }
    [AttributeUsage(AttributeTargets.Field)]
    public class Range : Attribute
    {

    }
    [AttributeUsage(AttributeTargets.Field)]
    public class Header : Attribute
    {

    }
    [AttributeUsage(AttributeTargets.Field)]
    public class Tooltip : Attribute
    {

    }
    [AttributeUsage(AttributeTargets.Field)]
    public class TextArea : Attribute
    {

    }
}
