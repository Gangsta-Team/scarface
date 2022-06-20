using Kaitai;
using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

class IniFile
{
    string Path;
    string EXE = Assembly.GetExecutingAssembly().GetName().Name;

    [DllImport("kernel32", CharSet = CharSet.Unicode)]
    static extern long WritePrivateProfileString(string Section, string Key, string Value, string FilePath);

    [DllImport("kernel32", CharSet = CharSet.Unicode)]
    static extern int GetPrivateProfileString(string Section, string Key, string Default, StringBuilder RetVal, int Size, string FilePath);

    public IniFile(string IniPath = null)
    {
        Path = new FileInfo(IniPath ?? EXE + ".ini").FullName;
    }

    public string Read(string Key, string Section = null)
    {
        var RetVal = new StringBuilder(255);
        GetPrivateProfileString(Section ?? EXE, Key, "", RetVal, 255, Path);
        return RetVal.ToString();
    }

    public void Write(string Key, string Value, string Section = null)
    {
        WritePrivateProfileString(Section ?? EXE, Key, Value, Path);
    }

    public void DeleteKey(string Key, string Section = null)
    {
        Write(Key, null, Section ?? EXE);
    }

    public void DeleteSection(string Section = null)
    {
        Write(null, null, Section ?? EXE);
    }

    public bool KeyExists(string Key, string Section = null)
    {
        return Read(Key, Section).Length > 0;
    }
}

class Window
{
    [DllImport("kernel32.dll")]
    static extern IntPtr GetConsoleWindow();

    [DllImport("user32.dll")]
    static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

    const int SW_HIDE = 0;
    const int SW_SHOW = 5;

    const int WINDOW_W = 800;
    const int WINDOW_H = 600;

    public static Form form = null;
    public static Form settings_form = null;
    public static Cement cmnt = null;
    public static string working_file = null;
    const string WINDOW_TITLE = "Scarface The World Is Yours: RCF Explorer";
    public static IniFile MyIni = null;

    private static Panel panel1;
    private static MenuStrip menuStrip1;
    private static Panel panel2;
    private static SplitContainer splitContainer1;
    private static ToolStripMenuItem toolStripMenuItem1;
    private static ToolStripMenuItem toolStripMenuItem2;
    private static TreeView treeView1;
    private static TextBox textBox1;

    //
    // Settings form
    // 
    private static Button pySave;
    private static Button pyDetect;
    private static Label pyLabel;
    private static TextBox pyPath;

    enum TypeView : ushort
    {
        None = 0,
        Unknown = 1,
        TorqueScript = 2,
        Pure3D = 3,
        bik = 4,
        rsd = 5
    }

    private static void InitializeSettingsComponent()
    {
        settings_form = new Form();
        settings_form.Text = WINDOW_TITLE + " - Settings";

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
    }

