using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Events
{
    public interface IApplicationEvent
    {
        void OnApplicationOpen();
        bool OnApplicationWantsClose();
        void OnApplicationClose();
        void OnApplicationMinimized();
        void OnApplicationMaximized();
    }
}
