namespace BlacklistManager
{
    internal class BlacklistEntry
    {
        public string Serial { get; set; }

        public override bool Equals(object obj)
        {
            var entry = obj as BlacklistEntry;
            if (entry == null) {
                return base.Equals(obj);
            }

            return (Serial == entry.Serial);
        }

        public override int GetHashCode()
        {
            return Serial.GetHashCode();
        }

        public override string ToString()
        {
            return Serial;
        }
    }
}
