using System;
using System.Windows.Forms;

class Window
{
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
            Application.EnableVisualStyles();
            Form form = new Form();

            MenuStrip menuStrip = new MenuStrip();
            menuStrip.Dock = DockStyle.Top;

            ToolStripMenuItem thisFileItem = new ToolStripMenuItem("&File");
            thisFileItem.DropDownItems.Add("&Clear");
            thisFileItem.DropDownItems.Add("E&xit");

            menuStrip.Items.Add(thisFileItem);
            menuStrip.Name = "menuStrip";
            form.Controls.Add(menuStrip);


            Application.Run(form);

        }
    }
}