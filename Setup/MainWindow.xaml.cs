using System.ComponentModel;
using System.Windows;
using System;
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

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
        }

        private void NotifyProperyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private void OkCommandExecute()
        {
            Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);

            Close();
        }

        private bool OkCommandCanExecute()
        {
            return !string.IsNullOrWhiteSpace(apiKey) && !string.IsNullOrWhiteSpace(ApiSecret) &&
                !string.IsNullOrWhiteSpace(AccessToken) && !string.IsNullOrWhiteSpace(AccessTokenSecret);
        }

        private void SaveToIni()
        {

        }
    }
}
