namespace BlacklistManager
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
            this.BlacklistButton = new System.Windows.Forms.Button();
            this.SerialText = new System.Windows.Forms.TextBox();
            this.BlacklistView = new System.Windows.Forms.DataGridView();
            ((System.ComponentModel.ISupportInitialize)(this.BlacklistView)).BeginInit();
            this.SuspendLayout();
            // 
            // BlacklistButton
            // 
            this.BlacklistButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.BlacklistButton.Location = new System.Drawing.Point(228, 12);
            this.BlacklistButton.Name = "BlacklistButton";
            this.BlacklistButton.Size = new System.Drawing.Size(75, 23);
            this.BlacklistButton.TabIndex = 0;
            this.BlacklistButton.Text = "Blacklist";
            this.BlacklistButton.UseVisualStyleBackColor = true;
            this.BlacklistButton.Click += new System.EventHandler(this.BlacklistButton_Click);
            // 
            // SerialText
            // 
            this.SerialText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SerialText.Location = new System.Drawing.Point(12, 14);
            this.SerialText.Name = "SerialText";
            this.SerialText.Size = new System.Drawing.Size(210, 20);
            this.SerialText.TabIndex = 1;
            // 
            // BlacklistView
            // 
            this.BlacklistView.AllowUserToAddRows = false;
            this.BlacklistView.AllowUserToOrderColumns = true;
            this.BlacklistView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.BlacklistView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.BlacklistView.Location = new System.Drawing.Point(13, 41);
            this.BlacklistView.Name = "BlacklistView";
            this.BlacklistView.ReadOnly = true;
            this.BlacklistView.Size = new System.Drawing.Size(290, 377);
            this.BlacklistView.TabIndex = 2;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(315, 430);
            this.Controls.Add(this.BlacklistView);
            this.Controls.Add(this.SerialText);
            this.Controls.Add(this.BlacklistButton);
            this.Name = "MainForm";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.MainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.BlacklistView)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button BlacklistButton;
        private System.Windows.Forms.TextBox SerialText;
        private System.Windows.Forms.DataGridView BlacklistView;
    }
}

