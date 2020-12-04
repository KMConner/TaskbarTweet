using System;

namespace CustomAction
{
    class Program
    {
        static int Main(string[] args)
        {
            if (args.Length != 1)
            {
                Console.WriteLine("No action specified");
                return -1;
            }

            switch (args[0])
            {
                case "install":
                    Actions.Install();
                    break;
                case "uninstall":
                    Actions.Uninstall();
                    break;
                default:
                    Console.WriteLine("Invalid action");
                    return 1;
            }

            return 0;
        }
    }
}
