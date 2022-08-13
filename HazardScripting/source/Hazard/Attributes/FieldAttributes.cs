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
        float Min, Max;
        public Range(float min, float max) {
            Min = min;
            Max = max;
        }
    }
    [AttributeUsage(AttributeTargets.Field)]
    public class Header : Attribute
    {
        string Text;
        public Header(string header) {
            Text = header;
        }
    }
    [AttributeUsage(AttributeTargets.Field)]
    public class Tooltip : Attribute
    {
        string Text;
        public Tooltip(string tooltip) {
            Text = tooltip;
        }
    }
    [AttributeUsage(AttributeTargets.Field)]
    public class TextArea : Attribute
    {
        float Min, Max;
    }
}
