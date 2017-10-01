using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace BlacklistManager
{
    public partial class MainForm : Form
    {
        BindingList<BlacklistEntry> blacklistEntries = new BindingList<BlacklistEntry>();

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(Settings.Default.BlacklistFilePath)) {
                throw new Exception("Blacklist file path was not set");
            }

            string fileContent = File.ReadAllText(Settings.Default.BlacklistFilePath, Encoding.UTF8);
            List<string> serialList = JsonConvert.DeserializeObject<List<string>>(fileContent);

            foreach (var serial in serialList) {
                BlacklistEntry entry = new BlacklistEntry() { Serial = serial };
                if (blacklistEntries.Contains(entry)) {
                    continue;
                }

                blacklistEntries.Add(entry);
            }

            BlacklistView.DataSource = blacklistEntries;

            BlacklistView.Columns["Serial"].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;

            blacklistEntries.ListChanged += OnListChangedEventHandler;
        }

        private void BlacklistButton_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(SerialText.Text)) {
                MessageBox.Show("Invalid serial");
                return;
            }

            string transformed = SerialText.Text.ToUpperInvariant();
            bool parsed = Guid.TryParse(transformed, out Guid result);
            if (!parsed) {
                MessageBox.Show("Invalid serial");
                return;
            }

            BlacklistEntry entry = new BlacklistEntry() { Serial = transformed };
            if (blacklistEntries.Contains(entry)) {
                MessageBox.Show("Already in list");
                return;
            }

            blacklistEntries.Add(entry);
        }

        private void WriteBlacklist()
        {
            List<string> serialList = new List<string>();
            foreach (var entry in blacklistEntries) {
                serialList.Add(entry.Serial);
            }

            string fileContent = JsonConvert.SerializeObject(serialList, Formatting.Indented);
            File.WriteAllText(Settings.Default.BlacklistFilePath, fileContent, Encoding.UTF8);
        }

        void OnListChangedEventHandler(object sender, ListChangedEventArgs e)
        {
            WriteBlacklist();
        }
    }
}
