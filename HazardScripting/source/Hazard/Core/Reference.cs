using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class Reference : IEquatable<Reference>
    {
        protected Reference() { ID = 0; }
        internal Reference(ulong ID)
        {
            this.ID = ID;
        }
        ~Reference() {}

        public ulong ID { get; protected set; }

        public override bool Equals(object obj) => obj is Reference other && Equals(other);

        // NOTE(Peter): Implemented according to Microsofts official documentation:
        // https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/statements-expressions-operators/how-to-define-value-equality-for-a-type
        public bool Equals(Reference other)
        {
            if (other is null)
                return false;

            if (ReferenceEquals(this, other))
                return true;

            return ID == other.ID;
        }

        public override int GetHashCode() => (int)ID;

        public static bool operator == (Reference r1, Entity r2) => r1 is null ? r2 is null : r1.Equals(r2);
        public static bool operator != (Reference r1, Entity r2) => !(r1 == r2);

        public static implicit operator bool(Reference entity)
        {
            try { return entity.ID != 0; }
            catch { return false; }
        }
    }
}
