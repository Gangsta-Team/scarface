using Kaitai;
using System;
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
    public static TreeView tv = null;
    public static TreeNode tvnode = null;
    const string WINDOW_TITLE = "Scarface The World Is Yours: RCF Explorer";

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

            MenuStrip menuStrip = new MenuStrip();
            menuStrip.Dock = DockStyle.Top;

            ToolStripMenuItem thisFileItem = new ToolStripMenuItem("&File");
            thisFileItem.DropDownItems.Add("Open RCF");
            thisFileItem.DropDownItems.Add("Exit");

            thisFileItem.DropDownItems[0].Click += Item_Click;
            thisFileItem.DropDownItems[1].Click += Item_Click;

            menuStrip.Items.Add(thisFileItem);
            menuStrip.Name = "menuStrip";
            form.Controls.Add(menuStrip);

            tv = new TreeView();
            tv.Dock = DockStyle.Fill;

            Panel p1 = new Panel();
            p1.Dock = DockStyle.Fill;

            p1.Controls.Add(tv);

            Panel p2 = new Panel();
            p2.Dock = DockStyle.Fill;

            SplitContainer sp = new SplitContainer();
            sp.Panel1.Controls.Add(p1);
            sp.Panel2.Controls.Add(p2);

            sp.Dock = DockStyle.Fill;

            form.Controls.Add(sp);

            Application.Run(form);

        }
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
                    form.Text = WINDOW_TITLE + " - File: " + openFileDialog.FileName;
                    cmnt = Cement.FromFile(openFileDialog.FileName);
                    MessageBox.Show("Count of files: "+cmnt.FilenameDirectory_.Count);
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