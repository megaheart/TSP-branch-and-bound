using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WpfApp1.Algorithm;

namespace WpfApp1
{
    public class PointInfo
    {
        public Ellipse Shape { get; set; }
        public TextBlock Index { get; set; }
        public Point posOnBoard { get; set; }
    }
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ObservableCollection<Point> points;
        List<PointInfo> pointShapes;
        public MainWindow()
        {
            InitializeComponent();
            points = new ObservableCollection<Point>();
            PosLog.ItemsSource = points;
            pointShapes = new List<PointInfo>();
        }
        private void Canvas_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            Point p = Mouse.GetPosition(this);
            double x = Math.Round(p.X / 4.5, 6);
            double y = Math.Round(p.Y / 4.5, 6);
            points.Add(new Point(x, y));
            PointInfo pointInfo = new PointInfo();
            pointInfo.posOnBoard = p;
            pointInfo.Shape = new Ellipse();
            pointInfo.Shape.Width = 10;
            pointInfo.Shape.Height = 10;
            pointInfo.Shape.Fill = Brushes.Red;
            Canvas.SetLeft(pointInfo.Shape, p.X - 5);
            Canvas.SetTop(pointInfo.Shape, p.Y - 5);
            pointInfo.Index = new TextBlock();
            pointInfo.Index.Text = "[" + points.Count + "]";
            pointInfo.Index.FontSize = 14;
            Panel.SetZIndex(pointInfo.Index, 9999);
            if (p.X > 33 && p.Y > 28)
            {
                Canvas.SetLeft(pointInfo.Index, p.X - 30);
                Canvas.SetTop(pointInfo.Index, p.Y - 25);
            }
            else
            {
                Canvas.SetLeft(pointInfo.Index, p.X + 5);
                Canvas.SetTop(pointInfo.Index, p.Y + 5);
            }
            board.Children.Add(pointInfo.Shape);
            board.Children.Add(pointInfo.Index);
            pointShapes.Add(pointInfo);
        }
        private void RemovePoint(object sender, RoutedEventArgs e)
        {
            Button button = sender as Button;
            Point p = (Point)button.DataContext;
            int index = points.IndexOf(p);
            points.RemoveAt(index);
            board.Children.Remove(pointShapes[index].Shape);
            board.Children.Remove(pointShapes[index].Index);
            pointShapes.RemoveAt(index);
            for (; index < points.Count; index++)
            {
                pointShapes[index].Index.Text = "[" + (index + 1) + "]";
            }
        }
        string ip = "";
        HttpClient client = new HttpClient();
        private void Start_TSP(object sender, RoutedEventArgs e)
        {
            (sender as Button).IsEnabled = false;
            double[][] A = new double[points.Count][];
            for (int i = 0; i < points.Count; i++)
            {
                A[i] = new double[points.Count];
                A[i][i] = double.MaxValue;
            }
            for (int i = 0; i < points.Count; i++)
            {
                for (int j = i + 1; j < points.Count; j++)
                {
                    A[i][j] = A[j][i]
                        = Math.Round(Math.Sqrt(Math.Pow((points[i].X - points[j].X), 2) + Math.Pow((points[i].Y - points[j].Y), 2)), 6);
                }
            }
            //string message = points.Count.ToString();
            //for (int i = 0; i < points.Count; i++)
            //{
            //    for (int j = 0; j < points.Count; j++)
            //    {
            //        if(A[i][j] < 1E300)
            //        {
            //            Console.Write("{0,12} ", A[i][j]);
            //        }
            //        else
            //        {
            //            Console.Write("{0,12} ", "∞");
            //        }
            //        message = " " + A[i][j];
            //    }
            //    Console.WriteLine();
            //    Console.WriteLine("Sending Data.........................");
            //}
            //var stringContent = new StringContent(message.ToString());
            //var response = client.PostAsync("192.168.1.13", stringContent).Result;
            //Console.WriteLine("Response: {0}", response.Content.ToString());
            //new CheckAllWays().Result_Run(A, A.Length);
            new Thread(new ThreadStart(() =>
            {
                int[] tsp_actual = new TSP().TSP_Run(A, A.Length);
                board.Dispatcher.Invoke(new Action(() =>
                {
                    this.Draw(tsp_actual);
                }), System.Windows.Threading.DispatcherPriority.ApplicationIdle);
            })).Start();
        }
        private void Draw(int[] tsp_actual)
        {
            Line myLine;
            for (int i = 1;i < tsp_actual.Length; i++)
            {
                int h = tsp_actual[i - 1];
                int f = tsp_actual[i];
                myLine = new Line();
                myLine.Stroke = Brushes.Green;
                myLine.X1 = pointShapes[h].posOnBoard.X;
                myLine.X2 = pointShapes[f].posOnBoard.X;
                myLine.Y1 = pointShapes[h].posOnBoard.Y;
                myLine.Y2 = pointShapes[f].posOnBoard.Y;
                myLine.HorizontalAlignment = HorizontalAlignment.Center;
                myLine.VerticalAlignment = VerticalAlignment.Center;
                myLine.StrokeThickness = 2;
                board.Children.Add(myLine);
            }
        }
        private void Reset(object sender, RoutedEventArgs e)
        {
            points.Clear();
            pointShapes.Clear();
            board.Children.Clear();
            startBtn.IsEnabled = true;
        }
        private void Import_Data(object sender, RoutedEventArgs e)
        {
            var dialog = new Microsoft.Win32.OpenFileDialog();
            dialog.DefaultExt = ".txt"; // Default file extension
            dialog.Filter = "Text documents (.txt)|*.txt"; // Filter files by extension
            dialog.Multiselect = false;

            // Show open file dialog box
            bool? result = dialog.ShowDialog();
            if (result == true)
            {
                points.Clear();
                string filename = dialog.FileName;
                string[] pointsRaw = File.ReadAllText(filename).Trim().Split('\n');
                foreach (string line in pointsRaw)
                {
                    if (string.IsNullOrEmpty(line)) continue;
                    string[] pointRaw = line.Split(',');
                    if (pointRaw.Length != 2) continue;
                    double x, y;
                    if (double.TryParse(pointRaw[0], out x) && double.TryParse(pointRaw[1], out y))
                    {
                        points.Add(new Point(x, y));
                    }
                }
            }
        }
        private void Export_Data(object sender, RoutedEventArgs e)
        {
            var dialog = new Microsoft.Win32.SaveFileDialog();
            dialog.FileName = "TSP data " + DateTime.Now.ToString("yyyy-MM-dd hh mm ss"); // Default file name
            dialog.DefaultExt = ".txt"; // Default file extension
            dialog.Filter = "Text documents (.txt)|*.txt"; // Filter files by extension

            // Show save file dialog box
            bool? result = dialog.ShowDialog();
            if (result == true)
            {
                string filename = dialog.FileName;
                string outp = "";
                foreach (Point p in points)
                {
                    outp += p.X + "," + p.Y + "\n";
                }
                File.WriteAllText(filename, outp.Remove(outp.Length - 1));
            }
        }
    }
}