    private static void InitializeComponent()
    {

        panel1 = new System.Windows.Forms.Panel();
        panel2 = new System.Windows.Forms.Panel();
        menuStrip1 = new System.Windows.Forms.MenuStrip();
        splitContainer1 = new System.Windows.Forms.SplitContainer();
        panel1.SuspendLayout();
        panel2.SuspendLayout();
        ((System.ComponentModel.ISupportInitialize)(splitContainer1)).BeginInit();
        splitContainer1.SuspendLayout();
        form.SuspendLayout();
        // 
        // panel1
        // 
        panel1.Controls.Add(menuStrip1);
        panel1.Dock = System.Windows.Forms.DockStyle.Top;
        panel1.Location = new System.Drawing.Point(0, 0);
        panel1.Name = "panel1";
        panel1.Size = new System.Drawing.Size(800, 24);
        panel1.TabIndex = 0;
        // 
        // panel2
        // 
        panel2.Controls.Add(splitContainer1);
        panel2.Dock = System.Windows.Forms.DockStyle.Fill;
        panel2.Location = new System.Drawing.Point(0, 24);
        panel2.Name = "panel2";
        panel2.Size = new System.Drawing.Size(800, 426);
        panel2.TabIndex = 1;
        // 
        // menuStrip1
        // 
        menuStrip1.Location = new System.Drawing.Point(0, 0);
        menuStrip1.Name = "menuStrip1";
        menuStrip1.Size = new System.Drawing.Size(800, 24);
        menuStrip1.TabIndex = 0;
        menuStrip1.Text = "menuStrip1";
        //
        // toolStripMenuItem1
        //
        toolStripMenuItem1 = new ToolStripMenuItem("File");
        toolStripMenuItem1.DropDownItems.Add("Open RCF");
        toolStripMenuItem1.DropDownItems.Add("Exit");
        toolStripMenuItem1.DropDownItems[0].Click += Item_Click;
        toolStripMenuItem1.DropDownItems[1].Click += Item_Click;
        menuStrip1.Items.Add(toolStripMenuItem1);
        //
        // menuItem2
        //
        toolStripMenuItem2 = new ToolStripMenuItem("Settings");
        toolStripMenuItem2.Click += Item_Click;
        menuStrip1.Items.Add(toolStripMenuItem2);
        //
        // treeView1
        //
        treeView1 = new TreeView();
        treeView1.Dock = DockStyle.Fill;
        treeView1.NodeMouseClick += new TreeNodeMouseClickEventHandler(TreeView_Click);
        splitContainer1.Panel1.Controls.Add(treeView1);
        // 
        // splitContainer1
        // 
        splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
        splitContainer1.Location = new System.Drawing.Point(0, 0);
        splitContainer1.Name = "splitContainer1";
        splitContainer1.Size = new System.Drawing.Size(800, 426);
        splitContainer1.SplitterDistance = 266;
        splitContainer1.TabIndex = 0;
        //
        // textBox1
        //
        textBox1 = new TextBox();
        textBox1.Dock = DockStyle.Fill;
        textBox1.Visible = false;
        textBox1.Multiline = true;
        splitContainer1.Panel2.Controls.Add(textBox1);
        // 
        // Form1
        // 
        form.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
        form.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        form.ClientSize = new System.Drawing.Size(800, 450);
        form.Controls.Add(panel2);
        form.Controls.Add(panel1);
        form.MainMenuStrip = menuStrip1;
        form.Name = "Form1";
        form.Text = "Form1";
        panel1.ResumeLayout(false);
        panel1.PerformLayout();
        panel2.ResumeLayout(false);
        ((System.ComponentModel.ISupportInitialize)(splitContainer1)).EndInit();
        splitContainer1.ResumeLayout(false);
        form.ResumeLayout(false);

        MyIni = new IniFile("Settings.ini");

        InitializeSettingsComponent();

        // Read settings
        var myPath = MyIni.Read("PythonPath");
        if (myPath != "")
            pyPath.Text = MyIni.Read("PythonPath").ToString();

    }

    [STAThread]
    static void Main(string[] args)
    {
        if (args.Length > 0)
        {
            Console.WriteLine("[-] Scarface The World Is Yours [-]");
            // Display the number of command line arguments.
            Console.WriteLine(args.Length);
            Console.ReadKey();
        }
        else {
            var handle = GetConsoleWindow();

            // Hide
            ShowWindow(handle, SW_HIDE);

            Application.EnableVisualStyles();
            form = new Form();
            form.Text = WINDOW_TITLE;
            form.Width = WINDOW_W;
            form.Height = WINDOW_H;

            InitializeComponent();

            Application.Run(form);

        }
    }

