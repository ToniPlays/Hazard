using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{
    [System.AttributeUsage(AttributeTargets.Class)]
    public class Cached : Attribute 
    {
    }

    public enum Status
    {
        Done = 0,
		Awaiting = 1,
		InProgress = 2,
		Bug = 3
	}

    [System.AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct, AllowMultiple = true)]
    public class Author : Attribute
    {
        public double Version;
        private string Name;

        public Author(string name) {
            this.Name = name;
            Version = 1.0;
        }
    }
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Field | AttributeTargets.Method)]
    public class Todo : Attribute
    {
        private string Detail;
        private int Progress;

        public Todo(string value, Status progress)
        {
            Detail = value;
            Progress = (int)progress;
        }
    }
    [AttributeUsage(AttributeTargets.Class)]
    //Menu items
    public class MenuBarItem : Attribute {
        private string Path;

        public MenuBarItem(string path) {
            Path = path;
        }
    }
}
