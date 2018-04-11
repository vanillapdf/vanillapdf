namespace LicenseGenerator
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.GenerateButton = new System.Windows.Forms.Button();
            this.OwnerText = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.NoteText = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.SerialText = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.VersionText = new System.Windows.Forms.TextBox();
            this.OEMCheck = new System.Windows.Forms.CheckBox();
            this.label3 = new System.Windows.Forms.Label();
            this.UpdatesYears = new System.Windows.Forms.NumericUpDown();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.TemporaryDays = new System.Windows.Forms.NumericUpDown();
            this.label9 = new System.Windows.Forms.Label();
            this.TemporaryCheckbox = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.UpdatesYears)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.TemporaryDays)).BeginInit();
            this.SuspendLayout();
            // 
            // GenerateButton
            // 
            this.GenerateButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.GenerateButton.Location = new System.Drawing.Point(592, 474);
            this.GenerateButton.Name = "GenerateButton";
            this.GenerateButton.Size = new System.Drawing.Size(75, 23);
            this.GenerateButton.TabIndex = 0;
            this.GenerateButton.Text = "Generate";
            this.GenerateButton.UseVisualStyleBackColor = true;
            this.GenerateButton.Click += new System.EventHandler(this.button1_Click);
            // 
            // OwnerText
            // 
            this.OwnerText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.OwnerText.Location = new System.Drawing.Point(89, 12);
            this.OwnerText.Name = "OwnerText";
            this.OwnerText.Size = new System.Drawing.Size(578, 20);
            this.OwnerText.TabIndex = 1;
            this.OwnerText.Text = "Sample company";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Owner";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 41);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(30, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Note";
            // 
            // NoteText
            // 
            this.NoteText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.NoteText.Location = new System.Drawing.Point(89, 38);
            this.NoteText.Name = "NoteText";
            this.NoteText.Size = new System.Drawing.Size(578, 20);
            this.NoteText.TabIndex = 3;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 93);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Updates";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 157);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(33, 13);
            this.label5.TabIndex = 10;
            this.label5.Text = "Serial";
            // 
            // SerialText
            // 
            this.SerialText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SerialText.Enabled = false;
            this.SerialText.Location = new System.Drawing.Point(89, 154);
            this.SerialText.Name = "SerialText";
            this.SerialText.Size = new System.Drawing.Size(578, 20);
            this.SerialText.TabIndex = 9;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(12, 183);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(42, 13);
            this.label6.TabIndex = 12;
            this.label6.Text = "Version";
            // 
            // VersionText
            // 
            this.VersionText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.VersionText.Enabled = false;
            this.VersionText.Location = new System.Drawing.Point(89, 180);
            this.VersionText.Name = "VersionText";
            this.VersionText.Size = new System.Drawing.Size(578, 20);
            this.VersionText.TabIndex = 11;
            // 
            // OEMCheck
            // 
            this.OEMCheck.AutoSize = true;
            this.OEMCheck.Location = new System.Drawing.Point(89, 65);
            this.OEMCheck.Name = "OEMCheck";
            this.OEMCheck.Size = new System.Drawing.Size(15, 14);
            this.OEMCheck.TabIndex = 13;
            this.OEMCheck.UseVisualStyleBackColor = true;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 65);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(61, 13);
            this.label3.TabIndex = 14;
            this.label3.Text = "Commercial";
            // 
            // UpdatesYears
            // 
            this.UpdatesYears.Location = new System.Drawing.Point(89, 91);
            this.UpdatesYears.Name = "UpdatesYears";
            this.UpdatesYears.Size = new System.Drawing.Size(61, 20);
            this.UpdatesYears.TabIndex = 15;
            this.UpdatesYears.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(156, 93);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(32, 13);
            this.label7.TabIndex = 16;
            this.label7.Text = "years";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(177, 122);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(29, 13);
            this.label8.TabIndex = 19;
            this.label8.Text = "days";
            // 
            // TemporaryDays
            // 
            this.TemporaryDays.Enabled = false;
            this.TemporaryDays.Location = new System.Drawing.Point(110, 120);
            this.TemporaryDays.Name = "TemporaryDays";
            this.TemporaryDays.Size = new System.Drawing.Size(61, 20);
            this.TemporaryDays.TabIndex = 18;
            this.TemporaryDays.Value = new decimal(new int[] {
            30,
            0,
            0,
            0});
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(12, 123);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(57, 13);
            this.label9.TabIndex = 17;
            this.label9.Text = "Temporary";
            // 
            // TemporaryCheckbox
            // 
            this.TemporaryCheckbox.AutoSize = true;
            this.TemporaryCheckbox.Location = new System.Drawing.Point(89, 124);
            this.TemporaryCheckbox.Name = "TemporaryCheckbox";
            this.TemporaryCheckbox.Size = new System.Drawing.Size(15, 14);
            this.TemporaryCheckbox.TabIndex = 20;
            this.TemporaryCheckbox.UseVisualStyleBackColor = true;
            this.TemporaryCheckbox.CheckedChanged += new System.EventHandler(this.TemporaryCheckbox_CheckedChanged);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(679, 509);
            this.Controls.Add(this.TemporaryCheckbox);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.TemporaryDays);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.UpdatesYears);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.OEMCheck);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.VersionText);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.SerialText);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.NoteText);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.OwnerText);
            this.Controls.Add(this.GenerateButton);
            this.Name = "MainForm";
            this.Text = "MainForm";
            this.Load += new System.EventHandler(this.MainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.UpdatesYears)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.TemporaryDays)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button GenerateButton;
        private System.Windows.Forms.TextBox OwnerText;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox NoteText;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox SerialText;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox VersionText;
        private System.Windows.Forms.CheckBox OEMCheck;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown UpdatesYears;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.NumericUpDown TemporaryDays;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.CheckBox TemporaryCheckbox;
    }
}