    public static TreeNode PopulateTreeNode(string[] paths, string pathSeparator)
    {
        if (paths == null)
            return null;

        TreeNode thisnode = new TreeNode();
        TreeNode currentnode;
        char[] cachedpathseparator = pathSeparator.ToCharArray();
        foreach (string path in paths)
        {
            currentnode = thisnode;
            //currentnode.FullPath = path;
            string name = Path.GetFileName(path);
            foreach (string subPath in path.Split(cachedpathseparator))
            {
                int image_index = 0;
                if (null == currentnode.Nodes[subPath])
                {
                    if (name == subPath)
                    {
                        if (subPath.Contains(".rcf")) image_index = 7;
                        if (subPath.Contains(".cso")) image_index = 2;
                        if (subPath.Contains(".dso")) image_index = 2;
                        if (subPath.EndsWith(".bik")) image_index = 6;
                        if (subPath.EndsWith(".p3d")) image_index = 8;
                    }
                    else
                    {
                        image_index = 1;
                    }
                    currentnode = currentnode.Nodes.Add(subPath, subPath);
                }
                else
                    currentnode = currentnode.Nodes[subPath];

            }
        }

        if (thisnode.Text == "")
        {
            string filename = Path.GetFileName(working_file);
            thisnode.Text = filename;
            thisnode.ImageIndex = 7;
            thisnode.SelectedImageIndex = 7;
        }

        return thisnode;
    }

    private static void ToggleView(TypeView type)
    {
        textBox1.Visible = false;
        switch (type)
        {
            case TypeView.None: break;
            case TypeView.Pure3D: break;
            case TypeView.TorqueScript:
                textBox1.Visible = true;
                break;
            case TypeView.bik: break;
            default: break;
        }
    }

    private static byte[] GetFileContentFromPath(string file_path)
    {

        for (int i = 0; i < cmnt.FilenameDirectory_.Count; i++)
        {
            if (cmnt.FilenameDirectory_[i].Path == file_path)
            {
                return cmnt.Directory[i].File;
            }
        }
        return null;
    }

    private static void TreeView_Click(object sender, TreeNodeMouseClickEventArgs e)
    {
        string ext = null;
        ext = Path.GetExtension(e.Node.Text).Replace(".", "");
        if (ext.Length > 0)
        {
            switch (ext)
            {
                case "cso":
                    {
                        ToggleView(TypeView.TorqueScript);
                        Debug.WriteLine(e.Node.Text);
                        byte[] fcontent = GetFileContentFromPath(e.Node.Text);
                        Debug.WriteLine(fcontent);
                        //textBox1.Text = Encoding.ASCII.GetString(fcontent).Trim();
                    } break;
                case "p3d": ToggleView(TypeView.Pure3D); break;
                case "bik": ToggleView(TypeView.bik); break;
                case "rsd": ToggleView(TypeView.rsd); break;
                default: break;
            }
        }
    }

    private static void Item_Click(object sender, EventArgs e)
    {
        Debug.WriteLine("Item click" + sender.ToString());
        switch (sender.ToString())
        {
            case "Open RCF":
                OpenFileDialog openFileDialog = new OpenFileDialog();
                openFileDialog.InitialDirectory = @"C:\";
                openFileDialog.Title = "Browse RCF Files";
                openFileDialog.DefaultExt = "rcf";
                openFileDialog.Filter = "rcf files (*.rcf)|*.rcf|All files (*.*)|*.*";
                openFileDialog.CheckFileExists = true;
                openFileDialog.CheckPathExists = true;
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    working_file = Path.GetFileNameWithoutExtension(openFileDialog.FileName);
                    form.Text = WINDOW_TITLE + " - File: " + openFileDialog.FileName;
                    cmnt = Cement.FromFile(openFileDialog.FileName);
                    string[] paths = new string[cmnt.FilenameDirectory_.Count];
                    for (int i = 0; i < cmnt.FilenameDirectory_.Count; i++)
                    {
                        paths[i] = cmnt.FilenameDirectory_[i].Path;
                    }
                    TreeNode tn = PopulateTreeNode(paths, "\\");
                    treeView1.Nodes.Add(tn);
                }
                break;
            case "Exit":
                Application.Exit();
                break;
            case "Settings":
                settings_form.ShowDialog();
                break;

            default:
                break;
        }
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
            MyIni.Write("PythonPath", pyPath.Text);
        }
    }
}