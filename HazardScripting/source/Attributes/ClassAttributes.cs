﻿using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{

    public enum Progress { Done = 0, Awaiting = 1, InProgress = 2, Bug = 3 };

    [System.AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct, AllowMultiple = true)]
    public class Author : System.Attribute
    {
        public double Version;
        private string Name;

        public Author(string name) {
            this.Name = name;
            Version = 1.0;
        }
    }
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Field)]
    public class Todo : Attribute
    {
        private string Detail;
        private int Progress;

        public Todo(string value, Progress progress)
        {
            Detail = value;
            Progress = (int)progress;
        }
    }
}
