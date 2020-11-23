using System.ComponentModel;
using System.Windows;
using System;
using System.IO;
using MvvmCross.Commands;

namespace Setup
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        private string apiKey;

        private string apiSecret;

        private string accessToken;

        private string accessTokenSecret;

        private IMvxCommand cancelCommand;

        private IMvxCommand okCommand;

        private IMvxCommand loadCommand;

        private readonly string iniPath;

        public string ApiKey
        {
            get { return apiKey; }
            set
            {
                apiKey = value;
                NotifyProperyChanged(nameof(ApiKey));
                OkCommand.RaiseCanExecuteChanged();
            }
        }

        public string ApiSecret
        {
            get { return apiSecret; }
            set
            {
                apiSecret = value;
                NotifyProperyChanged(nameof(ApiSecret));
                OkCommand.RaiseCanExecuteChanged();
            }
        }

        public string AccessToken
        {
            get { return accessToken; }
            set
            {
                accessToken = value;
                NotifyProperyChanged(nameof(AccessToken));
                OkCommand.RaiseCanExecuteChanged();
            }
        }

        public string AccessTokenSecret
        {
            get { return accessTokenSecret; }
            set
            {
                accessTokenSecret = value;
                NotifyProperyChanged(nameof(AccessTokenSecret));
                OkCommand.RaiseCanExecuteChanged();
            }
        }

        public IMvxCommand CancelCommand
        {
            get => cancelCommand ??= new MvxCommand(Close);
        }

        public IMvxCommand OkCommand
        {
            get => okCommand ??= new MvxCommand(OkCommandExecute, OkCommandCanExecute);
        }

        public IMvxCommand LoadCommand
        {
            get => loadCommand ??= new MvxCommand(LoadFromIni);
        }

        public MainWindow()
        {
            InitializeComponent();
            iniPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), @"TaskbarTweet\auth.ini");
            DataContext = this;
        }

        private void NotifyProperyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private void OkCommandExecute()
        {
            Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
            try
            {
                SaveIni();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to save config file: \r\n" + ex.Message);
                return;
            }

            Close();
        }

        private bool OkCommandCanExecute()
        {
            return !string.IsNullOrWhiteSpace(apiKey) && !string.IsNullOrWhiteSpace(ApiSecret) &&
                !string.IsNullOrWhiteSpace(AccessToken) && !string.IsNullOrWhiteSpace(AccessTokenSecret);
        }

        private void LoadFromIni()
        {
            ApiKey = IniFile.Read(iniPath, "ConsumerKey", "account");
            ApiSecret = IniFile.Read(iniPath, "ConsumerSecret", "account");
            AccessToken = IniFile.Read(iniPath, "AccessToken", "account");
            AccessTokenSecret = IniFile.Read(iniPath, "AccessTokenSecret", "account");
        }

        private void SaveIni()
        {
            IniFile.Write(iniPath, "ConsumerKey", ApiKey, "account");
            IniFile.Write(iniPath, "ConsumerSecret", ApiSecret, "account");
            IniFile.Write(iniPath, "AccessToken", AccessToken, "account");
            IniFile.Write(iniPath, "AccessTokenSecret", AccessTokenSecret, "account");
        }
    }
}
