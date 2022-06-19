using Kaitai;
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;

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
    public static Cement cmnt = null;
    public static TreeNode tvnode = null;
    public static string working_file = null;
    const string WINDOW_TITLE = "Scarface The World Is Yours: RCF Explorer";


    private static Panel panel1;
    private static MenuStrip menuStrip1;
    private static Panel panel2;
    private static SplitContainer splitContainer1;
    private static ToolStripMenuItem toolStripMenuItem1;
    private static TreeView treeView1 = null;

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
        toolStripMenuItem1 = new ToolStripMenuItem("&File");
        toolStripMenuItem1.DropDownItems.Add("Open RCF");
        toolStripMenuItem1.DropDownItems.Add("Exit");
        toolStripMenuItem1.DropDownItems[0].Click += Item_Click;
        toolStripMenuItem1.DropDownItems[1].Click += Item_Click;
        menuStrip1.Items.Add(toolStripMenuItem1);
        //
        // treeView1
        //
        treeView1 = new TreeView();
        treeView1.Dock = DockStyle.Fill;
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
            Debug.WriteLine(path);
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

    private static void Item_Click(object sender, EventArgs e)
    {
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
                    Debug.WriteLine(working_file);
                    form.Text = WINDOW_TITLE + " - File: " + openFileDialog.FileName;
                    cmnt = Cement.FromFile(openFileDialog.FileName);
                    MessageBox.Show("Count of files: "+cmnt.FilenameDirectory_.Count);
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

            default: 
                break;
        }
    }
}