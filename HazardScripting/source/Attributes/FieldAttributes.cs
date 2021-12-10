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
        float min, max;
        public Slider(float min, float max) {
            this.min = min;
            this.max = max;
        }
    }
    [AttributeUsage(AttributeTargets.Field)]
    public class Range : Attribute
    {
        float min, max;
        public Range(float min, float max) {
            this.min = min;
            this.max = max;
        }
    }
    [AttributeUsage(AttributeTargets.Field)]
    public class Header : Attribute
    {
        string header;
        public Header(string header) {
            this.header = header;
        }
    }
    [AttributeUsage(AttributeTargets.Field)]
    public class Tooltip : Attribute
    {
        string tooltip;
        public Tooltip(string tooltip) {
            this.tooltip = tooltip;
        }
    }
    [AttributeUsage(AttributeTargets.Field)]
    public class TextArea : Attribute
    {

    }
}
