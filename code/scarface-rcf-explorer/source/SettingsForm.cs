
using System;
using System.Windows.Forms;

class SettingsForm
{
    //
    // Settings form
    // 
    public static Button pySave;
    public static Button pyDetect;
    public static Label pyLabel;
    public static TextBox pyPath;
    private static Form settings_form;

    public static void InitializeSettingsComponent()
    {
        settings_form = new Form();
        settings_form.Text = Window.WINDOW_TITLE + " - Settings";

        pySave = new System.Windows.Forms.Button();
        pyDetect = new System.Windows.Forms.Button();
        pyLabel = new System.Windows.Forms.Label();
        pyPath = new System.Windows.Forms.TextBox();
        settings_form.SuspendLayout();
        // 
        // button1
        // 
        pySave.Location = new System.Drawing.Point(475, 23);
        pySave.Name = "button1";
        pySave.Size = new System.Drawing.Size(75, 23);
        pySave.TabIndex = 0;
        pySave.Text = "Save";
        pySave.UseVisualStyleBackColor = true;
        pySave.Click += Settings_CloseButton_Click;
        // 
        // button2
        // 
        pyDetect.Location = new System.Drawing.Point(394, 23);
        pyDetect.Name = "button2";
        pyDetect.Size = new System.Drawing.Size(75, 23);
        pyDetect.TabIndex = 1;
        pyDetect.Text = "Find";
        pyDetect.UseVisualStyleBackColor = true;
        pyDetect.Click += Settings_FindButton_Click;
        // 
        // label1
        // 
        pyLabel.AutoSize = true;
        pyLabel.Location = new System.Drawing.Point(12, 9);
        pyLabel.Name = "label1";
        pyLabel.Size = new System.Drawing.Size(67, 13);
        pyLabel.TabIndex = 2;
        pyLabel.Text = "Python path:";
        // 
        // pyPath
        // 
        pyPath.Location = new System.Drawing.Point(12, 25);
        pyPath.Name = "textBox1";
        pyPath.Size = new System.Drawing.Size(376, 20);
        pyPath.TabIndex = 3;
        // 
        // Settings
        // 
        settings_form.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
        settings_form.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        settings_form.ClientSize = new System.Drawing.Size(561, 59);
        settings_form.Controls.Add(pyPath);
        settings_form.Controls.Add(pyLabel);
        settings_form.Controls.Add(pySave);
        settings_form.Controls.Add(pyDetect);
        settings_form.Name = "Settings";
        settings_form.Text = "Settings";
        settings_form.ResumeLayout(false);
        settings_form.PerformLayout();

        // Read settings
        var myPath = Window.ini.Read("PythonPath");
        if (myPath != "")
            pyPath.Text = Window.ini.Read("PythonPath").ToString();
    }

    private static void Settings_CloseButton_Click(object sender, EventArgs e)
    {
        pyPath.ReadOnly = true;
        settings_form.Close();
    }

    private static void Settings_FindButton_Click(object sender, EventArgs e)
    {
        OpenFileDialog openFileDialog = new OpenFileDialog();
        openFileDialog.InitialDirectory = @"C:\";
        openFileDialog.Title = "Browse Python executable";
        openFileDialog.DefaultExt = "exe";
        openFileDialog.Filter = "exe file (*.exe)|*.exe|All files (*.*)|*.*";
        openFileDialog.CheckFileExists = true;
        openFileDialog.CheckPathExists = true;
        if (openFileDialog.ShowDialog() == DialogResult.OK)
        {
            pyPath.ReadOnly = false;
            pyPath.Text = openFileDialog.FileName;
            pySave.Text = "Save";
            Window.ini.Write("PythonPath", pyPath.Text);
        }
    }

    public static Form GetForm()
    {
        return settings_form;
    }
}