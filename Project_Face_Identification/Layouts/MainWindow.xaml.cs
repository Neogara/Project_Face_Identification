using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;




namespace Project_Face_Identification.Layouts
    
{
    
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        
        private void OpenMainCamera_Click(object sender, RoutedEventArgs e)
        {
            var CameraMindow = new MainCameraWindow();
            CameraMindow.Show();

        }
     
       

        private void CloseMainCameraButton_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
