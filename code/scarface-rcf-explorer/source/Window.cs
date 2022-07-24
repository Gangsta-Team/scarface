using Kaitai;
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using System.ComponentModel.Design;
using ProcessWithOutput;
using System.Collections.Generic;
using System.Drawing;
using System.Text.RegularExpressions;
using System.ComponentModel;
using System.Reflection;
using System.Linq;

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

    private static Control window_control;
    public static Form form = null;
    public static Cement cmnt = null;
    public static string working_file = null;
    public static string file_path = null;
    public const string WINDOW_TITLE = "Scarface The World Is Yours: RCF Explorer";
    public const string TEMP_PATH = "./temp";
    public static IniFile ini = null;

    public static string[] paths;
    public static List<string> opened_files = new List<string>();

    private static Panel panel1;
    private static MenuStrip menuStrip1;
    private static Panel panel2;
    private static SplitContainer splitContainer1;
    private static SplitContainer splitContainer2;
    private static ToolStripMenuItem toolStripMenuItem1;
    private static ToolStripMenuItem toolStripMenuItem2;
    private static TreeView treeView1;
    private static ByteViewer byteviewer;
    private static TabControl tabControl1;
    private static string decompile_py = null;
    private static string decompile_path = null;
    private static TreeNode working_node = null;
    private static RichTextBox working_rtb = null;
    //private TabPage tabPage1;

    private static Form settings_form;

    enum TypeView : ushort
    {
        None = 0,
        Unknown = 1,
        TorqueScript = 2,
        Pure3D = 3,
        bik = 4,
        rsd = 5
    }

    private static void InitializeComponent()
    {

        panel1 = new System.Windows.Forms.Panel();
        panel2 = new System.Windows.Forms.Panel();
        menuStrip1 = new System.Windows.Forms.MenuStrip();
        splitContainer1 = new System.Windows.Forms.SplitContainer();
        splitContainer2 = new System.Windows.Forms.SplitContainer();
        tabControl1 = new System.Windows.Forms.TabControl();
        //hexBox1 = new HexBox();
        panel1.SuspendLayout();
        panel2.SuspendLayout();
        ((System.ComponentModel.ISupportInitialize)(splitContainer1)).BeginInit();
        ((System.ComponentModel.ISupportInitialize)(splitContainer2)).BeginInit();
        splitContainer1.SuspendLayout();
        splitContainer2.SuspendLayout(); 
        tabControl1.SuspendLayout();
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
        // splitContainer2
        //
        splitContainer2.Name = "splitContainer2";
        splitContainer2.Dock = DockStyle.Fill;
        splitContainer2.TabIndex = 0;
        splitContainer2.Orientation = Orientation.Horizontal;
        splitContainer2.SplitterDistance = 266;
        splitContainer2.Panel2.ControlAdded += new ControlEventHandler(splitContainer2_Panel2_ControlAdded);
        //
        // ByteViewer
        //
        byteviewer = new ByteViewer();
        byteviewer.Dock = DockStyle.Fill;
        byteviewer.SetBytes(new byte[] { });
        splitContainer2.Panel2.Controls.Add(byteviewer);
        // 
        // tabControl1
        // 
        tabControl1.Location = new System.Drawing.Point(117, 41);
        tabControl1.Name = "tabControl1";
        tabControl1.SelectedIndex = 0;
        tabControl1.Dock = DockStyle.Fill;
        tabControl1.TabIndex = 0;
        splitContainer2.Panel1.Controls.Add(tabControl1);
        splitContainer1.Panel2.Controls.Add(splitContainer2);
        // 
        // Form1
        // 
        form.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
        form.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        form.ClientSize = new System.Drawing.Size(900, 600);
        form.Controls.Add(panel2);
        form.Controls.Add(panel1);
        form.MainMenuStrip = menuStrip1;
        form.Name = "Form1";
        form.Text = "Form1";
        form.Load += new EventHandler(Window_Load);
        form.FormClosing += new FormClosingEventHandler(Window_FormClosing);
        panel1.ResumeLayout(false);
        panel1.PerformLayout();
        panel2.ResumeLayout(false);
        ((System.ComponentModel.ISupportInitialize)(splitContainer1)).EndInit();
        ((System.ComponentModel.ISupportInitialize)(splitContainer2)).EndInit();
        splitContainer1.ResumeLayout(false);
        splitContainer2.ResumeLayout(false);
        form.ResumeLayout(false);

        window_control = form;

        ini = new IniFile("Settings.ini");

        SettingsForm.InitializeSettingsComponent();
        settings_form = SettingsForm.GetForm();

        Directory.CreateDirectory(TEMP_PATH);

        Debug.WriteLine(ReadResource("decompiler.py"));
        decompile_py = ReadResource("decompiler.py");
        if(decompile_py != null)
        {
            decompile_path = TEMP_PATH + "/decompiler.py";
            File.WriteAllText(decompile_path, decompile_py);
        }
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
        //textBox1.Visible = false;
        switch (type)
        {
            case TypeView.None: break;
            case TypeView.Pure3D: break;
            case TypeView.TorqueScript:
                //textBox1.Visible = true;
                break;
            case TypeView.bik: break;
            default: break;
        }
    }

    private static Cement.RcfDirectory GetFileDirectoryFromPath(string file_path)
    {

        for (int i = 0; i < paths.Length; i++)
        {
            if (working_file + "\\" + cmnt.FilenameDirectory_[i].Path == file_path)
            {
                return cmnt.Directory[i];
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
                        Cement.RcfDirectory fcontent = GetFileDirectoryFromPath(e.Node.FullPath);
                        ToggleView(TypeView.TorqueScript);

                        if (fcontent.File.Length > 0)
                        {
                            file_path = TEMP_PATH + "/" + e.Node.Name;
                            using (FileStream fs = File.Create(file_path))
                            {
                                fs.Write(fcontent.File, 0, (int)fcontent.FlSize);
                            }
                            byteviewer.SetBytes(fcontent.File);

                            if (!opened_files.Contains(e.Node.FullPath))
                            {
                                var myPath = Window.ini.Read("PythonPath");
                                string pypath = (myPath != "") ? Window.ini.Read("PythonPath").ToString() : "python";

                                working_node = e.Node;
                                CreateTabPage(e.Node.Name);

                                var process = new ProcessWrapper(pypath, decompile_path + " " + file_path);

                                process.OutputDataReceived += (sendr, eventArgs) => {
                                    Debug.WriteLine($"Received: {eventArgs.Data}");
                                    if (eventArgs.Data != null)
                                    {
                                        if (eventArgs.Data.Contains("Successfully decompiled 1 out of 1 input files"))
                                        {
                                            Debug.WriteLine("Finished!");
                                            LoadDecompiledFile();
                                        }
                                    }
                                };
                                process.Start();
                                SelectTabByName(e.Node.Name);
                            }

                            SelectTabByName(e.Node.Name);
                        }

                    } break;
                case "p3d": ToggleView(TypeView.Pure3D); break;
                case "bik": ToggleView(TypeView.bik); break;
                case "rsd": ToggleView(TypeView.rsd); break;
                default: break;
            }
        }
    }

    private static void LoadDecompiledFile()
    {
        opened_files.Add(working_node.FullPath);
        using (StreamReader sr = File.OpenText(file_path+".cs"))
        {
            string s = "";
            string file_content = "";
            while ((s = sr.ReadLine()) != null)
            {
                file_content += s + "\n";
            }
            AppendTextBox(file_content);
            
        }
    }

    public static void AppendTextBox(string value)
    {
        if (form.InvokeRequired)
        {
            form.Invoke(new Action<string>(AppendTextBox), new object[] { value });
            return;
        }

        Regex r = new Regex("\\n");
        String[] lines = r.Split(value);

        foreach (string l in lines)
        {
            ParseLine(l, working_rtb);
        }

        ParseStrings(working_rtb);
        ParseComments(working_rtb);

    }

    public static void CreateTabPage(string file_name)
    {
        TabPage tp = new TabPage();
        tp.Name = file_name;
        tp.Padding = new System.Windows.Forms.Padding(3);
        tp.TabIndex = 0;
        tp.Text = file_name;
        tp.UseVisualStyleBackColor = true;
        tabControl1.Controls.Add(tp);

        RichTextBox m_rtb = new RichTextBox();
        m_rtb = new RichTextBox();
        m_rtb.Multiline = true;
        m_rtb.WordWrap = false;
        m_rtb.Name = file_name;
        m_rtb.AcceptsTab = true;
        m_rtb.ScrollBars = RichTextBoxScrollBars.ForcedBoth;
        m_rtb.Dock = DockStyle.Fill;
        m_rtb.SelectionFont = new Font("Courier New", 10, FontStyle.Regular);
        m_rtb.SelectionColor = Color.Black;
        working_rtb = m_rtb;

        tp.Controls.Add(m_rtb);
    }

    private static void ParseLine(string line, RichTextBox m_rtb)
    {
        Regex r = new Regex("([ \\t{}():;])");
        String[] tokens = r.Split(line);
        foreach (string token in tokens)
        {
            // Set the tokens default color and font.  
            m_rtb.SelectionColor = Color.Black;
            m_rtb.SelectionFont = new Font("Courier New", 10, FontStyle.Regular);
            // Check whether the token is a keyword.   
            String[] keywords = { "while", "else", "function", "if", "new", "return" };
            for (int i = 0; i < keywords.Length; i++)
            {
                if (keywords[i] == token)
                {
                    // Apply alternative color and font to highlight keyword.  
                    m_rtb.SelectionColor = Color.Blue;
                    m_rtb.SelectionFont = new Font("Courier New", 10, FontStyle.Bold);
                    break;
                }
            }
            m_rtb.SelectedText = token;
        }
        m_rtb.SelectedText = "\n";
    }

    private static void ParseStrings(RichTextBox m_rtb)
    {

        //Format strings
        string pattern_strings = @"(\"".*?\"")";
        var matches = Regex.Matches(m_rtb.Text, pattern_strings, RegexOptions.IgnoreCase);
        foreach (Match m in matches)
        {
            m_rtb.Select(m.Index, m.Length);
            m_rtb.SelectionColor = Color.DarkOrange;
        }
    }

    private static void ParseComments(RichTextBox m_rtb)
    {

        //Format strings
        string pattern_strings = @"(/\*([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*\*+/)|(//.*)";
        var matches = Regex.Matches(m_rtb.Text, pattern_strings, RegexOptions.IgnoreCase);
        foreach (Match m in matches)
        {
            m_rtb.Select(m.Index, m.Length);
            m_rtb.SelectionColor = Color.Green;
        }
    }

    private static void SelectTabByName(string name)
    {
        foreach (TabPage t in tabControl1.TabPages)
        {
            if (t.Name.Equals(name))
            {
                tabControl1.SelectedTab = t;
                break;
            }
        }
    }

    private static void Window_Load(object sender, EventArgs e)
    {
        splitContainer2.SplitterDistance = 266;
    }

    private static void Window_FormClosing(object sender, FormClosingEventArgs e)
    {
        Directory.Delete(TEMP_PATH, true);
    }


    private static void splitContainer2_Panel2_ControlAdded(object sender, ControlEventArgs e)
    {
        byteviewer.Width = splitContainer2.Panel2.Width;
        byteviewer.Height = splitContainer2.Panel2.Height;
    }

    private static string ReadResource(string name)
    {
        // Determine path
        var assembly = Assembly.GetExecutingAssembly();
        string resourcePath = name;
        // Format: "{Namespace}.{Folder}.{filename}.{Extension}"
        if (!name.StartsWith(nameof(SignificantDrawerCompiler)))
        {
            resourcePath = assembly.GetManifestResourceNames()
                .Single(str => str.EndsWith(name));
        }

        using (Stream stream = assembly.GetManifestResourceStream(resourcePath))
        using (StreamReader reader = new StreamReader(stream))
        {
            return reader.ReadToEnd();
        }
    }

    private static object SignificantDrawerCompiler()
    {
        throw new NotImplementedException();
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

                    cmnt.Directory.Sort((p, q) => p.FlOffset.CompareTo(q.FlOffset));

                    paths = new string[cmnt.FilenameDirectory_.Count];
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
}